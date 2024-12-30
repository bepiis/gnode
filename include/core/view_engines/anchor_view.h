//
//  anchor_view.h
//  Created by Ben Westcott on 12/29/24.
//

struct anchors
{
    struct N {};
    struct S {};
    struct E {};
    struct W {};

    struct NW {};
    struct NE {};
    struct SW {};
    struct SE {};
};

template<typename An>
inline static constexpr bool is_2D_anchor = 
    std::same_as<anchors::NW, An> or
    std::same_as<anchors::NE, An> or
    std::same_as<anchors::SW, An> or
    std::same_as<anchors::SE, An>;

template<typename An>
inline static constexpr bool is_1D_anchor = 
    std::same_as<anchors::N, An> or
    std::same_as<anchors::S, An> or
    std::same_as<anchors::E, An> or
    std::same_as<anchors::W, An>;

template<typename An>
inline static constexpr bool is_west_bearing = 
    std::same_as<anchors::W, An> or
    std::same_as<anchors::NW, An> or
    std::same_as<anchors::SW, An>;

template<typename An>
inline static constexpr bool is_east_bearing = 
    std::same_as<anchors::E, An> or
    std::same_as<anchors::NE, An> or
    std::same_as<anchors::SE, An>;

template<typename An>
inline static constexpr bool is_north_bearing =
    std::same_as<anchors::N, An> or
    std::same_as<anchors::NE, An> or
    std::same_as<anchors::NW, An>;

template<typename An>
inline static constexpr bool is_south_bearing = 
    std::same_as<anchors::S, An> or
    std::same_as<anchors::SW, An> or
    std::same_as<anchors::SE, An>;

template<typename An>
inline static constexpr bool is_anchor = is_1D_anchor<An> or is_2D_anchor<An>;

template<typename Egn, typename An>
requires
    exportable<Egn> and
    is_anchor<An>
struct engine_view<export_views::anchor, Egn, An>
{
public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;

private:
    using pointer = engine_type const*;
    using ctor_type = engine_type const&;
    using anchor_type = An;
    using sz_extract = engine_ct_extents<engine_type>;

public:
    using orientation_type = typename get_engine_orientation<engine_type>::type;
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;

private:
    pointer m_eng_ptr;
    index_type imap;
    index_type jmap;

public:

    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr), imap(0), jmap(0)
    {}

    explicit
    constexpr engine_view(engine_type const& rhs)
    : m_eng_ptr(&rhs)
    {}

    constexpr index_type rows() const noexcept
    {
        if constexpr(sz_extract::is_constexpr_rows())
        {
            return sz_extract::rows();
        }
        else
        {
            return m_eng_ptr->rows();
        }
    }

    constexpr index_type cols() const noexcept
    {
        if constexpr(sz_extract::is_constexpr_cols())
        {
            return sz_extract::cols();
        }
        else
        {
            return m_eng_ptr->cols();
        }
    }

    constexpr index_type size() const noexcept
    {
        if constexpr(sz_extract::is_constexpr_size())
        {
            return sz_extract::size();
        }
        else
        {
            return m_eng_ptr->size();
        }
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    requires
        (not vec_type<Egn>)
    {
        if constexpr(std::same_as<anchors::NW, anchor_type>)
        {
            return (*m_eng_ptr)(i, j);
        }
        else if constexpr(std::same_as<anchors::NE, anchor_type>)
        {
            return (*m_eng_ptr)(i, cols() - 1 - j);
        }
        else if constexpr(std::same_as<anchors::SW, anchor_type>)
        {
            return (*m_eng_ptr)(rows() - 1 - i, j);
        }
        else if constexpr(std::same_as<anchors::SE, anchor_type>)
        {
            return (*m_eng_ptr)(rows() - 1 - i, cols() - 1 - j);
        }
        else
        {
            throw std::runtime_error("Invalid anchor type for 2D access operator.");
        }
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    requires
        rowvec_type<Egn>
    {
        return (*this)(j);
    }

    constexpr const_reference operator()(index_type i, index_type j) const
    requires
        colvec_type<Egn>
    {
        return (*this)(i);
    }

    constexpr const_reference operator()(index_type i) const
    requires
        vec_type<Egn>
    {
        if constexpr(rowvec_type<Egn>)
        {
            if constexpr(is_west_bearing<anchor_type>)
            {
                return (*m_eng_ptr)(i);
            }
            else if constexpr(is_east_bearing<anchor_type>)
            {
                return (*m_eng_ptr)(cols() - 1 - i);
            }
            else
            {
                throw std::runtime_error("Invalid anchor type for 1D row vec access operator.");
            }
        }
        else if constexpr(colvec_type<Egn>)
        {
            if constexpr(is_north_bearing<anchor_type>)
            {
                return (*m_eng_ptr)(i);
            }
            else if constexpr(is_south_bearing<anchor_type>)
            {
                return (*m_eng_ptr)(rows() - 1 - i);
            }
            else
            {
                throw std::runtime_error("Invalid anchor type for 1D col vec access operator.");
            }

        }
        else
        {
            static_assert("This shouldn't ever be reached.");
        }
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};