//
//  engine.h
//  Created by Ben Westcott on 11/18/24.
//

#include <cstdint>
#include <mdspan>
//#include <type_traits>
#include <concepts>
#include <algorithm>

template<typename From, typename To>
struct static_value_caster
{
    To operator()(From f) const
    {
        return static_cast<To>(f);
    }
};

// see:
// https://stackoverflow.com/questions/55288555/c-check-if-statement-can-be-evaluated-constexpr.
template<typename Lambda, int = (Lambda{}(), 0)>
constexpr bool is_constexpr(Lambda)
{
    return true;
}

constexpr bool is_constexpr(...)
{
    return false;
}

template<typename Egn>
constexpr inline bool is_col_static_v = is_constexpr([]{Egn().cols(); });

template<typename Egn>
constexpr inline bool is_row_static_v = is_constexpr([]{Egn().rows(); });

template<typename Egn>
constexpr inline bool is_size_static_v = is_constexpr([]{Egn().size(); });


struct matrix_orientation
{
    struct row_major_t{};
    struct col_major_t{};
    struct none_t{};

    template<typename S>
    static constexpr S offset(S major, S minor, S minor_length)
    {
        return minor + major * minor_length;
    }
};

template<typename L>
concept valid_storage_orientation = 
    std::is_same_v<L, matrix_orientation::row_major_t> or
    std::is_same_v<L, matrix_orientation::col_major_t>;

/*
 *
 * Engine concepts
 * 
 */

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
 *  Checks for ensuring that return types of size and length
 *  methods are consistent with the stated indexing type
 * 
 */

template<typename Egn>
concept consistent_return_sizes = requires (Egn const& eng)
{
    std::same_as<decltype(eng.size()), typename Egn::index_type>;
    std::same_as<decltype(eng.reach()), typename Egn::index_type>;
};

template<typename Egn>
concept consistent_return_lengths = requires (Egn const& eng)
{
    std::same_as<decltype(eng.rows()), typename Egn::index_type>;
    std::same_as<decltype(eng.cols()), typename Egn::index_type>;
    std::same_as<decltype(eng.row_reach()), typename Egn::index_type>;
    std::same_as<decltype(eng.col_reach()), typename Egn::index_type>;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Concepts used to require mutable and immutable access.
 * Depending on the type of engine, it may have both, one or neither:
 *  - storage types must have both
 *  - const view types need only immutable access (may have to change)
 *  - non-const view types need only mutable access
 * 
 */

template<typename T, typename Egn>
concept valid_immutable_access_return_type = 
    consistent_immutable_ref_type<Egn> and
    std::same_as<T, typename Egn::const_reference>;

template<typename Egn>
concept immutable_access = requires(Egn const& eng, typename Egn::index_type x)
{
    valid_immutable_access_return_type<decltype(eng(x, x)), Egn>;
    valid_immutable_access_return_type<decltype(eng(x)), Egn>;
};

template<typename T, typename Egn>
concept valid_mutable_access_return_type =
    consistent_mutable_ref_type<Egn> and
    std::same_as<T, typename Egn::reference>;

template<typename Egn>
concept mutable_access = requires(Egn const& eng, typename Egn::index_type x)
{
    valid_mutable_access_return_type<decltype(eng(x, x)), Egn>;
    valid_mutable_access_return_type<decltype(eng(x)), Egn>;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  States the baseline requirements for an engine to
 *  access data
 * 
 */

template<typename Egn>
concept readable_engine = 
    base_types<Egn> and 
    //convertible_refs<Egn> and 
    consistent_mutable_ref_type<Egn> and
    consistent_immutable_ref_type<Egn> and
    consistent_return_sizes<Egn> and
    consistent_return_lengths<Egn> and
    immutable_access<Egn>;

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Checks the supplied type for reshaping methods
 * 
 */

template<typename Egn>
concept reshapeable_engine = requires(Egn & eng, typename Egn::index_type x)
{
    {eng.reshape(x, x, x, x)};
};

template<typename Egn>
concept row_reshapeable_engine = requires(Egn & eng, typename Egn::index_type x)
{
    {eng.reshape_rows(x, x)};
};

template<typename Egn>
concept col_reshapeable_engine = requires(Egn & eng, typename Egn::index_type x)
{
    {eng.reshape_cols(x, x)};
};

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
    using rect_init_list = std::initializer_list<std::initializer_list<T>>;

    template<typename T>
    using init_list = std::initializer_list<T>;

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
    static constexpr void validate_init_list(rect_init_list<T> lst)
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

    template<class Egn, typename R, typename C>
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
                eng.reshape_cols(ncols, eng.col_reach);
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

    template<class EgnX, class EgnY>
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
    static constexpr void copy2(rect_init_list<T> src, EgnY & dst)
    requires
        std::convertible_to<T, typename EgnY::data_type> and
        writable_engine<EgnY>
    {
        using index_type = typename EgnY::index_type;
        using data_type = typename EgnY::data_type;

        validate_init_list(src);

        auto nbr_rows = src.size();
        auto nbr_cols = src.begin()->size();

        // may reshape dst
        validate(dst, nbr_rows, nbr_cols);

        index_type dst_idx, dst_jdx;
        auto src_idx = src.begin();

        dst_idx = dst_jdx = static_cast<data_type>(0);

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
    
    template<class Egn, typename R1, typename R2, typename C1, typename C2>
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
                src(i, j) = std::move(dst(i, j));
            }
        }
    }
    
    template<class Egn, typename R, typename C>
    static constexpr void move_data(Egn & src, Egn & dst, R nbr_rows, C nbr_cols)
    requires writable_engine<Egn>
    {
        move_data<Egn, R, R, C, C>(src, dst, 0, nbr_rows, 0, nbr_cols);
    }

    template<class Egn, typename C1, typename C2, typename T>
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

    template<class Egn, typename R1, typename R2, typename T>
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

    template<class EgnX, class EgnY>
    static constexpr bool compare_exact(EgnX const& lhs, EgnY const& rhs)
    requires 
        readable_engine<EgnX> and 
        readable_engine<EgnY>
    {
        using index_type_lhs = typename EgnX::index_type;
        using index_type_rhs = typename EgnY::index_type;

        index_type_lhs lhs_nbr_rows = lhs.rows();
        index_type_lhs lhs_nbr_cols = lhs.cols();
        
        index_type_rhs rhs_nbr_rows = rhs.rows();
        index_type_rhs rhs_nbr_cols = rhs.cols();

        if(!sizes_equal(lhs_nbr_rows, lhs_nbr_cols, rhs_nbr_rows, rhs_nbr_cols))
        {
            return false;
        }

        index_type_lhs lhs_i, lhs_j;
        index_type_rhs rhs_i, rhs_j;

        lhs_i = static_cast<index_type_lhs>(0);
        rhs_i = static_cast<index_type_rhs>(0);

        for(; lhs_i < lhs_nbr_rows; lhs_i++, rhs_i++)
        {
            lhs_j = static_cast<index_type_lhs>(0);
            rhs_j = static_cast<index_type_rhs>(0);

            for(; lhs_j < lhs_nbr_cols; lhs_j++, rhs_j++)
            {
                if(lhs(lhs_i, lhs_j) !=  rhs(rhs_i, rhs_j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    // TODO: 
    //static constexpr bool compare_exact(Egn const& lhs, rect_init_list<T> rhs);
    //static constexpr bool compare_exact(Egn const& lhs, mdspan<T, extents<IT, X0, X1>, SL, SA> const& src)
    //static constexpr bool compare_exact(Egn & dst, mdspan<T, extents<IT, X0>, SL, SA> const& src)
    //static constexpr bool compare_exact(Egn & dst, initializer_list<T> src)
    //static constexpr bool compare_exact(Egn & dst, Ctnr const& src)
};


#define ENGINE_SUPPORTED
#include "storage_engine.h"
