//
//  view_engine.h
//  Created by Ben Westcott on 11/23/24.
//

#pragma once

#ifndef ENGINE_SUPPORTED
    #error view engine must be supported... include engine.h instead of its constituient parts!
#endif

/*
 * A engine view must have the following at the minimum:
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
template<typename Egn, typename Vw>
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

template<typename VEgn>
concept has_immutable_view_ref =
    std::same_as<typename VEgn::reference, typename VEgn::data_type> or
    std::same_as<typename VEgn::reference, typename VEgn::const_reference>;

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

#include "view_engines/transparent_view_engine.h"
#include "view_engines/const_transparent_view_engine.h"
#include "view_engines/const_negation_view_engine.h"
#include "view_engines/const_conjugate_view_engine.h"
#include "view_engines/transpose_view_engine.h"
#include "view_engines/const_transpose_view_engine.h"

#include "view_engines/row_view_engine.h"
#include "view_engines/const_row_view_engine.h"
#include "view_engines/col_view_engine.h"
#include "view_engines/const_col_view_engine.h"
#include "view_engines/boxed_view_engine.h"
    
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
template<typename VEgn, typename OP, typename ...Args>
concept expandable = 
    immutable_view<VEgn> and
    std::invocable<OP, Args...> and
    requires(OP && op, Args && ...args)
    {
        {std::invoke(std::forward<OP>(op), std::forward<Args>(args)...)} 
            -> std::same_as<typename VEgn::const_reference>;
    };

template<typename VEgn, typename OP>
requires 
    expandable<VEgn, OP, VEgn const&, typename VEgn::index_type, typename VEgn::index_type>
struct expand_view
{

public:
    using engine_type = VEgn;

private:
    using pointer_type = engine_type const*;


public:
    using owning_engine_type = typename has_owning_engine_type_alias<VEgn>::owning_engine_type;
    // Egn must have these by writable_engine (which requires readable_engine) requirement:
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;

/* view engine private data requirements */
private:
    pointer_type m_eng_ptr;
    
    index_type nbr_rows;
    index_type nbr_cols;

    using fun_type = const_reference(engine_type const&, index_type, index_type);
    std::function<fun_type> op;
    
public:

    constexpr expand_view() noexcept
    : m_eng_ptr(nullptr), nbr_rows(0), nbr_cols(0),
      op([]() -> const_reference {return static_cast<const_reference>(0);})
    {}

    explicit 
    constexpr expand_view
    (
        engine_type const& rhs,
        index_type nr,
        index_type nc,
        OP fun
    ) //$ [NG]
    : m_eng_ptr(&rhs), nbr_rows(nr), nbr_cols(nc)
    {
        op = std::bind
        (
            &OP::operator(),
            fun,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3
        );
    }

    constexpr bool has_view() const
    {
        return m_eng_ptr != nullptr;
    }

public:

    /* rule of zero */
    
    // required for readable_engine (consistant_return_lengths):
    //      rows() -> index_type
    constexpr index_type rows() const noexcept
    {
        return nbr_rows;
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      cols() -> index_type
    constexpr index_type cols() const noexcept
    {
        return nbr_cols;
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        return nbr_rows * nbr_cols;
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    {
        //return std::invoke(&OP::operator(), op)(m_eng_ptr, i, j);
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

};

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
template<typename VEgnY, typename VEgnX, typename OP>
requires 
    (immutable_view<VEgnY> or mutable_view<VEgnY>) and
    immutable_view<VEgnX>
struct view_composer;

/*
 * view composer partial specialization
 * 
 * enabled if VEgnX's elements are invocable with
 * VEgnY's element type
 */ 
template<typename VEgnY, typename VEgnX>
requires
    std::invocable<typename VEgnX::data_type, typename VEgnY::data_type>
struct view_composer<VEgnY, VEgnX, void>
{};

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
    using type = view_combiner<matrix_view_engine<Egn, Vw>, Vws...>::type;
};

template<typename Egn, typename Vw>
struct view_combiner<Egn, Vw>
{
    using type = matrix_view_engine<Egn, Vw>;
};*/
