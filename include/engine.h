//
//  engine_helper.h
//  Created by Ben Westcott on 11/9/24.
//

#pragma once

#include <cstdint>

template<typename T>
using rect_init_list = std::initializer_list<std::initializer_list<T>>;

template<typename T>
using init_list = std::initializer_list<T>>;

struct matrix_orientation
{
    struct row_major_t{};
    struct col_major_t{};
    struct none_t{};

    template<typename S>
    static constexpr S offset(S major, S minor, S minor_length)
    {
        return major + minor * minor_length;
    }
};

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * engine_helper:
 * 
 *      are_same_reach: returns whether two engines are the same size
 *                  - use common_type_t
 *      are_same_size
 * 
 *      is_valid_size: input parameter is non-negative
 *      is_valid_room: input parameter is greater than zero
 * 
 *      is_valid_initializer_list: returns whether supplied intializer list is valid
 *                  - i.e. {{1,2,3}, {4,5,6}} is valid
 *                  - but {{1,2,3}, {4,5}} is not
 * 
 *      have_compat_size: uses size_compare to verify if two engines have compatible sizes
 *      
 *      swap noexcept: swaps two things T1 and T2 via std::move
 * 
 *      verify_and_reshape: takes a destination, and source rows + cols and reshapes dst to fit source
 *          - checks if reshape need to occur (ie. dst rows & cols != src rows & cols)
 * 
 *      copy_to: takes an engine, and some other type and assigns the data of the other type to the engine
 *              - engine -> engine
 *              - mdspan -> engine
 *              - initializer_list (2D) -> engine
 *              - 1D random access container -> engine
 *       
 *      fill_(rows|cols) : fills r|c from pos a0 to a1
 * 
 *      move_elements: move elements from engine1 to engine2 starting at (0,0)??? to (n, m)
 * 
 *      is_exactly_equal: compares element-wise (after checking size_compare) whether an engine is exactly equal to another engine or related type
 *
 */
struct engine_helper
{
    using index_type = std::size_t;

    static constexpr bool is_valid_size(index_type x);
    static constexpr bool is_valid_room(index_type x);

    template<typename T>
    static constexpr bool is_valid_init_list(rect_init_list<T> lst);

    static constexpr bool sizes_equal(index_type x1, index_type x2);

    static constexpr bool sizes_equal(index_type r1, index_type c1, index_type r2, index_type c2);
    static constexpr bool sizes_compatible(index_type r1, index_type c1, index_type r2, index_type c2);

    template<typename T>
    static constexpr void swap(T t1, T t2) noexcept;

    template<class Egn>
    static constexpr void validate(Egn & eng, index_type nbr_rows, index_type nbr_cols);

    template<class Egn, typename X>
    static constexpr void validate(Egn & eng, X size);

    template<class EgnX, class EgnY>
    static constexpr void copy2(EgnX const& src, EgnY & dst);

    template<class CntrX, class EgnY>
    static constexpr void copy2(CntrX const& src, EgnY & dst);

    template<typename T, typename EgnY>
    static constexpr void copy2(rect_init_list<T> src, EgnY & dst);

    // TODO: mdspan copy2
};


/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * engine_data (template takes data type, allocator type, and data orientation):
 * 
 *      class variables: data, nbr_rows, nbr_cols, max_rows, max_cols
 * 
 *      bool is_(row|col)_major = is_same_v<orientation, layout::(row|col)_major>
 * 
 *      destructor, copy and move constructor, and copy and move assignment are default
 *
 * 
    std::mdspan<int, 
                  std::extents<size_t, std::dynamic_extent, std::dynamic_extent>, 
                  std::layout_stride> 
 *  
 * */

template<typename T, class Alloc, typename L>
struct matrix_data
{
    using storage_type = std::vector<T, Alloc>;
    using index_type = std::size_t;

    using extents_type = std::extents<index_type, std::dynamic_extent, std::dynamic_extent>
    using stride_type = std::layout_stride;

    using mdspan_type = typename std::mdspan<T, extents_type, stride_type>;
    using const_mdspan_type = typename std::mdspan<T const, extents_type, stride_type>;

    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;

    storage_type m_data;

    index_type m_rows;
    index_type m_row_reach;

    index_type m_cols;
    index_type m_col_reach;

    constexpr matrix_data(void)
    : m_data(), m_rows(0), m_row_reach(0), m_cols(0), m_col_reach(0) {}

    constexpr matrix_data(matrix_data const& other) = default;
    constexpr matrix_data(matrix_data && other) noexcept = default;

    ~matrix_data(void) = default;

    constexpr matrix_data & operator=(matrix_data const& other) = default;
    constexpr matrix_data & operator=(matrix_data && other) = default;
};

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * matrix_storage_engine (template takes data type, allocator type, and data orientation)
 * 
 *      class variables: engine_data  
 * 
 *      bool is_(row|col)_major     
 *      
 *      destructor, copy and move constructor, and copy and move assignment are default
 *      
 *      element access operators, associated mdspan object (maybe is a class variable?), reshape methods
 * 
 */
template<typename T, class Alloc, typename L>
struct matrix_storage_engine
{
    using storage_type = matrix_data<T, Alloc, L>;
    using index_type = std::size_t;

public:

    static constexpr bool is_row_major = storage_type::is_row_major;
    static constexpr bool is_col_major = storage_type::is_col_major;

    storage_type m_data;

    using data_type = T;
    using allocator_type = Alloc;
    using orientation_type = L;

    using reference = T &;
    using const_reference = T const&;
    using index_type = std::size_t;

    using mdspan_type = typename matrix_data<T, Alloc, L>::mdspan_type;
    using const_mdspan_type = typename matrix_data<T, Alloc, L>::mdspan_type;

    constexpr matrix_storage_engine(void) = default;
    constexpr matrix_storage_engine(matrix_storage_engine const& other) = default;
    constexpr matrix_storage_engine(matrix_storage_engine && other) = default;

    constexpr matrix_storage_engine & operator=(matrix_storage_engine const& other) = default;
    constexpr matrix_storage_engine & operator=(matrix_storage_engine && other) = default;
};




