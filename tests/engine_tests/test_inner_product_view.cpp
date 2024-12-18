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


