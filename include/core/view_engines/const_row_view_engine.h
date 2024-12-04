//
//  const_row_view_engine.h
//  Created by Ben Westcott on 11/30/24.
//

#pragma once

template<typename Egn>
requires 
    exportable<Egn>
struct engine_view<Egn, export_views::row>
{

/* view engine public type alias requirements */
public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;

private:
    using pointer_type = engine_type const*;
    using ctor_type = engine_type const&;

/* engine public type alias requirements */
public:

    // engine_type is not required to have a valid orientation type to be readable, so it may
    // not have one defined, thus we have to preform type detection to extract its orientation type
    // correctly. 
    using orientation_type = typename get_engine_orientation<Egn>::type;
    
    // Egn must have these by writable_engine (which requires readable_engine) requirement:
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

/* view engine private data requirements */
private:
    pointer_type m_eng_ptr;
    index_type m_row;

/* view engine public method requirements */
public:

    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr), m_row(0)
    {}

    explicit
    constexpr engine_view(engine_type const& rhs, index_type row = 0)
    : m_eng_ptr(&rhs), m_row(row)
    {}

    constexpr bool has_view() const
    {
        return m_eng_ptr != nullptr;
    }

/* engine public method requirements */
public:

    /* rule of zero */

    // required for readable_engine (consistant_return_lengths): 
    //      rows() -> index_type
    constexpr index_type rows() const noexcept
    {
        /*
        if constexpr(is_row_ct_v<engine_type>)
        {
            return engine_ct_extents<engine_type>::rows();
        }
        else
        {
            return m_eng_ptr->rows();
        }*/
        return 1;
    }

    // required for readable_engine (consistant_return_lengths): 
    //      cols() -> index_type
    constexpr index_type cols() const noexcept
    {
        /*
        if constexpr(is_col_ct_v<engine_type> and engine_ct_extents<engine_type>::cols() != 0)
        {
            return engine_ct_extents<engine_type>::cols();
        }
        else
        {
            return m_eng_ptr->cols();
        }*/
        return m_eng_ptr->cols();
    }

    // required for readable_engine (consistant_return_lengths): 
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        /*
        if constexpr(is_size_ct_v<engine_type>)
        {
            return engine_ct_extents<engine_type>::size();
        }
        else
        {
            return m_eng_ptr->size();
        }*/
        return cols();
    }

    // required or readable_engine (immutable_access):
    //      (*this)(i, j) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i, j) -> reference
    constexpr const_reference operator()(index_type i, index_type j) const
    {
        return (*m_eng_ptr)(m_row, j);
    }

    // required or readable_engine (immutable_access):
    //      (*this)(i) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i) -> reference
    constexpr const_reference operator()(index_type j) const
    {
        return (*m_eng_ptr)(m_row, j);
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};

