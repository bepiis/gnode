//
//  test_outer_product_view.cpp
//  Created by Ben Westcott on 12/23/24.
//

TEST_CASE
(
    "IF M, N are storage engine types,\n"
    "RV and CV are row and col view types,\n"
    "THEN if rv and cv are constructed from m, n,\n"
    "THEN the outer product is well defined over the range\n"
    "of rv's rows (m's rows) and cv's cols (n's cols).\n"
)
{
    using dtypeM = std::complex<double>;
    using atypeM = std::allocator<dtypeM>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrows, ncols, ltype>;
    using RV = engine_view<export_views::row, M>;

    const literal2D<std::complex<double>> data_in_m =
        {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
         {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
         {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
         {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};

    M m(data_in_m);
    RV rv(m, 1);

    using dtypeN = std::complex<long double>;
    using atypeN = std::allocator<dtypeN>;

    using N = matrix_storage_engine<dtypeN, atypeN, nrows, ncols, ltype>;
    using CV = engine_view<export_views::col, N>;

    const literal2D<std::complex<double>> data_in_n =
        {{1.0 - 0.5i, 1.5 - 1.0i, 2.0 - 1.5i, 2.5 - 2.0i},
         {3.0 - 2.5i, 3.5 - 3.0i, 4.0 - 3.5i, 4.5 - 4.0i},
         {5.0 - 4.5i, 5.5 - 5.0i, 6.0 - 5.5i, 6.5 - 6.0i},
         {7.0 - 6.5i, 7.5 - 7.0i, 8.0 - 7.5i, 8.5 - 8.0i}};

    N n(data_in_n);
    CV cv(n, 2);

    using OP = engine_view<product_views::outer,CV, RV>;

    OP op(cv, rv);

    REQUIRE(cv.rows() == op.rows());
    REQUIRE(rv.cols() == op.cols());
    REQUIRE(cv.rows() * rv.cols() == op.size());

    REQUIRE(std::same_as<decltype(op(0, 0)), std::complex<long double>>);
}

TEST_CASE
(
    "IF M, N are storage engine types,\n"
    "and the data type of N's data type is such that\n"
    "it is callable on M's data type,\n"
    "and IF RV and CV are row and col view types,\n"
    "and rv and cv are constructed from m, n,\n"
    "then outer product view gives the matrix Fc(m1) Fc(m2) ... Fc(mn).\n"
)
{
    using dtypeM = std::complex<double>;
    using atypeM = std::allocator<dtypeM>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtypeM, atypeM, nrows, ncols, ltype>;
    using RV = engine_view<export_views::row, M>;

    const literal2D<dtypeM> data_in_m =
        {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
         {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
         {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
         {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};

    M m(data_in_m);
    RV rv(m, 1);

    using dtypeN = std::function<std::complex<double>(dtypeM)>;
    using atypeN = std::allocator<dtypeN>;

    auto fun = [](double f, dtypeM d) -> std::complex<double> { return d * std::exp(-1.0i*pi*f); };

    using N = matrix_storage_engine<dtypeN, atypeN, nrows, ncols, ltype>;
    using CV = engine_view<export_views::col, N>;

    const literal2D<dtypeN> data_in_n =
        {
            {std::bind(fun, 0.25, std::placeholders::_1), std::bind(fun, -0.25, std::placeholders::_1)},
            {std::bind(fun, 0.33, std::placeholders::_1), std::bind(fun, -0.33, std::placeholders::_1)},
            {std::bind(fun, 0.50, std::placeholders::_1), std::bind(fun, -0.50, std::placeholders::_1)},
            {std::bind(fun, 1.00, std::placeholders::_1), std::bind(fun, -1.00, std::placeholders::_1)}
        };

    N n(data_in_n);
    CV cv(n, 0); 

    using OP = engine_view<product_views::outer, CV, RV>;

    OP op(cv, rv);

    eh::print(op);
}

TEST_CASE
(
    "IF M, N are storage engine types,\n"
    "and the data type of M's data type is such that\n"
    "it is callable on N's data type,\n"
    "and IF RV and CV are row and col view types,\n"
    "and rv and cv are constructed from m, n,\n"
    "then outer product view gives the matrix Fc(m1) Fc(m2) ... Fc(mn).\n"
)
{

    using dtypeN = std::complex<double>;
    using atypeN = std::allocator<dtypeN>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major;

    using N = matrix_storage_engine<dtypeN, atypeN, nrows, ncols, ltype>;
    using CV = engine_view<export_views::col, N>;

    const literal2D<dtypeN> data_in_n =
        {{1.0 + 0.5i, 1.5 + 1.0i, 2.0 + 1.5i, 2.5 + 2.0i},
         {3.0 + 2.5i, 3.5 + 3.0i, 4.0 + 3.5i, 4.5 + 4.0i},
         {5.0 + 4.5i, 5.5 + 5.0i, 6.0 + 5.5i, 6.5 + 6.0i},
         {7.0 + 6.5i, 7.5 + 7.0i, 8.0 + 7.5i, 8.5 + 8.0i}};

    N n(data_in_n);
    CV cv(n, 0);

    using Rtype = std::complex<long double>;

    using dtypeM = std::function<Rtype(dtypeN)>;
    using atypeM = std::allocator<dtypeM>;
    
    auto fun = [](dtypeN d, size_t gen_i, size_t gen_j) -> Rtype
    {
        return d * std::exp(1.00i * pi * (double)((1 + gen_i + gen_j)/2.0) * (std::pow(-1, (1 + gen_i + gen_j))));
    };

    using std::placeholders::_1;

    const literal2D<dtypeM> data_in_m = 
        {
            {std::bind(fun, _1, 0, 0), std::bind(fun, _1, 0, 1), std::bind(fun, _1, 0, 2), std::bind(fun, _1, 0, 3)},
            {std::bind(fun, _1, 1, 0), std::bind(fun, _1, 1, 1), std::bind(fun, _1, 1, 2), std::bind(fun, _1, 1, 3)},
            {std::bind(fun, _1, 2, 0), std::bind(fun, _1, 2, 1), std::bind(fun, _1, 2, 2), std::bind(fun, _1, 2, 3)},
            {std::bind(fun, _1, 3, 0), std::bind(fun, _1, 3, 1), std::bind(fun, _1, 3, 2), std::bind(fun, _1, 3, 3)}
        };

    using M = matrix_storage_engine<dtypeM, atypeM, nrows, ncols, ltype>;
    using RV = engine_view<export_views::row, M>;

    M m(data_in_m);
    RV rv(m, 0);

    using OP = engine_view<product_views::outer, CV, RV>;

    OP op(cv, rv);

    std::cout << "\n";

    eh::print(op);


    

}


