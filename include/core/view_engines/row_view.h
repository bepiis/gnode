//
//  row_view.h
//  Created by Ben Westcott on 11/30/24.
//

#pragma once

template<typename Egn>
requires 
    inportable<Egn>
struct engine_view<Egn, inport_views::row>
{

/* view engine public type alias requirements */
public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;

private:
    using pointer_type = engine_type*;
    using ctor_type = engine_type &;

    using sz_extract = engine_ct_extents<engine_type>;

/* engine public type alias requirements */
public:

    // engine_type is not required to have a valid orientation type to be readable, so it may
    // not have one defined, thus we have to preform type detection to extract its orientation type
    // correctly. 
    using orientation_type = typename get_engine_orientation<Egn>::type;
    
    // Egn must have these by writable_engine (which requires readable_engine) requirement:
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::reference;
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
    constexpr engine_view(engine_type & rhs, index_type row = 0)
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
        return 1;
    }

    // required for readable_engine (consistant_return_lengths): 
    //      cols() -> index_type
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

    // required for readable_engine (consistant_return_lengths): 
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        return cols();
    }

    // required or readable_engine (immutable_access):
    //      (*this)(i, j) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i, j) -> reference
    constexpr reference operator()(index_type i, index_type j) const
    {
        return (*m_eng_ptr)(m_row, j);
    }

    // required or readable_engine (immutable_access):
    //      (*this)(i) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i) -> reference
    constexpr reference operator()(index_type j) const
    {
        return (*m_eng_ptr)(m_row, j);
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};

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

    using sz_extract = engine_ct_extents<engine_type>;


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
        return 1;
    }

    // required for readable_engine (consistant_return_lengths): 
    //      cols() -> index_type
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

    // required for readable_engine (consistant_return_lengths): 
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
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
