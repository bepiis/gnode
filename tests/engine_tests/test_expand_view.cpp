//
//  test_expand_view.cpp
//  Created by Ben Westcott on 12/3/24.
//

namespace expand_view_test_space
{
    template<typename VEgn>
    struct mod_repeater
    {
        using idx_t = typename VEgn::index_type;
        using cref_t = typename VEgn::const_reference;
        using eng_t = VEgn const&;

        idx_t nrows, ncols;

        mod_repeater(idx_t nr, idx_t nc)
        : nrows(nr), ncols(nc)
        {}

        constexpr auto operator()(eng_t eng, idx_t i, idx_t j) 
        -> cref_t
        {
            return eng(i % nrows, j % ncols);
        }
    };

    template<typename VEgn>
    struct band_repeater
    {
        using idx_t = typename VEgn::index_type;
        using cref_t = typename VEgn::const_reference;
        using eng_t = VEgn const&;

        idx_t nrows, ncols;

        band_repeater(idx_t nr, idx_t nc)
        : nrows(nr), ncols(nc)
        {}

        constexpr auto operator()(eng_t eng, idx_t i, idx_t j) 
        -> cref_t
        {
            if((i >= j &&  i < j + nrows) || (j >= i && j < i + ncols))
            {
                return eng(i % nrows, j % nrows);
            }
            else return 0;
        }
    };

    template<typename VEgn>
    struct row_repeater
    {
        using idx_t = typename VEgn::index_type;
        using cref_t = typename VEgn::const_reference;
        using eng_t = VEgn const&;

        idx_t ncols;

        row_repeater(idx_t nc)
        : ncols(nc)
        {}

        constexpr auto operator()(eng_t eng, idx_t i, idx_t j) 
        -> cref_t
        {
            return eng(0, j);
        }
    };
};

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

    using EV = expand_view<TR, band_repeater<TR>>;

    size_t nr = 3;
    size_t nc = 3;

    size_t prows = 20;
    size_t pcols = 20;

    const literal2D<int64_t> data_in = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    const band_repeater<TR> mr(nr, nc);

    K k(data_in);
    T t(k);
    TR tr(t);


    EV e(tr, prows, pcols, mr);

    eh::print(e);
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

    using EV = expand_view<T, row_repeater<T>>;

    const literal2D<double> data_in = {{1, 2, 3, 4, 5, 6}};

    size_t nc = 6;
    size_t nr = 6;

    const row_repeater<T> rr(nc);

    K k(data_in);
    T t(k);
    EV e(t, nr, nc, rr);

    //eh::print(e);
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


