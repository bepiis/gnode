//
//  outer_product_view.h
//  Created by Ben Westcott on 12/13/24.
//

#pragma once

template<typename TLHS, typename TRHS>
concept outer_product_view_basics = 
    exportable<TLHS> and
    exportable<TRHS> and
    colvec_type<TLHS> and
    rowvec_type<TRHS>;

template<typename TLHS, typename TRHS>
requires
    outer_product_view_basics<TLHS, TRHS> and
    valid_product_view_traits<TLHS, TRHS>
struct engine_view<product_views::outer, TLHS, TRHS>
{

public:
    using lhs_owning_engine_type = typename has_owning_engine_type_alias<TLHS>::owning_engine_type;
    using rhs_owning_engine_type = typename has_owning_engine_type_alias<TRHS>::owning_engine_type;

    using lhs_engine_type = TLHS;
    using rhs_engine_type = TRHS;

private:
    using ptraits = product_view_traits<TLHS, TRHS>;
    using common_index_type = std::common_type<typename TLHS::index_type, 
                                               typename TRHS::index_type, std::size_t>;

    using lhs_pointer = TLHS const*;
    using rhs_pointer = TRHS const*;

    using lhs_sz_extract = engine_ct_extents<TLHS>;
    using rhs_sz_extract = engine_ct_extents<TRHS>;

public:

    // TODO: ???
    using orientation_type = matrix_orientation::row_major;
    using index_type = typename common_index_type::type;
    
    using data_type = typename ptraits::data_type;

    // TODO: loosen restrictions on readable views so that
    //       reference and const reference dont need to be
    //       specified since we dont really need them unless
    //       we specifically need a const reference/reference
    //       to the owning engine data.
    using reference = typename ptraits::data_type;
    using const_reference = typename ptraits::data_type;

private:

    lhs_pointer m_lhs_eng_ptr;
    rhs_pointer m_rhs_eng_ptr;

    friend struct view_tree;

public:

    constexpr engine_view() noexcept
    : m_lhs_eng_ptr(nullptr), m_rhs_eng_ptr(nullptr)
    {}

    explicit
    constexpr engine_view(TLHS const& lhs, TRHS const& rhs)
    : m_lhs_eng_ptr(&lhs), m_rhs_eng_ptr(&rhs)
    {}

    constexpr bool has_view() const
    {
        return (m_lhs_eng_ptr != nullptr) && (m_rhs_eng_ptr != nullptr);
    }

public:

    constexpr index_type rows() const noexcept
    {
        if constexpr(lhs_sz_extract::is_constexpr_rows())
        {
            return lhs_sz_extract::rows();
        }
        else
        {
            return m_lhs_eng_ptr->rows();
        }
    }

    constexpr index_type cols() const noexcept
    {
        if constexpr(rhs_sz_extract::is_constexpr_cols())
        {
            return rhs_sz_extract::cols();
        }
        else
        {
            return m_rhs_eng_ptr->cols();
        }
    }

    const index_type size() const noexcept
    {
        return rows() * cols();
    }

    constexpr const_reference operator()(index_type i, index_type j)
    requires
        engine_invocable_with<data_type, TLHS, TRHS>
    {
        return (*m_lhs_eng_ptr)(i)((*m_rhs_eng_ptr)(j));
    }

    constexpr const_reference operator()(index_type i, index_type j)
    requires
        engine_invocable_with<data_type, TRHS, TLHS>
    {
        return (*m_rhs_eng_ptr)(j)((*m_lhs_eng_ptr)(i));
    }

    constexpr const_reference operator()(index_type i, index_type j)
    requires
        common_data_types<TLHS, TRHS>
    {
        return static_cast<data_type>((*m_lhs_eng_ptr)(i)) * 
               static_cast<data_type>((*m_rhs_eng_ptr)(j));
    }

    constexpr void swap(engine_view & rhs)
    {
        engine_helper::swap(*this, rhs);
    }
};
