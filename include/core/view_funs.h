//
//  view_funs.h
//  Created by Ben Westcott on 12/4/24.
//

#pragma once

// NOTE: these are NOT engine types although they may seem like they might be
//       and thus, they intentionally do not satisfy the base engine type

namespace vfuns
{

template<typename Egn>
requires 
    base_engine<Egn>
struct transparent
{
    using index_type = typename Egn::index_type;
    using reference = typename Egn::reference;
    using const_reference = typename Egn::const_reference;
    using engine_type = Egn;

    static constexpr auto rows(engine_type const& eng)
    -> index_type
    {
        return eng.rows();
    }

    static constexpr auto cols(engine_type const& eng) 
    -> index_type
    {
        return eng.cols();
    }

    static constexpr auto eval(engine_type const& eng, index_type i, index_type j)
    -> const_reference
    requires readable_engine<Egn>
    {
        return eng(i, j);
    }

    static constexpr auto eval(engine_type & eng, index_type i, index_type j)
    -> reference
    requires writable_engine<Egn>
    {
        return eng(i, j);
    }

};

template<typename Egn>
requires 
    base_engine<Egn>
struct transpose
{
    using index_type = typename Egn::index_type;
    using reference = typename Egn::reference;
    using const_reference = typename Egn::const_reference;
    using engine_type = Egn;

    static constexpr auto rows(engine_type const& eng) 
    -> index_type
    {
        return eng.cols();
    }

    static constexpr auto cols(engine_type const& eng) 
    -> index_type
    {
        return eng.rows();
    }
    
    static constexpr auto eval(engine_type const& eng, index_type i, index_type j)
    -> const_reference
    requires readable_engine<Egn>
    {
        return eng(j, i);
    }

    static constexpr auto eval(engine_type & eng, index_type i, index_type j)
    -> reference
    requires writable_engine<Egn>
    {
        return eng(j, i);
    }
};

template<typename Egn>
requires 
    base_engine<Egn> and
    valid_unary_minus_operator<typename Egn::data_type>
struct negation
{
    using index_type = typename Egn::index_type;
    using reference = typename Egn::data_type;
    using const_reference = typename Egn::data_type;
    using engine_type = Egn;

    static constexpr auto rows(engine_type const& eng) 
    -> index_type
    {
        return eng.rows();
    }

    static constexpr auto cols(engine_type const& eng) 
    -> index_type
    {
        return eng.cols();
    }
    
    static constexpr auto eval(engine_type const& eng, index_type i, index_type j)
    -> const_reference
    requires readable_engine<Egn>
    {
        return -eng(i, j);
    }
};

template<typename Egn>
requires
    base_engine<Egn> and
    has_conjugate<typename Egn::data_type>
struct conjugate
{
    using index_type = typename Egn::index_type;
    using reference = typename Egn::data_type;
    using const_reference = typename Egn::data_type;
    using engine_type = Egn;

    static constexpr auto rows(engine_type const& eng) 
    -> index_type
    {
        return eng.rows();
    }

    static constexpr auto cols(engine_type const& eng) 
    -> index_type
    {
        return eng.cols();
    }
    
    static constexpr auto eval(engine_type const& eng, index_type i, index_type j)
    -> const_reference
    requires readable_engine<Egn>
    {
        return std::conj(eng(i, j));
    }

};








};




