//
//  const_negation_view_engine.h
//  Created by Ben Westcott on 11/25/24.
//

#pragma once

template<typename Egn>
requires
    exportable<Egn> and
    valid_unary_minus_operator<typename Egn::data_type>
struct engine_view<Egn, export_views::negation>
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
    
    // Egn must have these by readable_engine requirement:
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::data_type;
    using const_reference = typename engine_type::data_type;
    // using mdspan_type = ...
    // using const_mdspan_type = ...

/* view engine private data requirements */
private:
    pointer_type m_eng_ptr;

/* view engine public method requirements */
public:
    
    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr)
    {}
    
    explicit
    constexpr engine_view(engine_type const& rhs)
    : m_eng_ptr(&rhs)
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
        return m_eng_ptr->rows();
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
        return m_eng_ptr->size();
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i, j) -> reference or const_reference or data_type
    constexpr const_reference operator()(index_type i, index_type j) const
    {
        return vfuns::negation<Egn>::eval(*m_eng_ptr, i, j);
        //return -(*m_eng_ptr)(i, j);
    }
    
    // required or readable_engine (immutable_access):
    //      (*this)(i) -> reference or const_reference or data_type
    constexpr const_reference operator()(index_type i) const
    {
        return -(*m_eng_ptr)(i);
    }
    
    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};

