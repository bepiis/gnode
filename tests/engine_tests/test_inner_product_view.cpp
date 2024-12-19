//
//  test_inner_product_view.cpp
//  Created by Ben Westcott on 12/16/24.
//

#include <utility>

TEST_CASE
(
    "IF M, N are storage engine types\n"
    "AND M, N have rowvec and colvec dimensions\n"
    "respectfully and have convertible types\n"
    "THEN types M and N satisfy the requirements\n"
    "of inner_product.\n"
)
{
    using dtypeM = double;
    using atypeM = std::allocator<dtypeM>;
    
    constexpr size_t nrowsM = 1;
    constexpr size_t ncolsM = std::dynamic_extent;

    using ltypeM = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrowsM, ncolsM, ltypeM>;

    //const literal2D<dtypeM> data_in_m = {{1, 2, 3, 4, 5, 6, 7, 8}};
    //M m(data_in_m);

    REQUIRE(true == rowvec_dimensions<M>);
    REQUIRE(true == exportable<M>);

    using dtypeN = int64_t;
    using atypeN = std::allocator<dtypeN>;

    constexpr size_t nrowsN = std::dynamic_extent;
    constexpr size_t ncolsN = 1;

    using ltypeN = matrix_orientation::col_major;

    using N = matrix_storage_engine<dtypeN, atypeN, nrowsN, ncolsN, ltypeN>;

    //const literal2D<dtypeN> data_in_n = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}};
    //N n(data_in_n);

    REQUIRE(true == colvec_dimensions<N>);
    REQUIRE(true == exportable<N>);

    using dtypeR = double;

    REQUIRE(true == std::convertible_to<dtypeR, typename M::data_type>);
    REQUIRE(true == std::convertible_to<dtypeR, typename N::data_type>);

    REQUIRE
    (
        true ==
        std::same_as
        <
            dtypeR,
            std::invoke_result<decltype(inner_product<M, N, dtypeR, false>), M, N>::type
        >
    );

    REQUIRE
    (
        true ==
        std::same_as
        <
            dtypeR,
            std::invoke_result<decltype(inner_product<M, N, dtypeR, true>), M, N>::type
        >
    );
    

}

TEST_CASE
(
    "IF M, N are storage engine types\n"
    "AND M, N have rowvec and colvec dimensions\n"
    "THEN IF m and n are constructed from types M and N\n"
    "THEN inner_product(m, n) returns the expected value and type\n"
)
{
    using dtypeM = double;
    using atypeM = std::allocator<dtypeM>;
    
    constexpr size_t nrowsM = 1;
    constexpr size_t ncolsM = std::dynamic_extent;

    using ltypeM = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrowsM, ncolsM, ltypeM>;

    const literal2D<dtypeM> data_in_m = {{1, 2, 3, 4, 5, 6, 7, 8}};
    M m(data_in_m);

    using dtypeN = int64_t;
    using atypeN = std::allocator<dtypeN>;

    constexpr size_t nrowsN = std::dynamic_extent;
    constexpr size_t ncolsN = 1;

    using ltypeN = matrix_orientation::col_major;

    using N = matrix_storage_engine<dtypeN, atypeN, nrowsN, ncolsN, ltypeN>;

    const literal2D<dtypeN> data_in_n = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}};
    N n(data_in_n);

    using dtypeR = double;
    const dtypeR inner_mn = 204;

    auto res_guarded = inner_product<M, N, dtypeR, true>(m, n);
    REQUIRE(true == std::same_as<dtypeR, decltype(res_guarded)>);
    REQUIRE(inner_mn == res_guarded);

    auto res_unguarded = inner_product<M, N, dtypeR, false>(m, n);
    REQUIRE(true == std::same_as<dtypeR, decltype(res_unguarded)>);
    REQUIRE(inner_mn == res_unguarded);
}

TEST_CASE
(
    "IF M, N are storage engine types\n"
    "AND M, N dont nessesarily have row/colvec dimensions\n"
    "AND RV, CV types are row/colview types of M and N respectively\n"
    "THEN IF rv and cv are constructed from types RV and CV\n"
    "THEN inner_product(rv, cv) returns the expected value and type\n"
)
{
    using dtypeM = std::complex<double>;
    using atypeM = std::allocator<dtypeM>;
    
    constexpr size_t nrowsM = std::dynamic_extent;
    constexpr size_t ncolsM = std::dynamic_extent;

    using ltypeM = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrowsM, ncolsM, ltypeM>;

    const literal2D<std::complex<double>> data_in_m =
        {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
         {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
         {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
         {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};

    M m(data_in_m);

    using RV = engine_view<export_views::row, M>;

    RV rv(m, 1);

    using dtypeN = std::complex<double>;
    using atypeN = std::allocator<dtypeN>;

    constexpr size_t nrowsN = std::dynamic_extent;
    constexpr size_t ncolsN = std::dynamic_extent;

    using ltypeN = matrix_orientation::row_major;

    using N = matrix_storage_engine<dtypeN, atypeN, nrowsN, ncolsN, ltypeN>;

    const literal2D<std::complex<double>> data_in_n =
        {{1.0 - 0.5i, 1.5 - 1.0i, 2.0 - 1.5i, 2.5 - 2.0i},
         {3.0 - 2.5i, 3.5 - 3.0i, 4.0 - 3.5i, 4.5 - 4.0i},
         {5.0 - 4.5i, 5.5 - 5.0i, 6.0 - 5.5i, 6.5 - 6.0i},
         {7.0 - 6.5i, 7.5 - 7.0i, 8.0 - 7.5i, 8.5 - 8.0i}};  

    N n(data_in_n);

    using CV = engine_view<export_views::col, N>;

    CV cv(n, 2);

    using dtypeR = std::complex<double>;
    const dtypeR inner_rvcv = 143.50 - 2.5i;

    auto res_guarded = inner_product<RV, CV, dtypeR, true>(rv, cv);
    REQUIRE(true == std::same_as<dtypeR, decltype(res_guarded)>);
    REQUIRE(inner_rvcv == res_guarded);

    auto res_unguarded = inner_product<RV, CV, dtypeR, false>(rv, cv);
    REQUIRE(true == std::same_as<dtypeR, decltype(res_unguarded)>);
    REQUIRE(inner_rvcv == res_unguarded); 

    REQUIRE(std::common_with<std::complex<int64_t>, double>);
   //REQUIRE(true == std::is_arithmetic_v<std::complex<double>>);
}

TEST_CASE
(
    "test"
)
{
    using cxd = std::complex<double>;
    using cxdf = std::function<cxd(cxd)>;

    using dtypeM = cxdf;
    using atypeM = std::allocator<dtypeM>;
    
    constexpr size_t nrowsM = std::dynamic_extent;
    constexpr size_t ncolsM = std::dynamic_extent;

    using ltypeM = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrowsM, ncolsM, ltypeM>;

    const literal2D<cxdf> data_in_m =
        {{[](cxd c) -> cxd { return c * 1i; }, 
          [](cxd c) -> cxd {return c * 2i; }, 
          [](cxd c) -> cxd { return c * 3i; }}};

    M m(data_in_m);

    using RV = engine_view<export_views::row, M>;

    RV rv(m, 0);

    using dtypeN = std::complex<double>;
    using atypeN = std::allocator<dtypeN>;

    constexpr size_t nrowsN = std::dynamic_extent;
    constexpr size_t ncolsN = std::dynamic_extent;

    using ltypeN = matrix_orientation::row_major;

    using N = matrix_storage_engine<dtypeN, atypeN, nrowsN, ncolsN, ltypeN>;

    const literal2D<std::complex<double>> data_in_n =
        {{1.0 - 0.5i, 1.5 - 1.0i, 2.0 - 1.5i, 2.5 - 2.0i},
         {3.0 - 2.5i, 3.5 - 3.0i, 4.0 - 3.5i, 4.5 - 4.0i},
         {5.0 - 4.5i, 5.5 - 5.0i, 6.0 - 5.5i, 6.5 - 6.0i}};

    N n(data_in_n);

    using CV = engine_view<export_views::col, N>;

    CV cv(n, 1);

    auto res = inner_product<RV, CV, cxd, false>(rv, cv);

    std::cout << res << "\n";
} 

TEST_CASE
(
    "IF RVT, CVT are types storage engine,\n"
    "RVT, M share the same data type,\n"
    "CVT satisfies colvec dimensions and RVT satisfies rowvec dimensions\n"
    "IP is type engine view specialized with inner product tag\n"
    "THEN inner<RVT, CVT>\n"
    "produces the expected product.\n"
)
{

    using dtype = double;
    using atype = std::allocator<double>;

    using CVT = matrix_storage_engine<dtype, atype, std::dynamic_extent, 1, matrix_orientation::row_major>;
    using RVT  = matrix_storage_engine<dtype, atype, 1, std::dynamic_extent, matrix_orientation::col_major>;

    using IP1 = engine_view<product_views::inner, RVT, CVT>;

    const literal2D<double> data_in_2 = 
        {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10}};

    const literal2D<double> data_in_3 = 
        {{1.00}, {1.01}, {1.02}, {1.03}, {1.04}, {1.05}, {-1.00}, {-1.10}};

    const literal2D<double> data_in_4 =
        {{1.00, 1.06, 1.12, 1.18, 1.24, 1.30},
         {1.01, 1.07, 1.13, 1.19, 1.25, 1.31},
         {1.02, 1.08, 1.14, 1.20, 1.26, 1.32}, 
         {1.03, 1.09, 1.15, 1.21, 1.27, 1.33},
         {1.04, 1.10, 1.16, 1.22, 1.28, 1.34},
         {1.05, 1.11, 1.17, 1.23, 1.29, 1.35},
         {-1.00, -2.00, -3.00, -4.00, -5.00, -6.00},
         {-1.10, -2.20, -3.30, -4.40, -5.50, -6.60}};

    double expected_out = 8.5155;

    RVT n1t(data_in_2);
    CVT n1(data_in_3);

    IP1 ip1(n1t, n1);

    REQUIRE(1 == ip1.rows());
    REQUIRE(1 == ip1.cols());
    REQUIRE(1 == ip1.size());
    REQUIRE_THAT(ip1(0, 0), Catch::Matchers::WithinRel(expected_out, 1E-10));
/*
    using cxd = std::complex<double>;
    using Nc =  matrix_storage_engine<cxd, std::allocator<cxd>, 1, std::dynamic_extent, matrix_orientation::col_major>;
    using IPcx = engine_view<product_views::inner, Nc, N>;

    const literal2D<cxd> data_in_5 = 
        {{2.00 + 1.00i, 3.00 + 1.01i, 4.00 + 1.02i, 5.00 + 1.03i, 6.00 + 1.04i, 7.00 + 1.05i, 8.00 - 1.00i, 9.00 - 1.10i}};

    Nc nc(data_in_5);

    IPcx ipcx(nc, n);

    eh::print(ipcx);*/

}

TEST_CASE
(
    "IF RVT, M are both storage engine types,\n"
    "RVT, M share the same data type,\n"
    "RVT satisfies rowvec dimensions,\n"
    "M does not satisfy row or colvec dimensions,\n"
    "then engine view with inner product tag produces a rowvec dimension\n"
    "type with the expected computed products and size 1 by m.cols().\n"
)
{
    using dtype = long double;
    using atype = std::allocator<dtype>;

    constexpr size_t rvt_nrows = 1;
    constexpr size_t rvt_ncols = std::dynamic_extent;

    constexpr size_t m_nrows = std::dynamic_extent;
    constexpr size_t m_ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using RVT = matrix_storage_engine<dtype, atype, rvt_nrows, rvt_ncols, ltype>;

    const literal2D<dtype> data_in_rvt = 
        {{5.00, 10.00, 15.00, 20.00, 25.00, 30.00, 35.00, 40.00}};

    RVT rv(data_in_rvt);
    
    using M = matrix_storage_engine<dtype, atype, m_nrows, m_ncols, ltype>;

    const literal2D<dtype> data_in_m =
        {{1.00, 1.06, 1.12, 1.18, 1.24, 1.30},
         {1.01, 1.07, 1.13, 1.19, 1.25, 1.31},
         {1.02, 1.08, 1.14, 1.20, 1.26, 1.32}, 
         {1.03, 1.09, 1.15, 1.21, 1.27, 1.33},
         {1.04, 1.10, 1.16, 1.22, 1.28, 1.34},
         {1.05, 1.11, 1.17, 1.23, 1.29, 1.35},
         {-1.00, -2.00, -3.00, -4.00, -5.00, -6.00},
         {-1.10, -2.20, -3.30, -4.40, -5.50, -6.60}};

    M m(data_in_m);

    using IP = engine_view<product_views::inner, RVT, M>;

    IP ip(rv, m);

    REQUIRE(std::same_as<dtype, decltype(ip(0, 0))>);

    const literal2D<dtype> expected_out = 
        {{2.950E1, -4.320E1, -1.159E2, -1.886E2, -2.613E2, -3.340E2}};

    REQUIRE(expected_out.begin()->size() == ip.cols());
    REQUIRE(expected_out.size() == ip.rows());

    auto exp_ptr = expected_out.begin()->begin();
    size_t ipidx = 0;

    for(; exp_ptr != expected_out.begin()->end(); ++exp_ptr, ++ipidx)
    {
        REQUIRE_THAT(ip(0, ipidx), Catch::Matchers::WithinRel(*exp_ptr, 1E-10));
    }
}

TEST_CASE
(
    "IF RVT, CVT are types storage engine,\n"
    "RVT, CVT do not have the same data type but neither are complex\n"
    "CVT satisfies colvec dimensions and RVT satisfies rowvec dimensions\n"
    "IP is type engine view specialized with inner product tag\n"
    "THEN engine view with inner product tag produces a rowvec dimension type\n"
    "which is size 1 by 1, and whose data type is the more precise of the two types.\n"
)
{
    using dtype_rv = double;
    using atype_rv = std::allocator<dtype_rv>;

    constexpr size_t nrows_rv = 1;
    constexpr size_t ncols_rv = std::dynamic_extent;

    using ltype_rv = matrix_orientation::row_major;

    using RVT = matrix_storage_engine<dtype_rv, atype_rv, nrows_rv, ncols_rv, ltype_rv>;

    using dtype_cv = long double;
    using atype_cv = std::allocator<dtype_rv>;

    constexpr size_t nrows_cv = std::dynamic_extent;
    constexpr size_t ncols_cv = 1;

    using ltype_cv = matrix_orientation::row_major;

    using RCT = matrix_storage_engine<dtype_cv, atype_cv, nrows_cv, ncols_cv, ltype_cv>;



}


