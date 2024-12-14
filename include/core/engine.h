//
//  engine.h
//  Created by Ben Westcott on 11/18/24.
//

#include <cstdint>
#include <mdspan>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <complex>
#include <tuple>
#include <functional>

template<typename From, typename To>
struct static_value_caster
{
    To operator()(From f) const
    {
        return static_cast<To>(f);
    }
};

// https://stackoverflow.com/questions/31762958/check-if-class-is-a-template-specialization
template<typename T, template<typename ...> typename Tmplte>
struct is_specialization : std::false_type
{};

template<template<typename ...> typename Tmplte, typename ... Args>
struct is_specialization<Tmplte<Args...>, Tmplte> : std::true_type
{};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * see:
 * [1] https://stackoverflow.com/questions/55288555/c-check-if-statement-can-be-evaluated-constexpr.
 * 
 * essentially, since lambdas can be constexpr and captureless lambdas can be default constructed (>= C++20)
 * then if a lambda appears as a template argument, then it must be a constant expression.
 * 
 * So if a constant expression appears within a captureless lambda, then it can be resolved as a template argument
 * otherwise it cant, and is_constexpr(...) is used.
 * 
 */ 
template<typename Lambda, int = (Lambda{}(), 0)> 
inline constexpr bool is_constexpr(Lambda)
{
    return true;
}

inline constexpr bool is_constexpr(...)
{
    return false;
}

// detects whether cols, rows or size methods are constexpr (see above note referencing [1])
template<typename Egn>
inline constexpr bool col_constexpr = is_constexpr([]{Egn().cols();});

template<typename Egn>
inline constexpr bool row_constexpr = is_constexpr([]{Egn().rows();});

template<typename Egn>
inline constexpr bool size_constexpr = is_constexpr([]{Egn().size();});

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * see:
 * [2] https://en.cppreference.com/w/cpp/types/void_t
 * 
 *  Detecting whether a type contains a nested type can be done by
 *  having a primary template which handles any type (inherits std::false_type)
 * 
 *  but then a template specialization which void casts the nested type in question will be chosen 
 *  if the type has that nested type.
 * 
 *  So then primary template is only chosen for types which do not fit the specialization.
 * 
 *  Here, this is used to determine whether the engine type contains a type alias called "owning_engine_type"
 *  If it has this type, then this implies that the engine type is not itself an owning engine, but rather points
 *  to an owning engine, or in some way is associated with an owning engine, but doesnt locally own any data. 
 * 
 */
template<typename Egn, typename = void>
struct has_owning_engine_type_alias : public std::false_type
{
    static constexpr bool is_owning = true;
    using owning_engine_type = Egn;
};

template<typename Egn>
struct has_owning_engine_type_alias<Egn, std::void_t<typename Egn::owning_engine_type>> : public std::true_type
{
    static constexpr bool is_owning = false;
    using owning_engine_type = typename Egn::owning_engine_type;
};

template<typename Egn>
inline constexpr bool is_owning_engine = has_owning_engine_type_alias<Egn>::is_owning;

template<typename Egn>
using get_owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * will extract the nbr of rows and cols of engine 
 * if they are evaluated at compiletime, then they are returned
 * if they are not, then they can only be std::dynamic_extent at compiletime so return that.
 * uses same trick seen in [1]
 * 
 */ 
template<typename Egn>
struct engine_ct_extents
{
private:
    
    template<typename Lambda, int = (Lambda{}(), 0)>
    static constexpr size_t get_ct_value(Lambda)
    {
        return Lambda{}();
    }

    // if param isnt satisfied by above template, then this one gets called:
    static constexpr size_t get_ct_value(...)
    {
        return std::dynamic_extent;
    }

public:

    // reaches dont need these since if std::dynamic_extent is returned for one
    // then it is implied that its corresponding reach is also dynamic

    static constexpr size_t size()
    {
        return get_ct_value([]{ return static_cast<size_t>(Egn().size());});
    }

    static constexpr bool is_constexpr_size()
    {
        return size() != 0 && size_constexpr<Egn>;
    }

    static constexpr size_t rows()
    {
        return get_ct_value([]{ return static_cast<size_t>(Egn().rows());});
    }

    static constexpr bool is_constexpr_rows()
    {
        return rows() != 0 && row_constexpr<Egn>;
    }

    static constexpr size_t cols()
    {
        return get_ct_value([]{ return static_cast<size_t>(Egn().cols());});
    }

    static constexpr bool is_constexpr_cols()
    {
        return (cols() != 0) && col_constexpr<Egn>;
    }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  matrix orientation definitions
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct matrix_orientation
{
    struct row_major {};
    struct col_major {};
    struct none {};

    template<typename S>
    requires
        std::integral<S>
    static constexpr S offset(S major, S minor, S minor_length)
    {
        return minor + major * minor_length;
    }

    template<typename S>
    requires
        std::integral<S>
    static constexpr std::tuple<S, S> offset(S index, S minor_length)
    {
        S major = index/minor_length;
        S minor = index - major * minor_length;

        return std::make_tuple(major, minor);
    }
};


template<typename X, typename = void>
struct get_index_type : public std::false_type
{
    using type = std::size_t;
};

// I used "index_type" instead of "size_type" up until now, and did not realize
// that the standard library typically uses "size_type" for the indexing type,
// so that's why there are two overloads here.
/*
template<typename X>
struct get_index_type<X, std::void_t<typename X::size_type>> : public std::true_type
{
    using type = typename X::size_type;
};*/

template<typename X>
struct get_index_type<X, std::void_t<typename X::index_type>> : public std::true_type
{
    using type = typename X::index_type;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  The following two use the type detection idiom shown in [2]
 *  to extract the matrix orientation of the supplied engine type
 * 
 */

template<typename Egn, typename = void>
struct get_engine_orientation
{
    using type = matrix_orientation::none;
};


template<typename Egn>
struct get_engine_orientation<Egn, std::void_t<typename Egn::orientation_type>>
{
    using type = Egn::orientation_type;
};

template<typename L>
struct get_engine_transpose_orientation
{
    using type = matrix_orientation::none;
};

template<>
struct get_engine_transpose_orientation<matrix_orientation::col_major>
{
    using type = matrix_orientation::row_major;
};

template<>
struct get_engine_transpose_orientation<matrix_orientation::row_major>
{
    using type = matrix_orientation::col_major;
};

// If engine orientation isnt row, or col major, then the engine cannot be indexed, 
// and thus cannot be readble and thus cannot be writable. 
template<typename L>
concept valid_storage_orientation = 
    std::same_as<L, matrix_orientation::row_major> or
    std::same_as<L, matrix_orientation::col_major>;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Engine concepts
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template<typename Egn>
concept arithmetic_engine = std::is_arithmetic_v<typename Egn::data_type>;

template<typename Egn>
concept integral_engine = std::integral<typename Egn::data_type>;

template<typename Egn>
concept floating_point_engine = std::floating_point<typename Egn::data_type>;

template<typename X>
concept valid_unary_minus_operator = requires(X && x)
{
    { -x } -> std::convertible_to<X>;
};

template<typename X>
concept valid_binary_star_operator = requires(X && x1, X && x2)
{
    { x1 * x2 } -> std::convertible_to<X>;
    { x2 * x1 } -> std::convertible_to<X>;
};

template<typename T1, typename T2>
concept comparable_types_one_sided = requires(T1 t1, T2 t2)
{
    { t1 == t2 } -> std::same_as<bool>;
};

template<typename T1, typename T2>
concept comparable_types = 
    comparable_types_one_sided<T1, T2> and 
    comparable_types_one_sided<T2, T1>;


template<typename X>
concept has_conjugate = requires(X const& x)
{
    { std::conj(x) } -> std::convertible_to<X>;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Any engine needs to atleast state these type aliases.
 * 
 */
template<typename Egn>
concept base_types = requires (Egn const& eng)
{
    typename Egn::data_type;
    typename Egn::index_type;
    typename Egn::reference;
    typename Egn::const_reference;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Checks for ensuring that reference type aliases are consistent with
 *  taking a reference of the stated data type alias.
 * 
 */
template<typename Egn>
concept consistent_mutable_ref_type = 
    std::same_as<typename Egn::reference, typename Egn::data_type&>;

template<typename Egn>
concept consistent_immutable_ref_type = 
    std::same_as<typename Egn::const_reference, typename Egn::data_type const&>;

// enforces requirement that engine data type can be converted from reference types
template<typename Egn>
concept convertible_refs = 
    std::convertible_to<typename Egn::const_reference, typename Egn::data_type> and 
    std::convertible_to<typename Egn::reference, typename Egn::data_type>;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Applies constraint that compile-time
 * dimensions cannot be zero, even if they're std::dynamic_extent
 * 
 */
template<size_t R, size_t C>
concept template_dimensions = 
    (R > 0) and (C > 0);

template<size_t R, size_t C>
concept fixed_template_dimensions = 
    (R > 0) and (R != std::dynamic_extent) and 
    (C > 0) and (C != std::dynamic_extent);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Allocator support
 * 
 */
#include "allocator_support.h"

/*
 * The allocator type for a storage engine can be void only if
 * fixed template dimensions are also supplied.
 */

template<typename T, size_t R, size_t C, typename Alloc>
concept engine_allocator = 
    (allocator_type_is_void<Alloc> and fixed_template_dimensions<R, C>) or
    (base_allocator_interface<Alloc> and template_dimensions<R, C>);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Checks for ensuring that return types of size and length
 *  methods are consistent with the stated indexing type
 * 
 */
template<typename X, typename IT>
concept non_static_dimensions = requires (X && x)
{
    { x.rows() } -> std::same_as<IT>;
    { x.cols() } -> std::same_as<IT>;
};

template<typename X, typename IT, typename C>
concept static_dimensions = requires (C && c)
{
    { X::rows(c) } -> std::same_as<IT>;
    { X::cols(c) } -> std::same_as<IT>;
};

template<typename X, typename IT, typename C>
concept dimensions = 
    non_static_dimensions<X, IT> or
    static_dimensions<X, IT, void> or
    static_dimensions<X, IT, C>;

template<typename X>
concept rowvec_dimensions = 
    1 == engine_ct_extents<X>::rows();

template<typename X>
concept colvec_dimensions = 
    1 == engine_ct_extents<X>::cols();

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Checks for checking whether the type X has a access operator
 *      X : (engine) type
 *      CT: constructor type (ctor type), usually engine_type& or const&
 *      IT: index type
 *      C : return type, usually reference, or const reference or data type
 */

template<typename X, typename CT, typename IT, typename C>
concept static_evaluation = requires(CT && ct, IT && it)
{
    { X::operator()(ct, it, it) } -> std::same_as<C>;
};

template<typename X, typename CT, typename IT, typename C>
concept non_static_evalutation = requires(X && x, CT && ct, IT && it)
{
    { x(ct, it, it) } -> std::same_as<C>;
};



template<typename Egn>
concept consistent_return_sizes = requires (Egn && eng)
{
    { eng.size() } -> std::same_as<typename Egn::index_type>;
};

template<typename Egn>
concept consistent_return_lengths = 
    non_static_dimensions<Egn, typename Egn::index_type>;



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Concepts used to require mutable and immutable access.
 * Depending on the type of engine, it may have both, one or neither:
 *  - storage types must have both
 *  - const view types need only immutable access (may have to change)
 *  - non-const view types need only mutable access
 * 
 */
/*
template<typename T, typename Egn>
concept valid_immutable_access_return_type = 
    consistent_immutable_ref_type<Egn> and
    std::same_as<T, typename Egn::const_reference>;
*/

template<typename I, typename Egn>
concept valid_immutable_access_return_type = 
    std::same_as<I, typename Egn::reference> or
    std::same_as<I, typename Egn::const_reference> or
    std::same_as<I, typename Egn::data_type>;

template<typename Egn>
concept immutable_access = requires(Egn const& eng, typename Egn::index_type x)
{
    { eng(x, x) } -> valid_immutable_access_return_type<Egn>;
    { eng(x) } -> valid_immutable_access_return_type<Egn>;
};


template<typename I, typename Egn>
concept valid_mutable_access_return_type =
    consistent_mutable_ref_type<Egn> and
    std::same_as<I, typename Egn::reference>;

template<typename Egn>
concept mutable_access = requires(Egn & eng, typename Egn::index_type x)
{
    { eng(x, x) } -> valid_mutable_access_return_type<Egn>;
    { eng(x) } -> valid_mutable_access_return_type<Egn>;
};

template<typename Egn>
concept base_engine = 
    base_types<Egn> and
    convertible_refs<Egn> and
    consistent_return_sizes<Egn> and
    consistent_return_lengths<Egn>;

template<typename Egn>
concept owning_engine =
    base_engine<Egn> and
    has_owning_engine_type_alias<Egn>::is_owning;

template<typename Egn>
concept non_owning_engine = base_engine<Egn> and not owning_engine<Egn>;

template<typename EgnX, typename EgnY>
concept same_owning_engine =
    non_owning_engine<EgnX> and
    non_owning_engine<EgnY> and
    std::same_as<typename EgnX::owning_engine_type, typename EgnY::owning_engine_type>;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  States the baseline requirements for an engine to
 *  access data
 * 
 */
template<typename Egn>
concept readable_engine = 
    base_engine<Egn> and
    //(immutable_access<Egn> or mutable_access<Egn>);
    immutable_access<Egn>;
    // TODO: maybe swappable?



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  States the baseline requirements for an engine to
 *  access and modify data
 * 
 */
template<typename Egn>
concept writable_engine = 
    readable_engine<Egn> and 
    mutable_access<Egn>;

template<typename EgnX, typename EgnY>
concept comparable_engines = 
    readable_engine<EgnX> and
    readable_engine<EgnY> and
    comparable_types<typename EgnX::data_type, typename EgnY::data_type>;

template<typename Egn, typename T>
concept comparable_engine_and_literal2D = 
    readable_engine<Egn> and
    comparable_types<typename Egn::data_type, T>;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Checks the supplied type for reshaping methods
 * 
 */

template<typename Egn>
concept row_reshapeable_engine = writable_engine<Egn>
and requires(Egn & eng, typename Egn::index_type x)
{
    { eng.row_reach() } -> std::same_as<typename Egn::index_type>;
    { eng.reshape_rows(x, x) };
};

template<typename Egn>
concept col_reshapeable_engine = writable_engine<Egn>
and requires(Egn & eng, typename Egn::index_type x)
{
    { eng.col_reach() } -> std::same_as<typename Egn::index_type>;
    { eng.reshape_cols(x, x) };
};

template<typename Egn>
concept reshapeable_engine = writable_engine<Egn>
and requires(Egn & eng, typename Egn::index_type x)
{
    { eng.reach() } -> std::same_as<typename Egn::index_type>;
    { eng.reshape(x, x, x, x) };
};


/*
 * An engine must have the following at the minimum:
 * {
 *  private type aliases:
 *      using self_type = ...
 *      using orient = matrix_orientation;
 *      using helper = engine_helper;
 *      // using mdhelper = ...     // not implemented yet
 *      
 * 
 *  public type aliases: 
 *      using orientation_type = ...
 *      using data_type = ...
 *      using index_type = ...
 *      using reference = ...
 *      using const_reference = ...
 *      // using mdspan_type = ...      // not implemented yet
 *      // using const_mdspan_type = ...    // not implemented yet
 *  
 *  public methods:
 *      default destructor
 *      default copy, move constructors
 *      default copy, move assignment
 * 
 *      atleast one element access operator
 * 
 *      rows, row_reach, cols, col_reach, size and reach methods
 * };
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Defines helper methods for dealing with:
 *  - indexing type differences
 *  - invalid sizes and reaches
 *  - engine swapping
 *  - engine moving
 *  - engine copying
 *  - exact element by element comparison
 * 
 */
struct engine_helper
{
    template<typename T>
    using literal2D = std::initializer_list<std::initializer_list<T>>;

    template<typename T>
    using literal1D = std::initializer_list<T>;

    template<typename X, typename Y>
    static constexpr bool sizes_equal(X x, Y y)
    {
        using com = std::common_type_t<X, Y, size_t>;

        return static_cast<com>(x) == static_cast<com>(y);
    }

    template<typename R1, typename C1, typename R2, typename C2>
    static constexpr bool sizes_equal(R1 r1, C1 c1, R2 r2, C2 c2)
    {
        using com = std::common_type_t<R1, C1, R2, C2, size_t>;

        bool rows_eq = static_cast<com>(r1) == static_cast<com>(r2);
        bool cols_eq = static_cast<com>(c1) == static_cast<com>(c2);

        return rows_eq && cols_eq;
    }

    template<typename R1, typename C1, typename R2, typename C2>
    static constexpr bool sizes_compatible(R1 r1, C1 c1, R2 r2, C2 c2)
    {
        using com = std::common_type_t<R1, C1, R2, C2, size_t>;

        com size1 = static_cast<com>(r1) * static_cast<com>(c1);
        com size2 = static_cast<com>(r2) * static_cast<com>(c2);

        return size1 == size2;
    }

    template<typename X>
    static constexpr void validate_length(X x)
    {
        //return x >= static_cast<X>(0);
        if(x < static_cast<X>(1))
        {
            throw std::runtime_error("invalid length.");
        }
    }

    template<typename X>
    static constexpr void validate_reach(X x)
    {
        //return x > static_cast<X>(0);
        if(x < static_cast<X>(0))
        {
            throw std::runtime_error("invalid reach.");
        }
    }
    
    template<typename X, typename Y>
    static constexpr void validate_size(X x, Y y)
    {
        if(!sizes_equal(x, y))
        {
            throw std::runtime_error("invalid size.");
        }
    }

    template<typename T>
    static constexpr void validate_literal2D(literal2D<T> lst)
    {
        size_t inner_len = lst.begin()->size();
        for(auto x : lst)
        {
            if(x.size() != inner_len)
            {
                throw std::runtime_error("invalid initializer list.");
            }
        }
    }

    template<typename T>
    static constexpr void swap(T & t1, T & t2) noexcept
    {
        if(&t1 != &t2)
        {
            T tmp = std::move(t1);
            t1 = std::move(t2);
            t2 = std::move(tmp);
        }
    }

    template<typename Egn, typename R, typename C>
    static constexpr void validate(Egn & eng, R nbr_rows, C nbr_cols)
    requires writable_engine<Egn>
    {
        auto nrows = static_cast<typename Egn::index_type>(nbr_rows);
        auto ncols = static_cast<typename Egn::index_type>(nbr_cols);

        if constexpr(reshapeable_engine<Egn>)
        {
            if(nrows != eng.rows() || ncols != eng.cols())
            {
                eng.reshape(nrows, eng.row_reach(), ncols, eng.col_reach());
            }
        }
        else if constexpr(row_reshapeable_engine<Egn>)
        {
            validate_size(ncols, eng.cols());
            if(nrows != eng.rows())
            {
                eng.reshape_rows(nrows, eng.row_reach());
            }
        }
        else if constexpr(col_reshapeable_engine<Egn>)
        {
            validate_size(nrows, eng.rows());
            if(ncols != eng.cols())
            {
                eng.reshape_cols(ncols, eng.col_reach());
            }
        }
        else
        {
            validate_size(ncols, eng.cols());
            validate_size(nrows, eng.rows());
        }
    }

    //template<class Egn, typename X>
    //static constexpr void validate(Egn & eng, X size);

    template<typename EgnX, typename EgnY>
    static constexpr void copy2(EgnX const& src, EgnY & dst)
    requires
        std::convertible_to<typename EgnX::data_type, typename EgnY::data_type> and
        readable_engine<EgnX> and
        writable_engine<EgnY>
    {
        using src_index_type = EgnX::index_type;
        
        using dst_index_type = EgnY::index_type;
        using dst_data_type = typename EgnX::data_type;
        
        src_index_type nbr_src_rows = src.rows();
        src_index_type nbr_src_cols = src.cols();

        // may reshape dst
        validate(dst, nbr_src_rows, nbr_src_cols);
    
        src_index_type src_idx, src_jdx;
        dst_index_type dst_idx, dst_jdx;

        src_idx = dst_idx = 0;
        for(; src_idx < nbr_src_rows; src_idx++, dst_idx++)
        {
            src_jdx = dst_jdx = 0;
            for(; src_jdx < nbr_src_cols; src_jdx++, dst_jdx++)
            {
                dst(dst_idx, dst_jdx) = static_cast<dst_data_type>(src(src_idx, src_jdx));
            }
        }
    
    }

    //template<class CntrX, class EgnY>
    //static constexpr void copy2(CntrX const& src, EgnY & dst);

    template<typename T, typename EgnY>
    static constexpr void copy2(literal2D<T> src, EgnY & dst)
    requires
        std::convertible_to<T, typename EgnY::data_type> and
        writable_engine<EgnY>
    {
        using index_type = typename EgnY::index_type;
        using data_type = typename EgnY::data_type;

        validate_literal2D(src);

        auto nbr_rows = src.size();
        auto nbr_cols = src.begin()->size();

        // may reshape dst
        validate(dst, nbr_rows, nbr_cols);

        index_type dst_idx, dst_jdx;
        auto src_idx = src.begin();

        dst_idx = dst_jdx = static_cast<index_type>(0);

        for(; dst_idx < dst.rows(); dst_idx++, src_idx++)
        {
            auto src_jdx = src_idx->begin();
            for(dst_jdx = 0; dst_jdx < dst.cols(); dst_jdx++, src_jdx++)
            {
                dst(dst_idx, dst_jdx) = static_cast<data_type>(*src_jdx);
            }
        }
    }
    
    //static constexpr void copy2(Egn & dst, mdspan<T, extents<IT, X0, X1>, SL, SA> const& src)
    //static constexpr void copy2(Egn & dst, Ctnr const& src)
    //static constexpr void copy2(Egn & dst, mdspan<T, extents<IT, X0>, SL, SA> const& src)
    //static constexpr void copy2(Egn & dst, initializer_list<T> src)
    
    template<typename Egn, typename R1, typename R2, typename C1, typename C2>
    static constexpr void move_data(Egn & src, Egn & dst, R1 ilb_inc, R2 iub_ex, C1 jlb_inc, C2 jub_ex)
    requires writable_engine<Egn>
    {
        using index_type = typename Egn::index_type;
        
        index_type istart = static_cast<index_type>(ilb_inc);
        index_type iend = static_cast<index_type>(iub_ex);
        
        index_type jstart = static_cast<index_type>(jlb_inc);
        index_type jend = static_cast<index_type>(jub_ex);
        
        for(index_type i = istart; i < iend; i++)
        {
            for(index_type j = jstart; j < jend; j++)
            {
                dst(i, j) = std::move(src(i, j));
            }
        }
    }
    
    template<typename Egn, typename R, typename C>
    static constexpr void move_data(Egn & src, Egn & dst, R nbr_rows, C nbr_cols)
    requires writable_engine<Egn>
    {
        move_data<Egn, R, R, C, C>(src, dst, 0, nbr_rows, 0, nbr_cols);
    }

    template<typename Egn, typename C1, typename C2, typename T>
    static constexpr void fill_cols(Egn & dst, C1 jlb_inc, C2 jub_ex, T const& value)
    requires
        std::convertible_to<T, typename Egn::data_type> and
        writable_engine<Egn>
    {
        using index_type = typename Egn::index_type;
        using data_type = typename Egn::data_type;

        index_type istart = static_cast<index_type>(0);
        index_type iend = dst.rows();
        index_type jstart = static_cast<index_type>(jlb_inc);
        index_type jend = static_cast<index_type>(jub_ex);

        for(index_type i = istart; i < iend; i++)
        {
            for(index_type j = jstart; j < jend; j++)
            {
                dst(i, j) = static_cast<data_type>(value);
            }
        }
    }

    template<typename Egn, typename R1, typename R2, typename T>
    static constexpr void fill_rows(Egn & dst, R1 ilb_inc, R2 iub_ex, T const& value)
    requires
        std::convertible_to<T, typename Egn::data_type> and
        writable_engine<Egn>
    {
        using index_type = typename Egn::index_type;
        using data_type = typename Egn::data_type;

        index_type istart = static_cast<index_type>(ilb_inc);
        index_type iend = static_cast<index_type>(iub_ex);
        index_type jstart = static_cast<index_type>(0);
        index_type jend = dst.cols();

        for(index_type i = istart; i < iend; i++)
        {
            for(index_type j = jstart; j < jend; j++)
            {
                dst(i, j) = static_cast<data_type>(value);
            }
        }
    }

    template<typename EgnX, typename EgnY>
    static constexpr bool compare2D_exact(EgnX const& lhs, EgnY const& rhs)
    requires 
        comparable_engines<EgnX, EgnY>
    {
        using itl = typename EgnX::index_type;
        using itr = typename EgnY::index_type;

        itl lhs_rows = lhs.rows();
        itl lhs_cols = lhs.cols();
        
        itr rhs_rows = rhs.rows();
        itr rhs_cols = rhs.cols();

        if(!sizes_equal(lhs_rows, lhs_cols, rhs_rows, rhs_cols))
        {
            return false;
        }

        itl lhs_i, lhs_j;
        itr rhs_i, rhs_j;

        lhs_i = static_cast<itl>(0);
        rhs_i = static_cast<itr>(0);

        for(; lhs_i < lhs_rows; lhs_i++, rhs_i++)
        {
            lhs_j = static_cast<itl>(0);
            rhs_j = static_cast<itr>(0);

            for(; lhs_j < lhs_cols; lhs_j++, rhs_j++)
            {
                if(lhs(lhs_i, lhs_j) !=  rhs(rhs_i, rhs_j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    template<typename EgnX, typename EgnY>
    static constexpr bool compare1D_exact(EgnX const& lhs, EgnY const& rhs)
    requires
        comparable_engines<EgnX, EgnY>
    {
        using itl = typename EgnX::index_type;
        using itr = typename EgnY::index_type;

        itl lhs_rows = lhs.rows();
        itl lhs_cols = lhs.cols();

        itr rhs_rows = rhs.rows();
        itr rhs_cols = rhs.cols();

        if(!sizes_compatible(lhs_rows, lhs_cols, rhs_rows, rhs_cols))
        {
            return false;
        }

        itl lhs_i = static_cast<itl>(0);
        itr rhs_i = static_cast<itr>(0);

        for(; lhs_i < lhs.size(); lhs_i++, rhs_i++)
        {
            if(lhs(lhs_i) != rhs(rhs_i))
            {
                return false;
            }
        }
        return true;
    }


    template<typename Egn, typename T>
    static constexpr bool compare2D_exact(Egn const& lhs, literal2D<T> rhs)
    requires
        comparable_engine_and_literal2D<Egn, T>
    {
        using itl = typename Egn::index_type;

        itl lhs_cols = lhs.cols();
        itl lhs_rows = lhs.rows();

        size_t rhs_cols = rhs.begin()->size();
        size_t rhs_rows = rhs.size();

        if(!sizes_equal(lhs_rows, lhs_cols, rhs_rows, rhs_cols))
        {
            return false;
        }

        itl lhs_i, lhs_j;
        auto rhs_i = rhs.begin();

        lhs_i = static_cast<itl>(0);
        for(; lhs_i < lhs_rows; lhs_i++, rhs_i++)
        {
            lhs_j = static_cast<itl>(0);
            auto rhs_j = rhs_i->begin();

            for(; lhs_j < lhs_cols; lhs_j++, rhs_j++)
            {
                if(lhs(lhs_i, lhs_j) != *rhs_j)
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    template<typename Egn>
    static constexpr void print(Egn const& rhs)
    requires
        readable_engine<Egn>
    {
        for(size_t r=0; r < rhs.rows(); r++)
        {
            for(size_t c=0; c < rhs.cols(); c++)
            {
                 std::cout << std::setprecision(2) << std::scientific << rhs(r, c) << "\t";
            }
            std::cout << "\n";
        }
    }


    //TODO: 
    //static constexpr bool compare_exact(Egn const& lhs, literal2D<T> rhs);
    //static constexpr bool compare_exact(Egn const& lhs, mdspan<T, extents<IT, X0, X1>, SL, SA> const& src)
    //static constexpr bool compare_exact(Egn & dst, mdspan<T, extents<IT, X0>, SL, SA> const& src)
    //static constexpr bool compare_exact(Egn & dst, initializer_list<T> src)
    //static constexpr bool compare_exact(Egn & dst, Ctnr const& src)

};


#define ENGINE_SUPPORTED

#include "storage_engine.h"
#include "view_engine.h"

