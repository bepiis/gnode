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
 *      points_to: returns engine which it points to
 *      is_pointed: returns whether engine points to a valid engine
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
    struct const_hermitian {};

    struct banded {};
    struct const_banded {};

    struct transpose {};
    struct const_transpose {};

    struct row {};
    struct const_row {};
    struct col {};
    struct const_col {};

    struct block {};
    struct const_block {};
};

#include "transparent_view_engine.h"