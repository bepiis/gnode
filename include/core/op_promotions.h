//
//  op_promotions.h
//  Created by Ben Westcott on 12/13/24.
//

/*
 * Allocator promotion machinery
 *
 * There are 3 options for the type Alloc:
 *      1. std::allocator
 *      2. a custom allocator which satisfies base_allocator_interface
 *      3. void (in which case template dimensions must be fixed and gt 0)
 * 
 * <std::allocator, std::allocator> promotes to std::allocator
 * <custom_alloc, std::allocator> promotes to custom alloc via rebind alloc
 * //TODO: finish 
 */

