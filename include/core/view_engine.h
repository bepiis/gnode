//
//  view_engine.h
//  Created by Ben Westcott on 11/23/24.
//

#pragma once

#ifndef ENGINE_SUPPORTED
    #error view engine must be supported... include engine.h instead of its constituient parts!
#endif

/*
 * An engine view must have the following at the minimum:
 *  requires readable_engine for const view types
 *  else requires writable engine for view types
 * {
 *  private type aliases (required for engine view): 
 *      using engine_pointer = ...
 * 
 *  public type aliases (required for general engine): 
 *      using orientation_type = ...
 *      using data_type = ...
 *      using index_type = ...
 *      using reference = ...
 *      using const_reference = ...
 *      // using mdspan_type = ...      // not implemented yet
 *      // using const_mdspan_type = ...    // not implemented yet
 * 
 *  public type aliases (required for engine view):
 *       using engine_type = ...
 *       using owning_engine_type = ...
 *       
 *  public methods (required for general engine):
 *      default destructor
 *      default copy, move constructors
 *      default copy, move assignment
 *      (rule of none)
 * 
 *      atleast one element access operator 
 *      (for mutable engine view, returns reference, immutable engine view returns const_reference)
 * 
 *      rows and cols
 * 
 *      A swap specialization
 * 
 *  public methods (required for engine view):
 *      default constructor noexcept, initializes engine_ptr to nullptr
 *      engine_type constructor: intializes engine_ptr to address of supplied engine_type reference
 *      has_view: returns whether engine points to a valid engine
 * 
 *  private data (required for engine view):
 *      engine_ptr ptr;
 *      
 * }
 */
//template<typename Egn, typename Vw>
//struct engine_view;
template<typename Vw, typename Egn, typename ...>
struct engine_view;

/*
 * import and export views (mutable vs not mutable)
 * 
 *      any mutable view or composition of mutable views
 *      are considered to be an import view
 * 
 *      any immutable view or composition of immutable views
 *      are considered an export view
 * 
 *      an export view can be virtually expanded, however import
 *      views cannot.
 */
struct export_views
{
    struct transparent {};
    struct negation {};
    struct conjugate {};
    struct transpose {};
    struct hermitian {};
    struct banded {};
    struct row {};
    struct col {};
    struct box {};
};

struct inport_views
{
    struct transparent {};
    struct transpose {};
    struct banded {};
    struct row {};
    struct col {};
    struct box {};
};

/*
 * view engine basic requirements:
 *     - Must at the least satisfy readable engine type
 *     - Must not own data, meaning it has an owning engine type alias
 *     - It must be noexcept swappable with others of the same view type and owning engine type
 */
template<typename VEgn>
concept view_basics = 
    readable_engine<VEgn> and
    std::is_nothrow_swappable_v<VEgn&> and
    not owning_engine<VEgn>;

template<typename X>
concept has_immutable_view_ref =
    std::same_as<typename X::reference, typename X::data_type> or
    std::same_as<typename X::reference, typename X::const_reference>;

template<typename VEgn>
concept mutable_view = 
    view_basics<VEgn> and
    writable_engine<VEgn>
    and not has_immutable_view_ref<VEgn>;

template<typename VEgn>
concept immutable_view = 
    view_basics<VEgn> and
    has_immutable_view_ref<VEgn>;

template<typename Egn>
concept exportable = 
    immutable_view<Egn> or 
    (readable_engine<Egn> and owning_engine<Egn>);

template<typename Egn>
concept inportable = 
    mutable_view<Egn> or
    (writable_engine<Egn> and owning_engine<Egn>);

#include "view_funs.h"

#include "view_engines/transparent_view.h"
#include "view_engines/negation_view.h"
#include "view_engines/conjugate_view.h"
#include "view_engines/transpose_view.h"
#include "view_engines/row_view.h"
#include "view_engines/col_view.h"
#include "view_engines/box_view.h"

template<template<typename...> typename VwTmA, typename VwA,
         template<typename...> typename VwTmB, typename VwB,
         typename ComEgn>
requires
    immutable_view<VwTmA<VwA, ComEgn>> and
    immutable_view<VwTmB<VwB, ComEgn>> and
    exportable<ComEgn>
struct basic_view_commutator
{
private:
    using common_engine_type = ComEgn;
    using data_type = typename ComEgn::data_type;
    using index_type = typename ComEgn::index_type;

public:
    /*
     * NOTE:
     *
     *  [?] https://en.wikipedia.org/wiki/Commutator
     * 
     * The commutator below satisfies the definition of
     * a commutator [1]:
     * 
     * [g, h] = g^-1 * h^-1 * g * h
     * 
     * Right now, the anti commutator does not strictly follow
     * the definition of the generally defined anti commutator.
     * 
     */
    static constexpr auto do_commute(common_engine_type const& com)
    -> std::pair<uint16_t, uint16_t>
    {
        using Atype = VwTmA<VwA, ComEgn>;
        using Btype = VwTmB<VwB, ComEgn>;

        uint16_t commutator = 0;
        uint16_t anti_commutator = 0;

        uint16_t shift_c = 0;
        uint16_t shift_ac = 0;

        VwTmA<VwA, Btype> a = VwTmA<VwA, Btype>(Btype(com));
        VwTmB<VwB, Atype> b = VwTmB<VwB, Atype>(Atype(com));

        auto accum = [&shift_c, &commutator](bool pred) mutable 
            { commutator |= ((pred ? 0b0 : 0b1) << shift_c++);};

        auto anti_accum = [&shift_ac, &anti_commutator](bool pred) mutable 
            { anti_commutator |= ((pred ? 0b0 : 0b1) << shift_ac++);};

        accum(a.rows() == b.rows());
        accum(a.cols() == b.cols());
        accum(a.size() == b.size());

        if(engine_helper::compare1D_exact(a, b))
        {
            accum(true);
        }
        else
        {
            for(; shift_c < 16; ++shift_c)
            {
                accum(false);
            }
        }

        anti_accum(a.rows() == b.cols());
        anti_accum(a.cols() == b.rows());
        anti_accum(a.rows() == b.size());
        anti_accum(a.cols() == b.size());
        anti_accum(a.size() == b.rows());
        anti_accum(a.size() == b.cols());

        return std::make_pair(commutator + 1, anti_commutator + 1);
    }
};

struct product_views
{
    struct inner {};
    struct outer {};
    struct scalar {};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * inner product view:
 *     If TLHS and TRHS are both exportable and either TLHS is explicitly a row vector type
 *     or TRHS is explicitly a col vector type then the 'inner' tag specializes product_view under 
 *     the following additional constraints:
 *          - TLHS cannot satisfy colvec_dimensions
 *          - TRHS cannot satisfy rowvec_dimensions
 *          - The element type of TLHS and TRHS must share a common type
 *          - IF compatible dimensions can be gaurenteed at compile time, then incompatible
 *            dimensions dont pass constraints. If not, then a runtime error must be generated
 *            upon construction of the product view if the dimensions are incompatible.
 * 
 * local storage:
 *      - TLHS engine pointer
 *      - TRHS engine pointer
 * IF dimensions are totally guarenteed at compile time
 *      - THEN a std::array with resulting dimensions. (idk if I like this, it feels like this should also be std::vector)
 * IF dimensions are totally guarenteed at CT and indicate a scalar result type
 *      - THEN data_type to store scalar result
 * ELSE:
 *      - A std::vector which grows as needed to store the reductions as
 *        the indexing access operator is called.
 *     
 * The inner product view should itself satisfy the exportable constraint inorder to
 * be considered an engine view. However, this view may own data (depending on the logic in the local storage section) 
 * although this data should be considered to have a limited lifetime given that this is a view type.
 * and thus may define the quasi_owner_type, indicating that the type owns indexable data which may be subject to deallocation.
 *
 * This view MUST satisfy (row/col)vec_dimensions concept for TLHS/TRHS being the explicit vector type
 * respectively
 * 
 * IF TLHS is explicit row vector type:
 * 
 * rows() { return 1 }
 * cols() { return rhs->cols() }
 * 
 * ELIF TRHS is explicit col vector type:
 * 
 * rows() { return lhs->rows() }
 * cols() { return 1 }
 * 
 * NOTE: y <- <x|A is the same as y^T <- (A^T)|x>
 *      In other words, the inner product operation is symmetric under the
 *      transpose operation, and COULD be exploited to reduce code size,
 *      but I am not entirely convinced that the preformance benefit of
 *      reduced code size in this instance outweighs the preformance cost
 *      of constructing extra views to handle the more general scenario.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * inner product functional operator:
 *      All of the constraints which apply to the inner product as a view
 *      also apply to the functional operator. The only difference here is
 *      that in the case where the inner product is in a functional pipeline,
 *      then it is important that the operator consumes lhs and rhs and produces
 *      the expected product dimensions depending on the explicit vector type:
 *              
 * IF TLHS is an explicit row vector type:
 *      process<Ins<TLHS, TRHS>, Outs< // satisfies rowvec_dimensions // > inner_product
 * 
 * IF TRHS is an explicit col vector type:
 *      process<Ins<TLHS, TRHS>, Outs< // satisfies colvec_dimensions // > inner_product
 * 
 * In the case of a functional operator, then we compute either a rowwise or colwise 
 * inner product at each step, so unlike the view type (where random access is assumed)
 * we do not have to store any internal state (which it ideally shouldnt anyways since its
 * a functional operator)
 * 
 */

template<typename TLHS, typename TRHS, bool SizeGuard = true>
requires
    exportable<TLHS> and 
    exportable<TRHS> and
    std::convertible_to<typename TLHS::data_type, typename TRHS::data_type> and
    rowvec_dimensions<TLHS> and
    colvec_dimensions<TRHS>
static constexpr auto inner_product(TLHS const& lhs, TRHS const& rhs)
-> typename TLHS::data_type
{
    using lhs_it = typename TLHS::index_type;
    using rhs_it = typename TRHS::index_type;

    using dtype = typename TLHS::data_type;

    lhs_it j = 0;
    rhs_it i = 0;
    dtype sum = 0;

    /*
       specifying the size guard adds a call to min to find
       the stop index in the case where lhs and rhs were not 
       previously checked for their size. Note that by default,
       this branch is taken, and the caller must explicitly disable the 
       size guard in the case where they're certain that
       the two have the right dimensions.
    */
    if constexpr(SizeGuard)
    {
        using it = std::common_type_t<lhs_it, rhs_it, std::size_t>;

        it k = 0;
        it stop = std::min(static_cast<it>(lhs.cols()), 
                           static_cast<it>(rhs.rows()));

        for(; k < stop; i++, j++)
        {
            sum += lhs(0, j) * static_cast<dtype>(rhs(i, 0));
        }
    }
    else
    {
        for(; j < lhs.cols(); i++, j++)
        {
            sum += lhs(0, j) * static_cast<dtype>(rhs(i, 0));
        }
    }

    return sum;
}

template<typename TLHS, typename TRHS>
requires
    exportable<TLHS> and 
    exportable<TRHS> and
    std::convertible_to<typename TLHS::data_type, typename TRHS::data_type> and
    rowvec_dimensions<TLHS> and (not rowvec_dimensions<TRHS>)
struct engine_view<product_views::inner, TLHS, TRHS>
{

public:
    using owning_engine_type = typename has_owning_engine_type_alias<TLHS>::owning_engine_type;
    using engine_type = TLHS;

    static constexpr bool quasi_owner_type = true;

private:
    using lhs_pointer = engine_type const*;
    using rhs_pointer = TRHS const*;

    using rhs_sz_extract = engine_ct_extents<TRHS>;
    using rhs_col_view_type = engine_view<export_views::col, TRHS>;

public:

    using orientation_type = typename get_engine_orientation<engine_type>::type;
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::data_type;
    using const_reference = typename engine_type::data_type;

private:

    lhs_pointer m_lhs_eng_ptr;
    rhs_pointer m_rhs_eng_ptr;

    std::vector<data_type> m_data;
    std::vector<bool> m_inner_computed;

public:

    constexpr engine_view() noexcept
    : m_lhs_eng_ptr(nullptr), m_rhs_eng_ptr(nullptr), m_data(), m_inner_computed()
    {}

    explicit
    constexpr engine_view(TLHS const& lhs, TRHS const& rhs)
    : m_lhs_eng_ptr(&lhs), m_rhs_eng_ptr(&rhs), m_data(rhs.cols()), m_inner_computed(rhs.cols())
    {
        if(lhs.cols() != rhs.rows())
        {
            throw std::runtime_error("incompatible dimensions for inner product.");
        }
    }

    constexpr bool has_view() const
    {
        return (m_lhs_eng_ptr != nullptr) && (m_rhs_eng_ptr != nullptr);
    }

public:

    constexpr index_type rows() const noexcept
    {
        return 1;
    }

    constexpr index_type cols() const noexcept
    {
        if constexpr(rhs_sz_extract::is_constexpr_cols())
        {
            return rhs_sz_extract::cols();
        }
        else
        {
            return m_data.size();
        }
    }

    constexpr index_type size() const noexcept
    {
        return cols();
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    {
        return (*this)(j);
    }

    constexpr const_reference operator()(index_type j) const
    {
        if(!m_inner_computed[j])
        {
            m_data[j] = inner_product<TLHS, rhs_col_view_type, false>
            (
                *m_lhs_eng_ptr,
                rhs_col_view_type(*m_rhs_eng_ptr, j)
            );

            m_inner_computed[j] = true;
        }

        return m_data[j];
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * outer product view:
 *      IF TLHS and TRHS are both exportable and TLHS is explicitly a col vector, and
 *      TRHS is explicitly a row vector, then the 'outer' tag specializes product view under
 *      the following additional constraints:
 *          - IF compatible dimensions can be gaurenteed at compile time, then incompatible
 *            dimensions dont pass constraints. If not, then a runtime error must be generated
 *            upon construction of the product view if the dimensions are incompatible.
 *
 * local storage:
 *      - TLHS engine pointer
 *      - TRHS engine pointer
 * 
 * rows() { return lhs->rows() }
 * cols() { return rhs->cols() }
 * operator(i, j) { return lhs(i, 0) * rhs(0, j) }
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * virtual expansion:
 *      a view engine which extends the indexing range of a view via
 *      some rule for representing the "virtual elements" which are
 *      actually outside the physical range of the owning engine's dimensions.
 * 
 *      mutable views cannot be virtually resized. 
 * 
 *      virtual expansion can only increase the percieved size, not decrease it 
 */
/*
template<typename OP, typename ...Args>
concept valid_expander = 
    std::invocable<OP, Args...> and
    base_types<OP> and
    requires(OP && op, Args && ...args)
    {
        typename OP::index_type;
        typename OP::const_reference;
        typename OP::engine_type;

        {std::invoke(std::forward<OP>(op), std::forward<Args>(args)...)}
                -> std::same_as<typename OP::const_reference>;
    };

template<typename VEgn, typename OP>
concept expandable = 
    exportable<VEgn> or
    valid_expander<OP, VEgn const&, typename VEgn::index_type, typename VEgn::index_type> and
    std::same_as<VEgn, typename OP::engine_type> and
    (
        std::same_as<typename OP::const_reference, typename VEgn::const_reference> or
        std::same_as<typename OP::const_reference, typename VEgn::data_type>
    );
*/
/*
 * view expander ctor type I (static, no OP requires no initialization)
 * 
 * We would like to strive for OPs to have static RC methods and a static access operator
 * General idea is that rows/cols and access operator take ctor_type as its first argument.
 *  - rows/cols will have one param being the ctor type
 *  - access operator needs ctor type and i, and j
 *  - transpose, conjugate, negation, row/col view can all be fully static
 *  - In this case with no OP constructor, expand view must inherit rows/cols from its own
 *    constructor, preferred that rows/cols get pulled from the constructing engine
 * 
 * The following template overload (by constraints) only accepts OPs which have only
 * static callables
 */
/*
template<typename VEgn, template<typename...> typename TmOP, typename ...Args>
requires 
    expandable<VEgn, TmOP<VEgn, Args...>> and
    static_dimensions<TmOP<VEgn, Args...>, typename VEgn::index_type, VEgn const&> and
    static_evaluation<TmOP<VEgn, Args...>, VEgn const&, typename VEgn::index_type, typename VEgn::const_reference>
struct expand_view
{
public:
    using engine_type = VEgn;

public:
    using owning_engine_type = typename has_owning_engine_type_alias<VEgn>::owning_engine_type;
    // Egn must have these by writable_engine (which requires readable_engine) requirement:
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;

private:

    using pointer_type = engine_type const*;
    using op_type = TmOP<VEgn, Args...>;
    using ctor_type = engine_type const&;
    using op_fun = std::function<const_reference(ctor_type, index_type, index_type)>;
    using rc_fun = std::function<index_type(ctor_type)>;

    static constexpr bool op_is_rc_constructible = 
        std::constructible_from<op_type, index_type, index_type>;

private:
    pointer_type m_eng_ptr;

    op_fun op;

    rc_fun rows_fun;
    rc_fun cols_fun;

public:

    constexpr expand_view() noexcept
    : m_eng_ptr(nullptr)
    {
        bind_rc_funs();
        bind_op_fun();  
    }

    explicit
    constexpr expand_view
    (
        engine_type const& rhs
    )
    : m_eng_ptr(&rhs)
    {
        bind_rc_funs();
        bind_op_fun();
    }

    constexpr bool has_view() const
    {
        return m_eng_ptr != nullptr;
    }

public:

    
    // required for readable_engine (consistant_return_lengths):
    //      rows() -> index_type
    constexpr index_type rows() const noexcept
    {
        return rows_fun(*m_eng_ptr);
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      cols() -> index_type
    constexpr index_type cols() const noexcept
    {
        return cols_fun(*m_eng_ptr);
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        return rows_fun() * cols_fun();
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    {
        return op(*m_eng_ptr, i, j);
    }

    constexpr const_reference operator()(index_type i) const
    {
        return (*m_eng_ptr)(i);
    }

    constexpr void swap(expand_view & rhs) noexcept
    {
        engine_helper::swap(rhs, *this);
    }

private:

    constexpr void bind_op_fun()
    {
        op = std::bind
        (
            &op_type::operator(),
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
        );
    }

    constexpr void bind_rc_funs()
    {
        rows_fun = std::bind(&op_type::rows, *m_eng_ptr);
        cols_fun = std::bind(&op_type::cols, *m_eng_ptr);
    }
};
*/

/*
 * view expander ctor type II (op_type is a parameter)
 *
 * If the OP needs to be constructed, we have two options:
 *  1. The constructor takes an additional parameter of type op_type which is the constructed
 *     object. It is bound to op, and rc_funs. 
 *          - op gets three placeholders in the binding process
 *          - rc funs each get one
 */




/*
 * an import view and an export view come together with a function object
 * to form an operation:
 * 
 * template<typename LVw, typename ExVw, typename OP>
 * 
 * A LVw can be an import or export view. ExVw must be a export view:
 * 
 *      template<typename ImVw, typename ExVw, typename OP>
 *          The result of preforming OP(ImVw x, ExVw y) -> x
 * 
 *      template<typename ExVw, typename ExVw, typename OP>
 *          The result of preforming OP(ExVw x, ExVw y) is 
 *          another export view, which stores the intermediate 
 *          action of OP, ExVw and ExVw on that element
 * 
 *      template<typename ExVw, typename ImVw, typename OP> 
 *          is invalid.
 * 
 * IF ExVw views an engine whose elements are function objects, then the element
 * type of ImVw must be valid inputs to those objects. This OP will be named ExApply, and the
 * return value of the ExVw function objects must be the type of ImVw
 * 
 * IF the element types of the Vws are both arithmetic, the OP will be an arithmetic
 * operation on the two. Type promotions will need to occur here. 
 * 
 * If the two Vws are the same the percieved dimension (either the two owning engines have the same shape
 * or the ExVws are virtually expanded)
 */


/*
 * view composer root template
 * 
 * Any view composition must satisfy this requirement
 * If there is a mutable view, it must be on the LHS, i.e. VEgnY
 */ 
/*
template<typename VEgnY, typename VEgnX, typename OP>
requires 
    (immutable_view<VEgnY> or mutable_view<VEgnY>) and
    immutable_view<VEgnX>
struct view_composer;*/

/*
 * view composer partial specialization
 * 
 * enabled if VEgnX's elements are invocable with
 * VEgnY's element type
 */ 
/*
template<typename VEgnY, typename VEgnX>
requires
    std::invocable<typename VEgnX::data_type, typename VEgnY::data_type>
struct view_composer<VEgnY, VEgnX, void>
{};*/

/*
 * Dual view engine:
 *      two view engines (satisfy view engine basics): V1, V2
 * 
 *      they are not required to have the same owning engine, as 
 *      long as both are both readable (satisfied above)
 * 
 *      virtual resize rule(s) can be passed to modify percieved sizes
 * 
 */

/*
template<typename Egn, typename Vw, typename... Vws>
struct view_combiner
{
    using type = view_combiner<engine_view<Egn, Vw>, Vws...>::type;
    using constructor = engine_view<Egn, Vw>(typename type::constructor);
};

template<typename Egn, typename Vw>
struct view_combiner<Egn, Vw>
{
    using type = engine_view<Egn, Vw>;
};*/
