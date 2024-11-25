//
//  storage_engine.h
//  Created by Ben Westcott on 11/9/24.
//

#pragma once

#ifndef ENGINE_SUPPORTED
    #error engine must be supported... include engine.h instead of its constituient parts!
#endif

template<typename T, class Alloc, std::size_t R, std::size_t C, typename L>
struct matrix_data
{
    using storage_type = std::vector<T, Alloc>;
    using index_type = std::size_t;
    // using mdspan_type = ...
    // using const_mdspan_type = ...
    
    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;
    static constexpr bool is_static_row_vector = (R == 1);
    static constexpr bool is_static_col_vector = (C == 1);
    static constexpr bool is_row_dynamic = false;
    static constexpr bool is_col_dynamic = false;

    static constexpr index_type m_rows = R;
    static constexpr index_type m_row_reach = R;

    static constexpr index_type m_cols = C;
    static constexpr index_type m_col_reach = C;

    storage_type m_data;

    constexpr matrix_data(void)
    : m_data(R * C)
    {}
};

template<typename T, std::size_t R, std::size_t C, typename L>
struct matrix_data<T, void, R, C, L>
{
    using storage_type = std::array<T, R * C>;
    using index_type = std::size_t;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;
    static constexpr bool is_static_row_vector = (R == 1);
    static constexpr bool is_static_col_vector = (C == 1);
    static constexpr bool is_row_dynamic = false;
    static constexpr bool is_col_dynamic = false;

    static constexpr index_type m_rows = R;
    static constexpr index_type m_row_reach = R;

    static constexpr index_type m_cols = C;
    static constexpr index_type m_col_reach = C;

    storage_type m_data;

    constexpr matrix_data(void)
    : m_data()
    {}
};

template<typename T, class Alloc, std::size_t R, typename L>
struct matrix_data<T, Alloc, R, std::dynamic_extent, L>
{
    using storage_type = std::vector<T, Alloc>;
    using index_type = std::size_t;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;
    static constexpr bool is_static_row_vector = (R == 1);
    static constexpr bool is_static_col_vector = false;
    static constexpr bool is_row_dynamic = false;
    static constexpr bool is_col_dynamic = true;

    static constexpr index_type m_rows = R;
    static constexpr index_type m_row_reach = R;

    storage_type m_data;

    index_type m_cols;
    index_type m_col_reach;

    constexpr matrix_data(void)
    : m_data(), m_cols(0), m_col_reach(0)
    {}

    constexpr matrix_data(matrix_data const& other) = default;
    constexpr matrix_data(matrix_data && other) = default;

    constexpr matrix_data & operator=(matrix_data const& other) = default;
    constexpr matrix_data & operator=(matrix_data && other) = default;
};

template<typename T, class Alloc, std::size_t C, typename L>
struct matrix_data<T, Alloc, std::dynamic_extent, C, L>
{
    using storage_type = std::vector<T, Alloc>;
    using index_type = std::size_t;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;
    static constexpr bool is_static_row_vector = false;
    static constexpr bool is_static_col_vector = (C == 1);
    static constexpr bool is_row_dynamic = true;
    static constexpr bool is_col_dynamic = false;

    static constexpr index_type m_cols = C;
    static constexpr index_type m_col_reach = C;

    storage_type m_data;

    index_type m_rows;
    index_type m_row_reach;

    constexpr matrix_data(void)
    : m_data(), m_rows(0), m_row_reach(0)
    {}
};


template<typename T, class Alloc, typename L>
struct matrix_data<T, Alloc, std::dynamic_extent, std::dynamic_extent, L>
{
    using storage_type = std::vector<T, Alloc>;
    using index_type = std::size_t;

    using extents_type = std::extents<index_type, std::dynamic_extent, std::dynamic_extent>;
    using stride_type = std::layout_left;

    using mdspan_type = typename std::mdspan<T, extents_type, stride_type>;
    using const_mdspan_type = typename std::mdspan<T const, extents_type, stride_type>;

    static constexpr bool is_row_major = std::is_same_v<L, matrix_orientation::row_major_t>;
    static constexpr bool is_col_major = std::is_same_v<L, matrix_orientation::col_major_t>;
    static constexpr bool is_static_row_vector = false;
    static constexpr bool is_static_col_vector = false;
    static constexpr bool is_row_dynamic = true;
    static constexpr bool is_col_dynamic = true;

    storage_type m_data;

    index_type m_rows;
    index_type m_row_reach;

    index_type m_cols;
    index_type m_col_reach;

    constexpr matrix_data(void)
    : m_data(), m_rows(0), m_row_reach(0), m_cols(0), m_col_reach(0) 
    {}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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
template<typename T, class Alloc, std::size_t R, std::size_t C, typename L>
struct matrix_storage_engine
{

private:

    using storage_type = matrix_data<T, Alloc, R, C, L>;
    using self_type = matrix_storage_engine;
    using orient = matrix_orientation;
    using helper = engine_helper;

    template<typename U>
    using literal2D = std::initializer_list<std::initializer_list<U>>;

    template<typename U>
    using literal1D = std::initializer_list<U>;

    storage_type m_data;


public:

    static constexpr bool is_row_major = storage_type::is_row_major;
    static constexpr bool is_col_major = storage_type::is_col_major;
    static constexpr bool is_static_row_vector = storage_type::is_static_row_vector;
    static constexpr bool is_static_col_vector = storage_type::is_static_col_vector;
    static constexpr bool is_row_dynamic = storage_type::is_row_dynamic;
    static constexpr bool is_col_dynamic = storage_type::is_col_dynamic;
    
    static constexpr bool is_fully_dynamic = is_row_dynamic and is_col_dynamic;
    static constexpr bool is_fully_static = (not is_row_dynamic) and (not is_col_dynamic);


    using allocator_type = Alloc;

    // required valid orientation: row_major_t or col_major_t
    using orientation_type = L;

    // required for readability:
    using data_type = T;
    using index_type = std::size_t;
    using reference = T &;
    using const_reference= T const&;

    //using mdspan_type = typename matrix_data<T, Alloc, L>::mdspan_type;
    //using const_mdspan_type = typename matrix_data<T, Alloc, L>::mdspan_type;

    constexpr matrix_storage_engine(void) = default;
    constexpr matrix_storage_engine(matrix_storage_engine const& other) = default;
    constexpr matrix_storage_engine(matrix_storage_engine && other) = default;

    constexpr matrix_storage_engine & operator=(matrix_storage_engine const& other) = default;
    constexpr matrix_storage_engine & operator=(matrix_storage_engine && other) = default;

    constexpr matrix_storage_engine(index_type nbr_rows, index_type nbr_cols)
    requires 
        self_type::is_fully_dynamic
    : m_data()
    {
        //std::cout << "called nbr_rows, nbr_cols constructor (requires is_fully_dynamic).\n";
        __reshape(nbr_rows, nbr_rows, nbr_cols, nbr_cols);
    }
    constexpr matrix_storage_engine(index_type nbr_rows, index_type row_reach, index_type nbr_cols, index_type col_reach)
    requires 
        self_type::is_fully_dynamic
    : m_data()
    {
        //std::cout << "called nbr_rows, row_reach, nbr_cols, col_reach constructor (requires is_fully_dynamic).\n";
        __reshape(nbr_rows, row_reach, nbr_cols, col_reach);
    }

    constexpr matrix_storage_engine(index_type nbr_rows, index_type row_reach)
    requires 
        self_type::is_row_dynamic and (not self_type::is_col_dynamic)
    : m_data()
    {
        //std::cout << "called nbr_cols, col_reach constructor (requires is_row_dynamic).\n";
        __reshape_rows(nbr_rows, row_reach);
    }

    constexpr matrix_storage_engine(index_type nbr_cols, index_type col_reach)
    requires 
        self_type::is_col_dynamic and (not self_type::is_row_dynamic)
    : m_data()
    {
        //std::cout << "called nbr_cols, col_reach constructor (requires is_col_dynamic).\n";
        __reshape_cols(nbr_cols, col_reach);
    }
    
    //constexpr matrix_storage_engine(index_type nbr_rows);
    //constexpr matrix_storage_engine(index_type nbr_rows, index_type row_reach);
    //constexpr matrix_storage_engine(index_type nbr_cols);
    //constexpr matrix_storage_engine(index_type nbr_cols, index_type col_reach);

    //constexpr matrix_storage_engine(index_type size);
    //constexpr matrix_storage_engine(index_type size, index_type room);

    template<typename Egn>
    constexpr matrix_storage_engine(Egn const& other)
    requires
        std::convertible_to<typename Egn::data_type, data_type>
    : m_data()
    {
        //std::cout << "called copy constructor.\n";
        helper::copy2(other, *this);
    }

    template<typename Egn>
    constexpr matrix_storage_engine & operator=(Egn const& other)
    requires
        std::convertible_to<typename Egn::data_type, data_type>
    {
        //std::cout << "called copy assignment operator.\n";
        helper::copy2(other, *this);
        return *this;
    }

    template<typename U>
    constexpr matrix_storage_engine(literal2D<U> lst)
    requires
        std::convertible_to<U, data_type>
    : m_data()
    {
        //std::cout << "called rect init list constructor.\n";
        helper::copy2(lst, *this);
    }

    template<typename U>
    constexpr matrix_storage_engine & operator=(literal2D<U> lst)
    requires
        std::convertible_to<U, data_type>
    {
        //std::cout << "called rect init list assignment operator.\n";
        helper::copy2(lst, *this);
        return *this;
    }

    //template<typename R, typename IT, size_t X, size_t Y, typename LP, typename AP>
    //constexpr matrix_storage_engine(std::mdspan<R, std::extents<IT, X, Y>, LP, AP> const& other);

    //template<typename R, typename IT, size_t X, size_t Y, typename LP, typename AP>
    //constexpr matrix_storage_engine & operator=(std::mdspan<R, std::extents<IT, X, Y>, LP, AP> const& other);

    //template<typename Cntr>
    //constexpr matrix_storage_engine(Cntr const& other);

    //template<typename Cntr>
    //constexpr matrix_storage_engine & operator=(Cntr const& other);

    // required for readability: rows -> index_type
    //$ [PAFC]
    constexpr index_type rows(void) const noexcept
    {
        return m_data.m_rows;
    }
    
    // required for readability: row_reach -> index_type
    //$ [PAFC]
    constexpr index_type row_reach(void) const noexcept
    {
        return m_data.m_row_reach;
    }

    // required for readability: cols -> index_type
    //$ [PAFC]
    constexpr index_type cols(void) const noexcept
    {
        return m_data.m_cols;
    }
    
    // required for readability: col_reach -> index_type
    //$ [PAFC]
    constexpr index_type col_reach(void) const noexcept
    {
        return m_data.m_col_reach;
    }

    // required for readability: size -> index_type
    //$ [PAFC]
    constexpr index_type size(void) const noexcept
    {
        return m_data.m_rows * m_data.m_cols;
    }
    
    // required for readability: reach -> index_type
    //$ [PAFC]
    constexpr index_type reach(void) const noexcept
    {
        return m_data.m_row_reach * m_data.m_col_reach;
    }
    
    // required for mutable access: eng(i) -> Egn::reference
    constexpr reference operator()(index_type i)
    {
        return m_data.m_data[i];
    }

    // required for immutable access: eng(i) -> Egn::const_reference
    constexpr const_reference operator()(index_type i) const
    {
        return m_data.m_data[i];
    }
    
    // atleast one of the following two
    // required for mutable access: eng(i, j) -> Egn::reference
    constexpr reference operator()(index_type i, index_type j)
    requires self_type::is_row_major
    {
        //std::cout << "called reference 2D operator.\n";
        return m_data.m_data[orient::offset(i, j, m_data.m_col_reach)];
    }

    constexpr reference operator()(index_type i, index_type j)
    requires self_type::is_col_major
    {
        return m_data.m_data[orient::offset(j, i, m_data.m_row_reach)];
    }
    
    // atleast one of the following two
    // required for immutable access: eng(i, j) -> Egn::const_reference
    constexpr const_reference operator()(index_type i, index_type j) const
    requires self_type::is_row_major
    {
        //std::cout << "called const_reference 2D operator.\n";
        return m_data.m_data[orient::offset(i, j, m_data.m_col_reach)];
    }
    
    constexpr const_reference operator()(index_type i, index_type j) const
    requires self_type::is_col_major
    {
        return m_data.m_data[orient::offset(j, i, m_data.m_row_reach)];
    }

    //constexpr mdspan_type span() noexcept
    //constexpr const_mdspan_type span() const noexcept
    
    // required for general reshapeability
    constexpr void reshape(index_type nbr_rows, index_type row_reach, index_type nbr_cols, index_type col_reach)
    requires self_type::is_fully_dynamic
    {
        __reshape(nbr_rows, row_reach, nbr_cols, col_reach);
    }

    // required for column reshapeability
    /*
    constexpr void reshape_cols(index_type nbr_cols, index_type col_reach)
    requires self_type::is_fully_dynamic
    {
        __reshape(m_data.m_rows, m_data.row_reach, nbr_cols, col_reach);
    }*/

    constexpr void reshape_cols(index_type nbr_cols, index_type col_reach)
    requires self_type::is_col_dynamic
    {
        __reshape_cols(nbr_cols, col_reach);
    }

    // required for row reshapeability
    /*
    constexpr void reshape_rows(index_type nbr_rows, index_type row_reach)
    requires self_type::is_fully_dynamic
    {
        __reshape(nbr_rows, row_reach, m_data.m_rows, m_data.m_row_reach);
    }*/

    constexpr void reshape_rows(index_type nbr_rows, index_type row_reach)
    requires self_type::is_row_dynamic
    {
        __reshape_rows(nbr_rows, row_reach);
    }

    // required for swapability: Egn::swap(Egn & other)
    constexpr void swap(self_type & other) noexcept
    {
        helper::swap(m_data, other.m_data);
    }

    
private:

    constexpr void __reshape(index_type nbr_rows, index_type row_reach, index_type nbr_cols, index_type col_reach)
    requires self_type::is_fully_dynamic
    {
        helper::validate_length(nbr_rows);
        helper::validate_length(nbr_cols);
        helper::validate_reach(row_reach);
        helper::validate_reach(col_reach);

        //std::cout << "here" << "\n";
        
        if(nbr_rows > m_data.m_row_reach || row_reach != m_data.m_row_reach ||
           nbr_cols > m_data.m_col_reach || col_reach != m_data.m_col_reach)
        {
            // ensure that the rowreach is >= nbr_rows, and colreach >= nbr_cols
            row_reach = std::max(nbr_rows, row_reach);
            col_reach = std::max(nbr_cols, col_reach);

            //std::cout << "here again" << nbr_rows << "\t" << nbr_cols << "\n";

            self_type to;
            to.m_data.m_data.resize(row_reach * col_reach);
            to.m_data.m_rows = nbr_rows;
            to.m_data.m_cols = nbr_cols;
            to.m_data.m_row_reach = row_reach;
            to.m_data.m_col_reach = col_reach;

            //std::cout << "here again" << to.m_data.m_rows << "\t" << to.m_data.m_cols << "\n";

            helper::move_data
            (
                *this,
                to,
                std::min(nbr_rows, m_data.m_rows),
                std::min(nbr_cols, m_data.m_cols)
            );

            helper::swap(m_data, to.m_data);

        }
        else
        {
            if(nbr_rows < m_data.m_rows)
            {
                helper::fill_rows(*this, nbr_rows, m_data.m_rows, T{});
            }
            else if(nbr_rows > m_data.m_rows)
            {
                helper::fill_rows(*this, m_data.m_rows, nbr_rows, T{});
            }

            if(nbr_cols < m_data.m_cols)
            {
                helper::fill_cols(*this, nbr_cols, m_data.m_cols, T{});
            }
            else if(nbr_cols > m_data.m_cols)
            {
                helper::fill_cols(*this, m_data.m_cols, nbr_cols, T{});
            }
            m_data.m_rows = nbr_rows;
            m_data.m_cols = nbr_cols;
        }

    }

    constexpr void __reshape_rows(index_type nbr_rows, index_type row_reach)
    requires self_type::is_row_dynamic
    {
        helper::validate_length(nbr_rows);
        helper::validate_reach(row_reach);

        if(nbr_rows > m_data.m_row_reach || row_reach != m_data.m_row_reach)
        {
            row_reach = std::max(nbr_rows, row_reach);

            self_type to;
            to.m_data.m_data.resize(row_reach * m_data.m_col_reach);
            to.m_data.m_rows = nbr_rows;
            to.m_data.m_row_reach = row_reach;

            helper::move_data(*this, to, std::min(nbr_rows, m_data.m_rows), m_data.m_cols);
            helper::swap(m_data, to.m_data);
        }
        else
        {
            if(nbr_rows < m_data.m_rows)
            {
                helper::fill_rows(*this, nbr_rows, m_data.m_rows, T{});
            }
            else if(nbr_rows > m_data.m_rows)
            {
                helper::fill_rows(*this, m_data.m_rows, nbr_rows, T{});
            }
            m_data.m_rows = nbr_rows;
        }
    }
    
    constexpr void __reshape_cols(index_type nbr_cols, index_type col_reach)
    requires self_type::is_col_dynamic
    {
        helper::validate_length(nbr_cols);
        helper::validate_reach(col_reach);

        if(nbr_cols > m_data.m_col_reach || col_reach != m_data.m_col_reach)
        {
            col_reach = std::max(nbr_cols, col_reach);

            self_type to;
            to.m_data.m_data.resize(col_reach * m_data.m_row_reach);
            to.m_data.m_cols = nbr_cols;
            to.m_data.m_col_reach = col_reach;

            helper::move_data(*this, to, m_data.m_rows, std::min(nbr_cols, m_data.m_cols));
            helper::swap(m_data, to.m_data);
        }
        else
        {
            if(nbr_cols < m_data.m_cols)
            {
                helper::fill_cols(*this, nbr_cols, m_data.m_cols, T{});
            }
            else if(nbr_cols > m_data.m_cols)
            {
                helper::fill_cols(*this, m_data.m_cols, nbr_cols, T{});
            }
            m_data.m_cols = nbr_cols;
        }
    }

    //template<typename ST?, typename MSE>
    //static constexpr ST make_mdspan(MSE & mse) noexcept
};




