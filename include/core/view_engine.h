//
//  view_engine.h
//  Created by Ben Westcott on 11/23/24.
//

#pragma once

#ifndef ENGINE_SUPPORTED
    #error view engine must be supported... include engine.h instead of its constituient parts!
#endif

/*
 * A view engine must have the following at the minimum:
 *  requires readable_engine for const view types
 *  else requires writable engine for view types
 * {
 *  private type aliases (required for general engine):
 *      using self_type = ...
 *      using orient = matrix_orientation;
 *      using helper = engine_helper;
 *      // using mdhelper = ...     // not implemented yet
 * 
 *      // bool has_mdspan          // not implemented yet
 *  
 *  private type aliases (required for view engine): 
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
 *  public type aliases (required for view engine):
 *       using engine_type = ...
 *       using owning_engine_type = ...
 *       
 *  public methods (required for general engine):
 *      default destructor
 *      default copy, move constructors
 *      default copy, move assignment
 * 
 *      atleast one element access operator 
 *      (for mutable view engine, returns reference, immutable view engine returns const_reference)
 * 
 *      rows, row_reach, cols, col_reach, size and reach methods
 * 
 *      A swap specialization
 * 
 *  public methods (required for view engine):
 *      default constructor noexcept, initializes engine_ptr to nullptr
 *      engine_type constructor: intializes engine_ptr to address of supplied engine_type reference
 *      has_view: returns whether engine points to a valid engine
 * 
 *  private data (required for view engine):
 *      engine_ptr ptr;
 *      
 * }
 */

/*
 * view engine named requirements:
 *     - Must at the least satisfy readable engine 
 *     - Must not own data, meaning it has an owning engine type alias
 *     - It must be noexcept swappable with others of the same view type and owning engine type
 */

template<typename VEgn>
concept view_engine_basics = 
    readable_engine<VEgn> and
    std::is_nothrow_swappable_v<VEgn&> and
    not owning_engine<VEgn>;

template<typename VEgn>
concept mutable_view_engine = 
    view_engine_basics<VEgn> and
    writable_engine<VEgn>;

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
 * import and export views (mutable vs not mutable)
 * 
 *      any mutable view or composition of mutable views
 *      are considered to be an import view
 * 
 *      any immutable view of commpoisition of immutable views
 *      are considered an export view
 * 
 *      an export view can have virtually expanded, however import
 *      views cannot.
 * 
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
 * If the two Vws are the same dimensions, then 
 */



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

template<typename Egn, typename Vw>
struct matrix_view_engine;

// valid view types
struct matrix_view
{
    struct transparent {};
    struct const_transparent {};
    
    struct const_negation {};
    struct const_conjugate {};

    struct transpose {};
    struct const_transpose {};
    struct const_hermitian {};

    struct banded {};
    struct const_banded {};

    struct row {};
    struct const_row {};
    struct col {};
    struct const_col {};

    struct box {};
    struct const_box {};
};

template<typename Egn, typename Vw>
requires
    readable_engine<Egn>
struct view_lookup : public std::false_type
{
    using type = matrix_view::transparent;

    using data_type = typename Egn::data_type;
    using index_type = typename Egn::index_type;
    using reference = typename Egn::reference;
    using const_reference = typename Egn::const_reference;
    using pointer_type = Egn*;
    using rhs_type = Egn &;
    
    using result_type = reference;

    static constexpr result_type eval(data_type & d)
    {
        return d;
    }
};

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

template<typename Egn, typename Vw, typename... Vws>
struct view_combiner
{
    using type = view_combiner<matrix_view_engine<Egn, Vw>, Vws...>::type;
};

template<typename Egn, typename Vw>
struct view_combiner<Egn, Vw>
{
    using type = matrix_view_engine<Egn, Vw>;
};
