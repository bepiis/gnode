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

    struct block {};
    struct const_block {};
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

#include "transparent_view_engine.h"
#include "const_transparent_view_engine.h"
#include "const_negation_view_engine.h"
#include "const_conjugate_view_engine.h"
#include "transpose_view_engine.h"
#include "const_transpose_view_engine.h"

// matrix view engine types will already satisfy this requirement by design,
// but view expression itself must satisfy its own requirement, being
// view expressible, meaning it must satisfy the readable engine
// requirements and cannot own any data.
// The V prepending Egn denotes that the type can be included in expressions
// with view engine types, which this concept enforces.
template<typename VEgn>
concept view_expressible = readable_engine<VEgn> and not owning_engine<VEgn>;

template<typename EgnX, typename EgnY>
concept valid_view_expression = 
    view_expressible<EgnX> and 
    view_expressible<EgnY> and
    same_owning_engine<EgnX, EgnY>;

template<typename VEgnLHS, typename VEgnRHS>
requires
    valid_view_expression<VEgnLHS, VEgnRHS>
struct view_expression
{
private:

    // owning engine types must be the same
    using owning_engine_type = typename VEgnLHS::owning_engine_type;

    // same owning engine requirement guarentees that owning engine type alias is defined
    using owning_reference = typename owning_engine_type::reference;
    using owning_const_reference = typename owning_engine_type::const_reference;

    static constexpr bool left_ref_is_owning_ref = std::same_as<typename VEgnLHS::reference, owning_reference>;
    static constexpr bool right_ref_is_owning_ref = std::same_as<typename VEgnRHS::reference, owning_reference>;
    static constexpr bool ref_inherits_owning_ref = left_ref_is_owning_ref and right_ref_is_owning_ref;

    static constexpr bool left_ref_is_owning_data_type = std::same_as<typename VEgnLHS::reference, typename VEgnLHS::data_type>;
    static constexpr bool right_ref_is_owning_data_type = std::same_as<typename VEgnRHS::reference, typename VEgnLHS::data_type>;

    static constexpr bool ref_inherits_data_type = left_ref_is_owning_data_type or right_ref_is_owning_data_type;

public:

    // WARNING: matrix::orientation::none is not handled...
    // expression will always inherit RHS orientation 
    using orientation_type = typename get_engine_orientation<VEgnRHS>::type;

    using data_type = typename VEgnLHS::data_type;
    using index_type = std::common_type<typename VEgnLHS::index_type, typename VEgnRHS::index_type, size_t>;
    
    // since if const ref is data type, then ref will inherit data type also
    // see reference alias below.
    using const_reference = std::conditional_t<ref_inherits_data_type, 
                                               data_type, 
                                               owning_const_reference>;
                                               
    // if left AND right ref are owning refs, then ref is owning ref
    // else if left OR right ref is an owning data type then ref is owning data type
    // else ref is an owning const reference
    using reference = std::conditional_t<ref_inherits_owning_ref, 
                                         owning_reference,
                                         const_reference>;

    

};



