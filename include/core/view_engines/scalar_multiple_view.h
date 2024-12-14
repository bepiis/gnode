//
//  scalar_multiple_view.h
//  Created by Ben Westcott on 12/12/24.
//

#pragma once

template<typename Egn>
requires
    exportable<Egn>
struct engine_view<Egn, export_views::scalar_multiple>
{
public:
    using owning_engine_type = get_owning_engine_type<Egn>;
    using engine_type = Egn;

private:
    using pointer_type = engine_type const*;
    using ctor_type = engine_type const&;
    using sz_extract = engine_ct_extents<engine_type>;

public:
    using orientation_type = typename get_engine_orientation<Egn>::type;
    
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;

private:
    pointer_type m_eng_ptr;
    data_type scalar;

public:

    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr), scalar(0)
    {}

    template<typename ST>
    requires 
        std::convertible_to<ST, data_type>
    explicit
    constexpr engine_view(engine_type const& rhs, ST sclr)
    : m_eng_ptr(&rhs), scalar(static_cast<data_type>(sclr))
    {}

    




};