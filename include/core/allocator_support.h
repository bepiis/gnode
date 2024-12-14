//
//  allocator_support.h
//  Created by Ben Westcott on 12/13/24.
//

// Allocator named requirements:
// [1] https://en.cppreference.com/w/cpp/named_req/Allocator

/*
 * Named requirements state non-optional requirements are
 *      typename Alloc::value_type
 *      a.allocate(n)
 *      a.deallocate(p, n)
 *      { a1 == a2 } -> same_as<bool>
 *      { a1 != a2 } -> same_as<bool>
 *      { *p } -> same_as<T&>
 *      { *cp } -> same_as<T const&>
 *      is nothrow copyable (which implies it is nothrow movable?)
 *      { static_cast<A::pointer>(p) }
 *      { static_cast<A::const_pointer>(cp) }
 *      { std::pointer_traits<Alloc::pointer>::pointer_to(r) }
 */

/*
 * Taken from [1]: defines the base requirements for an allocator a
 */
template<typename Alloc>
concept base_allocator_interface = 
    requires(Alloc alloc, typename get_index_type<Alloc>::type n)
    {
        { *alloc.allocate(n) } -> std::same_as<typename Alloc::value_type&>;
        { alloc.deallocate(alloc.allocate(n), n) };
    } and
    std::copy_constructible<Alloc> and
    std::equality_comparable<Alloc>;

template<typename Alloc>
static inline constexpr bool allocator_type_is_void = std::same_as<Alloc, void>;