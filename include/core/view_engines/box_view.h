//
//  box_view.h
//  Created by Ben Westcott on 12/1/24.
//

template<typename Egn>
requires
    inportable<Egn>
struct engine_view<inport_views::box, Egn>
{

/* view engine public type alias requirements */
public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;

private:
    using pointer_type = engine_type*;
    using ctor_type = engine_type &;

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
    
    index_type start_row;
    index_type nbr_rows;
    index_type start_col;
    index_type nbr_cols;
    
/* view engine public method requirements */
public:
    
    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr), start_row(0), nbr_rows(0), start_col(0), nbr_cols(0)
    {}

    explicit
    constexpr engine_view(engine_type & rhs)
    : m_eng_ptr(&rhs), start_row(0), nbr_rows(rhs.rows()), start_col(0), nbr_cols(rhs.cols())
    {}
    
    explicit
    constexpr engine_view
    (
        engine_type & rhs,
        index_type sr,
        index_type nr,
        index_type cs,
        index_type nc
    ) //$ [NG]
    : m_eng_ptr(&rhs), start_row(sr), nbr_rows(nr), start_col(cs), nbr_cols(nc)
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
        return nbr_rows;
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      cols() -> index_type
    constexpr index_type cols() const noexcept
    {
        return nbr_cols;
    }
    
    // required for readable_engine (consistant_return_lengths):
    //      size() -> index_type
    constexpr index_type size() const noexcept
    {
        return nbr_rows * nbr_cols;
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i, j) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i, j) -> reference
    constexpr reference operator()(index_type i, index_type j) const
    {
        return (*m_eng_ptr)(start_row + i, start_col + j);
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i) -> reference or const_reference or data_type
    // required for writable_engine (mutable_access):
    //      (*this)(i) -> reference
    constexpr reference operator()(index_type i) const
    {
        return (*m_eng_ptr)(i);
    }
    
    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};

