//
//  inner_product_view.h
//  Created by Ben Westcott on 12/20/24.
//

#pragma once

template<typename TLHS, typename TRHS, typename RT, bool SizeGuard = true>
requires
    exportable<TLHS> and 
    exportable<TRHS> and
    valid_binary_star_operator<RT> and
    std::convertible_to<typename TLHS::data_type, RT> and
    std::convertible_to<typename TRHS::data_type, RT> and
    vec_type<TLHS> and 
    vec_type<TRHS>
static constexpr auto inner_product(TLHS const& lhs, TRHS const& rhs)
-> RT
{
    using lhs_it = typename TLHS::index_type;
    using rhs_it = typename TRHS::index_type;

    using dtype = RT;

    lhs_it j = 0;
    rhs_it i = 0;
    dtype sum = 0;

    
    /*
       specifying the size guard adds a call to min to find
       the stop index in the case where lhs and rhs were not 
       previously checked for their size. Note that by default,
       this branch is taken, and the caller must explicitly disable the 
       size guard in the case where they're certain that
       the two have the right dimensions.
    */
   
    if constexpr(SizeGuard)
    {
        using it = std::common_type_t<lhs_it, rhs_it, std::size_t>;

        it k = 0;
        it stop = std::min(static_cast<it>(lhs.cols()), 
                           static_cast<it>(rhs.rows()));
        
        for(; k < stop; ++k, ++i, ++j)
        {
            sum += static_cast<RT>(lhs(j)) * static_cast<RT>(rhs(i));
        }
    }
    else
    {
        for(; j < lhs.cols(); ++i, ++j)
        {
            sum += static_cast<RT>(lhs(j)) * static_cast<RT>(rhs(i));
        }
    }

    return sum;


}

template<typename TLHS, typename TRHS, typename RT, bool SizeGuard = true>
requires
    exportable<TLHS> and
    exportable<TRHS> and
    engine_invocable_with<RT, TLHS, TRHS> and
    vec_type<TLHS> and
    vec_type<TRHS>
static constexpr auto inner_product(TLHS const& lhs, TRHS const& rhs)
-> RT
{
    using lhs_it = typename TLHS::index_type;
    using rhs_it = typename TRHS::index_type;

    using dtype = RT;

    lhs_it j = 0;
    rhs_it i = 0;
    dtype sum = 0;

    if constexpr(SizeGuard)
    {
        using it = std::common_type_t<lhs_it, rhs_it, std::size_t>;

        it k = 0;
        it stop = std::min(static_cast<it>(lhs.size()), 
                           static_cast<it>(rhs.size()));
        
        for(; k < stop; ++k, ++i, ++j)
        {
            sum += static_cast<RT>(lhs(j)(rhs(i)));
        }
    }
    else
    {
        for(; j < lhs.size(); ++i, ++j)
        {
            sum += static_cast<RT>(lhs(j)(rhs(i)));
        }
    }    

    return sum;
}


template<typename TLHS, typename TRHS>
concept inner_product_view_basics =
    exportable<TLHS> and
    exportable<TRHS> and
    rowvec_dimension<TLHS>;

template<typename TLHS, typename TRHS>
requires
    inner_product_view_basics<TLHS, TRHS> and
    //common_data_types<TLHS, TRHS> and
    valid_product_view_traits<TLHS, TRHS> and
    nonvec_dimension<TRHS>
struct engine_view<product_views::inner, TLHS, TRHS>
{

public:
    using lhs_owning_engine_type = typename has_owning_engine_type_alias<TLHS>::owning_engine_type;
    using rhs_owning_engine_type = typename has_owning_engine_type_alias<TRHS>::owning_engine_type;

    using lhs_engine_type = TLHS;
    using rhs_engine_type = TRHS;

    static constexpr bool quasi_owner_type = true;

private:

    using ptraits = product_view_traits<TLHS, TRHS>;
    //using common_data_type = patched_common_type<typename TLHS::data_type, typename TRHS::data_type>;
    using common_index_type = std::common_type<typename TLHS::index_type, 
                                               typename TRHS::index_type, std::size_t>;

    using lhs_pointer = TLHS const*;
    using rhs_pointer = TRHS const*;

    using rhs_sz_extract = engine_ct_extents<TRHS>;
    using rhs_col_view_type = engine_view<export_views::col, TRHS>;

public:

    using orientation_type = matrix_orientation::row_major;
    using index_type = typename common_index_type::type;

    using data_type = typename ptraits::data_type;
    using reference = typename ptraits::data_type;
    using const_reference = typename ptraits::data_type;

private:

    lhs_pointer m_lhs_eng_ptr;
    rhs_pointer m_rhs_eng_ptr;

    std::vector<data_type> m_data;
    std::vector<bool> m_inner_computed;

public:

    constexpr engine_view() noexcept
    : m_lhs_eng_ptr(nullptr), m_rhs_eng_ptr(nullptr), m_data(), m_inner_computed()
    {}

    explicit
    constexpr engine_view(TLHS const& lhs, TRHS const& rhs)
    : m_lhs_eng_ptr(&lhs), m_rhs_eng_ptr(&rhs), m_data(rhs.cols()), m_inner_computed(rhs.cols())
    {
        if(lhs.cols() != rhs.rows())
        {
            throw std::runtime_error("incompatible dimensions for inner product.");
        }
    }

    constexpr bool has_view() const
    {
        return (m_lhs_eng_ptr != nullptr) && (m_rhs_eng_ptr != nullptr);
    }

public:

    constexpr index_type rows() const noexcept
    {
        return 1;
    }

    constexpr index_type cols() const noexcept
    {
        if constexpr(rhs_sz_extract::is_constexpr_cols())
        {
            return rhs_sz_extract::cols();
        }
        else
        {
            return m_data.size();
        }
    }

    constexpr index_type size() const noexcept
    {
        return cols();
    }

    constexpr const_reference operator()(index_type i, index_type j)
    {
        return (*this)(j);
    }

    constexpr const_reference operator()(index_type j)
    {
        if(!m_inner_computed[j])
        {
            m_data[j] = inner_product<TLHS, rhs_col_view_type, data_type, false>
            (
                *m_lhs_eng_ptr,
                rhs_col_view_type(*m_rhs_eng_ptr, j)
            );

            m_inner_computed[j] = true;
        }

        return m_data[j];
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }
};

template<typename TLHS, typename TRHS>
requires
    inner_product_view_basics<TLHS, TRHS> and
    //common_data_types<TLHS, TRHS> and
    product_view_traits<TLHS, TRHS>::value and
    colvec_dimension<TRHS>
struct engine_view<product_views::inner, TLHS, TRHS>
{
public:
    using lhs_owning_engine_type = typename has_owning_engine_type_alias<TLHS>::owning_engine_type;
    using rhs_owning_engine_type = typename has_owning_engine_type_alias<TRHS>::owning_engine_type;
    
    using lhs_engine_type = TLHS;
    using rhs_engine_type = TRHS;

    //static constexpr bool quasi_owner_type = false;

private:

    //using common_data_type = patched_common_type<typename TLHS::data_type, typename TRHS::data_type>;
    using ptraits = product_view_traits<TLHS, TRHS>;
    using common_index_type = std::common_type<typename TLHS::index_type, typename TRHS::index_type, std::size_t>;

    using lhs_pointer = TLHS const*;
    using rhs_pointer = TRHS const*;

    using rhs_sz_extract = engine_ct_extents<TRHS>;
    using rhs_col_view_type = engine_view<export_views::col, TRHS>;

public:

    using orientation_type = matrix_orientation::row_major;
    using index_type = typename common_index_type::type;

    using data_type = typename ptraits::data_type;
    using reference = typename ptraits::data_type;
    using const_reference = typename ptraits::data_type;

private:

    lhs_pointer m_lhs_eng_ptr;
    rhs_pointer m_rhs_eng_ptr;

    data_type m_data;

public:

    constexpr engine_view() noexcept
    : m_lhs_eng_ptr(nullptr), m_rhs_eng_ptr(nullptr), m_data(0)
    {}

    explicit
    constexpr engine_view(TLHS const& lhs, TRHS const& rhs)
    : m_lhs_eng_ptr(&lhs), m_rhs_eng_ptr(&rhs), m_data(0)
    {
        if(lhs.cols() != rhs.rows())
        {
            throw std::runtime_error("incompatible dimensions for inner product.");
        }

        m_data = inner_product<TLHS, TRHS, data_type, false>(*m_lhs_eng_ptr, *m_rhs_eng_ptr);
    }

    constexpr bool has_view() const
    {
        return (m_lhs_eng_ptr != nullptr) && (m_rhs_eng_ptr != nullptr);
    }

public:

    constexpr index_type rows() const noexcept
    {
        return 1;
    }

    constexpr index_type cols() const noexcept
    {
        if constexpr(rhs_sz_extract::is_constexpr_cols())
        {
            return rhs_sz_extract::cols();
        }
        else
        {
            return m_data.size();
        }
    }

    constexpr index_type size() const noexcept
    {
        return cols();
    }

    constexpr const_reference operator()(index_type i, index_type j)
    {
        return (*this)(j);
    }

    constexpr const_reference operator()(index_type j)
    {
        return m_data;
    }

    constexpr void swap(engine_view & rhs) noexcept
    {
        engine_helper::swap(*this, rhs);
    }  
};