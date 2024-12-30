//
//  scalar_product_view.h
//  Created by Ben Westcott on 12/19/24.
//

template<typename Egn, typename ST>
requires
    exportable<Egn>
struct engine_view<product_views::scalar, Egn, ST>
{

public:
    using owning_engine_type = typename has_owning_engine_type_alias<Egn>::owning_engine_type;
    using engine_type = Egn;

private:
    using pointer = engine_type const*;
    using ctor_type = engine_type const&;
    
    using sz_extract = engine_ct_extents<engine_type>;

public:
    using orientation_type = typename get_engine_orientation<engine_type>::type;
    using data_type = typename engine_type::data_type;
    using index_type = typename engine_type::index_type;
    using reference = typename engine_type::const_reference;
    using const_reference = typename engine_type::const_reference;

private:
    pointer m_eng_ptr;

public:

    constexpr engine_view() noexcept
    : m_eng_ptr(nullptr)
    {}

    

};
