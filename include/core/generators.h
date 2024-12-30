//
//  generators.h
//  Created by Ben Westcott on 12/19/24.
//

struct generator_types
{
    struct basic {};
};

template<typename GenT, typename SeqT, std::size_t R, std::size_t C, typename...>
struct generator;


/*
 * GenT: generator type
 *      - power series
 *      - dirchlet
 *      - ...
 */

/*
 * GenT = power series:
 *
 * This type of generator takes the following form:
 *      a_n g_n(x)
 * Where a_n is a sequence, g_n(x) is of the form
 *      g_n(x) = g_n(x)' * x^n
 * 
 * Ordinary generator (OG):
 *      G(a_n ; x) = a_n x^n
 *          - g_n(x)' = 1
 *          - g_n(x) = x^n
 *
 * Exponential generator (EG):
 *      EG(a_n ; x) = a_n x^n/n!
 *          - g_n(x)' = 1/n!
 *          - g_n(x) = x^n/n!
 * 
 * Poisson generator (PG):
 *      PG(a_n ; x) = a_n exp(-x) x^n/n!
 *                  = exp(-x).EG(a_n ; x)
 * 
 * Lambert series generator (LG):
 *      LG(a_n ; x) = a_n . x^n/(1-x^n)
 * 
 */

/*
 * GenT basic:
 *  - Takes callable (thus data type)
 *  - Size and orientation
 *  - No allocator
 */
template<std::size_t R, std::size_t C>
struct gen_dimensions
{
    using index_type = std::size_t;

    static constexpr bool row_dynamic = false;
    static constexpr bool col_dynamic = false;

    static constexpr index_type m_rows = R;
    static constexpr index_type m_cols = C;

    constexpr gen_dimensions()
    {}
};

template<std::size_t R>
struct gen_dimensions<R, std::dynamic_extent>
{
    using index_type = std::size_t;


    static constexpr bool row_dynamic = false;
    static constexpr bool col_dynamic = true;

    static constexpr index_type m_rows = R;
    index_type m_cols;
    
    constexpr gen_dimensions()
    : m_cols(0) 
    {}
};

template<std::size_t C>
struct gen_dimensions<std::dynamic_extent, C>
{
    using index_type = std::size_t;

    static constexpr bool row_dynamic = true;
    static constexpr bool col_dynamic = false;

    index_type m_rows;
    static constexpr index_type m_cols = C;

    constexpr gen_dimensions()
    : m_rows(0)
    {}
};

template<>
struct gen_dimensions<std::dynamic_extent, std::dynamic_extent>
{
    using index_type = std::size_t;

    static constexpr bool row_dynamic = true;
    static constexpr bool col_dynamic = true;

    index_type m_rows;
    index_type m_cols;

    constexpr gen_dimensions()
    : m_rows(0), m_cols(0)
    {}
};


template<typename SeqT, std::size_t R, std::size_t C, typename L>
requires
    template_dimensions<R, C> and
    valid_storage_orientation<L> and
    std::invocable<SeqT, std::size_t, std::size_t>
struct generator<generator_types::basic, SeqT, R, C, L>
{

public:
    using orientation_type = L;
    using index_type = std::size_t;

private:
    using seq_return_type = std::invoke_result_t<SeqT, index_type, index_type>;
    using gdims_type = gen_dimensions<R, C>;

public:

    using data_type = std::function<seq_return_type()>;
    using reference = data_type;
    using const_reference = data_type;

    static constexpr bool is_row_dynamic = gdims_type::row_dynamic;
    static constexpr bool is_col_dynamic = gdims_type::col_dynamic;

    static constexpr bool is_row_major = std::same_as<L, matrix_orientation::row_major>;
    static constexpr bool is_col_major = std::same_as<L, matrix_orientation::col_major>;

private: 
    gdims_type m_dims;
    SeqT seq;

public:

    constexpr generator() = default;
    constexpr generator(generator const& other) = default;
    constexpr generator(generator && other) = default;

    constexpr generator & operator=(generator const& other) = default;
    constexpr generator & operator=(generator && other) = default;

    constexpr generator(index_type nbr_rows, index_type nbr_cols)
    requires
        is_row_dynamic and is_col_dynamic
    : m_dims()
    {
        engine_helper::validate_length(nbr_rows);
        engine_helper::validate_length(nbr_cols);

        m_dims.m_rows = nbr_rows;
        m_dims.m_cols = nbr_cols;
    }

    constexpr generator(index_type nbr_rows)
    requires
        is_row_dynamic and (not is_col_dynamic)
    : m_dims()
    {
        engine_helper::validate_length(nbr_rows);

        m_dims.m_rows = nbr_rows;
    }

    constexpr generator(index_type nbr_cols)
    requires
        (not is_row_dynamic) and is_col_dynamic
    : m_dims()
    {
        engine_helper::validate_length(nbr_cols);

        m_dims.m_cols = nbr_cols;
    }

    constexpr index_type rows() const noexcept
    {
        return m_dims.m_rows;
    }

    constexpr index_type cols() const noexcept
    {
        return m_dims.m_cols;
    }

    constexpr index_type size() const noexcept
    {
        return m_dims.m_rows * m_dims.m_cols;
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    requires
        is_row_major
    {
        return std::bind(seq, i, j);
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    requires
        is_col_major
    {
        return std::bind(seq, j, i);
    }

    constexpr void swap(generator & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }

    /* not sure about this yet
    constexpr const_reference operator()(index_type i) const
    {
        return engine_helper::access2D(*this, i);
    }*/
};




/*
template<typename SeqT, typename VEgn>
struct generator<generator_types::basic, SeqT, VEgn>
{

};
*/