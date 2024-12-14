//
//  test_view_funs.cpp
//  Created by Ben Westcott on 12/12/24.
//

template<typename VEgn>
pfuns::process<VEgn, typename VEgn::const_reference> tposer = [](pfuns::producer<VEgn> in, pfuns::consumer<typename VEgn::const_reference> out)
{
    in([&out](VEgn eng) { out(eng(1, 2)); });
};


TEST_CASE
(
    "basic pipe test with negation operator"
)
{

    using namespace pfuns;

    process<char, char> to_upper = [](producer<char> in, consumer<char> out)
    {
        in ([&out](char c) {out(std::toupper(c));});
    };

    producer<char> hello = [ptr="hello"](consumer<char> s)
    {
        for(auto it=ptr; *it; ++it){ s(*it); }
    };

    consumer<char> print = [](char c){std::cout << c;};

    auto prog = hello | to_upper | print;
    prog();

    using cxd = std::complex<double>;

    process<cxd, cxd> conj = [](producer<cxd> in, consumer<cxd> out)
    {
        in( [&out](cxd c) {out(std::conj(c));});
    };

    consumer<cxd> printc = [](cxd c) { std::cout << c; };

    cxd num = cxd(1.0, -1.0);
    producer<cxd> start = [num](consumer<cxd> s)
    {
        s(num);
    }; 

    auto progx = start | conj | printc;
    progx();

    using dtype = double;
    using atype = std::allocator<double>;
    
    constexpr size_t nrows = 3;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using MTE = engine_view<M, inport_views::transparent>;

    const literal2D<double> data_in = {{1,2,3},{4,5,6},{7,8,9}};
    M m(data_in);
    MTE mte(m);

    producer<MTE> st = [mte](consumer<MTE> s)
    {
        s(mte, 1, 2);
    };

    consumer<double> printd = [](double const& num) { std::cout << num << "\n";};

    auto progy = st | tposer<MTE>;

}