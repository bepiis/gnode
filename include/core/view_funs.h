//
//  view_funs.h
//  Created by Ben Westcott on 12/4/24.
//

#pragma once

#include <string>

namespace pfuns
{

template<typename ...Ts>
struct consumer : std::function<void(Ts...)>
{
    using std::function<void(Ts...)>::function;
};

template<typename ...Ts>
using producer = consumer<consumer<Ts...>>;

template<typename In, typename Out>
using process = consumer<producer<In>, consumer<Out>>;

template<typename In, typename Out>
constexpr auto operator|(process<In, Out> proc, consumer<Out> cons)
-> consumer<In>
{
    return [proc, cons](In in)
    { 
        proc([&in](consumer<In> snk) mutable { snk(std::forward<In>(in)); }, cons); 
    };
}

template<typename In, typename Out>
constexpr auto operator|(producer<In> prod, process<In, Out> proc)
-> producer<Out> 
{
    return [prod, proc](consumer<Out> out)
    {
        proc(prod, out);
    };
}

template<typename In, typename Inter, typename Out>
constexpr auto operator|(process<In, Inter> aproc, process<Inter, Out> bproc)
-> process<In, Out> 
{
    return [aproc, bproc](producer<In> in, consumer<Out> out)
    {
        aproc(in, bproc | out);
    };
}

template<typename ...Ts>
auto operator|(producer<Ts...> src, consumer<Ts...> snk)
-> consumer<>
{
    return [src, snk]
    {
        src(snk);
    };
}


};