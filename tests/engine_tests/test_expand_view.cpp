//
//  test_expand_view.cpp
//  Created by Ben Westcott on 12/3/24.
//

namespace expand_view_test_space
{
    template<typename VEgn>
    requires readable_engine<VEgn>
    struct mod_repeater
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        index_type nrows, ncols;

        mod_repeater(index_type nr, index_type nc)
        : nrows(nr), ncols(nc)
        {}

        constexpr auto operator()(engine_type eng, index_type i, index_type j) 
        -> const_reference
        {
            return eng(i % nrows, j % ncols);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct band_repeater
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::data_type;
        using engine_type = VEgn const&;

        index_type nrows, ncols;

        band_repeater(index_type nr, index_type nc)
        : nrows(nr), ncols(nc)
        {}

        constexpr auto operator()(engine_type eng, index_type i, index_type j) 
        -> const_reference
        {
            if((i >= j &&  i < j + nrows) || (j >= i && j < i + ncols))
            {
                return eng(i % nrows, j % nrows);
            }
            else return 0;
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct row_repeater
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        index_type ncols;

        row_repeater(index_type nc)
        : ncols(nc)
        {}

        constexpr auto operator()(engine_type eng, index_type i, index_type j) 
        -> const_reference
        {
            return eng(0, j);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct transposer
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        static constexpr auto rows(engine_type eng) -> index_type
        {
            return eng.cols();
        }

        static constexpr auto cols(engine_type eng) -> index_type
        {
            return eng.rows();
        }

        static constexpr auto operator()(engine_type eng, index_type i, index_type j)
        -> const_reference
        {
            return eng(j, i);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct test_repeater_noidx_t
    {
        //using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        constexpr auto operator()(engine_type eng, size_t i, size_t j) 
        -> const_reference
        {
            return eng(i, j);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct test_repeater_nocref_t
    {
        using index_type = typename VEgn::index_type;
        //using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        constexpr auto operator()(engine_type eng, index_type i, index_type j) 
        -> typename VEgn::const_reference
        {
            return eng(i, j);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct test_repeater_noeng_t
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        //using engine_type = VEgn const&;

        constexpr auto operator()(VEgn const& eng, index_type i, index_type j) 
        -> const_reference
        {
            return eng(i, j);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct test_repeater_noivk_t
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        constexpr auto operator()(VEgn const& eng, index_type i, index_type j) 
        -> typename VEgn::data_type
        {
            return eng(i, j);
        }
    };

    template<typename VEgn>
    requires readable_engine<VEgn>
    struct test_repeater_no_t
    {
        using index_type = typename VEgn::index_type;
        using const_reference = typename VEgn::const_reference;
        using engine_type = VEgn const&;

        constexpr auto operator()(VEgn const& eng, index_type i, index_type j) 
        -> const_reference
        {
            return eng(i, j);
        }
    };
};

TEST_CASE
(
    "valid expander concept rejects expanders which dont\n"
    "have an invocable operator() OR\n"
    "define an index type, const ref and engine type OR"
    "invocable operator returns something other than its const reference.\n"
)
{

    using namespace expand_view_test_space;

    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using T = engine_view<K, export_views::transparent>;

    REQUIRE(false == valid_expander<test_repeater_noidx_t<T>, T const&, size_t, size_t>);
    REQUIRE(false == valid_expander<test_repeater_nocref_t<T>, T const&, size_t, size_t>);
    REQUIRE(false == valid_expander<test_repeater_noeng_t<T>, T const&, size_t, size_t>);
    REQUIRE(false == valid_expander<test_repeater_noeng_t<T>, T const&, size_t, size_t>);
    REQUIRE(false == valid_expander<test_repeater_noivk_t<T>, T const&, size_t, size_t>);
    REQUIRE(true == valid_expander<test_repeater_no_t<T>, T const&, size_t, size_t>);
}

TEST_CASE
(
    "IF expand view type satisfies:\n"
    "readable engine\n"
    "nothrow swappable\n"
    "not owning engine\n"
    "THEN it satisfies view basics\n"
)
{
    using namespace expand_view_test_space;

    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using T = engine_view<K, export_views::transparent>;
    using TR = engine_view<T, export_views::transpose>;

    using EV = expand_view<TR, transposer>;

    size_t nr = 3;
    size_t nc = 3;

    size_t prows = 20;
    size_t pcols = 20;

    const literal2D<int64_t> data_in = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};

    //const band_repeater<TR> mr(nr, nc);
    

    K k(data_in);
    T t(k);
    TR tr(t);


    EV e(tr);

    eh::print(e);
    std::cout << e.rows() << "\t" << e.cols() << "\n";
}

TEST_CASE
(
    "test"
)
{
    using namespace expand_view_test_space;

    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using K = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;
    using T = engine_view<K, export_views::transparent>;

    using EV = expand_view<T, row_repeater>;

    const literal2D<double> data_in = {{1, 2, 3, 4, 5, 6}};

    size_t nc = 6;
    size_t nr = 6;

    //const row_repeater<T> rr(nc);
/*
    K k(data_in);
    T t(k);
    EV e(t, nr, nc, rr);

    eh::print(e);*/
}

TEST_CASE
(
    "IF expand view type satisfies:\n"
    "view basics\n"
    "has an immutable reference type\n"
    "THEN it is an immutable view\n"
)
{

}

TEST_CASE
(
    "Expand view is NOT a mutable view\n"
)
{

}


