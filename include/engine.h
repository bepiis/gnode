//
//  engine_helper.h
//  Created by Ben Westcott on 11/9/24.
//

#pragma once

#include <cstdint>
#include <mdspan>
//#include <type_traits>
#include <concepts>
#include <algorithm>

template<typename T>
using rect_init_list = std::initializer_list<std::initializer_list<T>>;

template<typename T>
using init_list = std::initializer_list<T>;

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


template<typename From, typename To>
struct static_value_caster
{
    To operator()(From f) const
    {
        return static_cast<To>(f);
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

        constexpr bool rows_eq = static_cast<com>(r1) == static_cast<com>(r2);
        constexpr bool cols_eq = static_cast<com>(c1) == static_cast<com>(c2);

        return rows_eq && cols_eq;
    }

    template<typename R1, typename C1, typename R2, typename C2>
    static constexpr bool sizes_compatible(R1 r1, C1 c1, R2 r2, C2 c2)
    {
        using com = std::common_type_t<R1, C1, R2, C2, size_t>;

        constexpr com size1 = static_cast<com>(r1) * static_cast<com>(c1);
        constexpr com size2 = static_cast<com>(r2) * static_cast<com>(c2);

        return size1 == size2;
    }

    template<typename X>
    static constexpr void validate_length(X x)
    {
        //return x >= static_cast<X>(0);
        if(x >= static_cast<X>(0))
        {
            throw std::runtime_error("invalid length.");
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

    template<typename X>
    static constexpr void validate_room(X x)
    {
        //return x > static_cast<X>(0);
        if(x > static_cast<X>(0))
        {
            throw std::runtime_error("invalid room.");
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
    static constexpr void swap(T t1, T t2) noexcept
    {
        if(&t1 != &t2)
        {
            T tmp = std::move(t1);
            t1 = std::move(t2);
            t2 = std::move(tmp);
        }
    }

    template<class Egn, typename R, typename C>
    static constexpr void validate(Egn & eng, R nbr_rows, C nbr_cols);

    template<class Egn, typename X>
    static constexpr void validate(Egn & eng, X size);

    template<class EgnX, class EgnY>
    static constexpr void copy2(EgnX const& src, EgnY & dst)
    requires
        std::convertible_to<typename EgnX::data_type, typename EgnY::data_type>
        // should also require EgnX is atleast a readable type
        // and EgnY is atleast a writable type
    {
        using src_index_type = EgnX::index_type;
        
        using dst_index_type = EgnY::index_type;
        using dst_data_type = typename EgnX::data_type;
        
        src_index_type nbr_src_rows = src.rows();
        src_index_type nbr_src_cols = src.cols();
        
        // TODO: validate
    
        src_index_type src_idx, src_jdx;
        dst_index_type dst_idx, dst_jdx;

        for(src_idx = dst_idx = 0; src_idx < nbr_src_rows; src_idx++, dst_idx++)
        {
            for(src_jdx = dst_jdx = 0; src_jdx < nbr_src_cols; src_jdx++, dst_jdx++)
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
        std::convertible_to<T, typename Egn::data_type>
        // should also require that Egn be atleast writable
    {
        using index_type = typename Egn::index_type;

        validate_init_list(src);

        auto nbr_rows = src.size();
        auto nbr_cols = src.begin()->size();

        // TODO: validate 

        for(auto r : src)
        {
            
        }




    }
    // TODO: mdspan copy2
    
    template<class Egn, typename R1, typename R2, typename C1, typename C2>
    static constexpr void move_data(Egn & src, Egn & dst, R1 ilb_inc, R2 iub_ex, C1 jlb_inc, C2 jub_ex)
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
    static constexpr void move_elements(Egn & src, Egn & dst, R nbr_rows, C nbr_cols)
    {
        move_elements<Egn, Egn, R, R, C, C>(src, dst, 0, nbr_rows, 0, nbr_cols);
    }

    template<class Egn, typename C1, typename C2, typename T>
    static constexpr void fill_cols(Egn & dst, C1 jlb_inc, C2 jub_ex, T const& value)
    requires
        std::convertible_to<T, typename Egn::data_type>
        // should also require that Egn be atleast writable
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
        std::convertible_to<T, typename Egn::data_type>
        // should also require that Egn be atleast writable       
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

    using extents_type = std::extents<index_type, std::dynamic_extent, std::dynamic_extent>;
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
    using self_type = matrix_storage_engine;
    using orient = matrix_orientation;

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

    constexpr matrix_storage_engine(index_type nbr_rows, index_type nbr_cols);
    constexpr matrix_storage_engine(index_type nbr_rows, index_type nbr_cols, index_type row_reach, index_type col_reach);
    
    //constexpr matrix_storage_engine(index_type nbr_rows);
    //constexpr matrix_storage_engine(index_type nbr_rows, index_type row_reach);
    //constexpr matrix_storage_engine(index_type nbr_cols);
    //constexpr matrix_storage_engine(index_type nbr_cols, index_type col_reach);

    constexpr matrix_storage_engine(index_type size);
    //constexpr matrix_storage_engine(index_type size, index_type room);

    template<typename R>
    constexpr matrix_storage_engine(rect_init_list<R> lst);

    template<typename R>
    constexpr matrix_storage_engine & operator=(rect_init_list<R> lst);

    template<typename Egn>
    constexpr matrix_storage_engine(Egn const& other);

    template<typename Egn>
    constexpr matrix_storage_engine & operator=(Egn const& other);

    //template<typename R, typename IT, size_t X, size_t Y, typename LP, typename AP>
    //constexpr matrix_storage_engine(std::mdspan<R, std::extents<IT, X, Y>, LP, AP> const& other);

    //template<typename R, typename IT, size_t X, size_t Y, typename LP, typename AP>
    //constexpr matrix_storage_engine & operator=(std::mdspan<R, std::extents<IT, X, Y>, LP, AP> const& other);

    //template<typename Cntr>
    //constexpr matrix_storage_engine(Cntr const& other);

    //template<typename Cntr>
    //constexpr matrix_storage_engine & operator=(Cntr const& other);
    

    constexpr index_type rows(void) const noexcept
    {
        return m_data.m_rows;
    }
    
    constexpr index_type row_reach(void) const noexcept
    {
        return m_data.m_row_reach;
    }

    constexpr index_type cols(void) const noexcept
    {
        return m_data.m_cols;
    }
    
    constexpr index_type col_reach(void) const noexcept
    {
        return m_data.m_col_reach;
    }

    constexpr index_type size(void) const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }
    
    constexpr index_type room(void) const noexcept
    {
        return m_data.m_row_reach * m_data.m_col_reach;
    }
    
    constexpr reference operator()(index_type i)
    {
        return m_data.m_data[i];
    }
    
    constexpr const_reference operator()(index_type i) const
    {
        return m_data.m_data[i];
    }
    
    constexpr reference operator()(index_type i, index_type j)
    requires self_type::is_row_major
    {
        return m_data.m_data[orient::offset(i, j, m_data.m_col_reach)];
    }
    
    constexpr const_reference operator()(index_type i, index_type j) const
    requires self_type::is_row_major
    {
        return m_data.m_data[orient::offset(i, j, m_data.m_col_reach)];
    }
    
    constexpr reference operator()(index_type i, index_type j)
    requires self_type::is_col_major
    {
        return m_data.m_data[orient::offset(j, i, m_data.m_row_reach)];
    }
    
    constexpr const_reference operator()(index_type i, index_type j) const
    requires self_type::is_col_major
    {
        return m_data.m_data[orient::offset(j, i, m_data.m_row_reach)];
    }
    
    
private:

    constexpr void reshape(index_type nbr_rows, index_type row_reach, index_type nbr_cols, index_type col_reach);
    constexpr void reshape_rows(index_type nbr_rows, index_type row_reach);
    constexpr void reshape_cols(index_type nbr_cols, index_type col_reach);
    



};




