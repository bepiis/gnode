//
//  test_storage_engine_col_static.cpp
//  Created by Ben Westcott on 11/22/24.
//

TEST_CASE
(
    "IF m is nbr_rows, row_reach constructed\n"
    "AND THEN reshape_rows is called SUCH THAT new_nbr_rows > row_reach()\n"
    "THEN\n"
    "\tnew_nbr_rows == rows()\n"
    "\tAND row_reach == row_reach()\n"
    "\tAND rows() <= row_reach()\n"
    "\tAND ncols == cols()\n"
    "\tAND ncols == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 3 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN row_reach == 3 STRONG]"
    "[GIVEN DYN new_nbr_rows = 5 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t row_reach = 3;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach);

    size_t new_nbr_rows = 5;

    m.reshape_rows(new_nbr_rows, row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(new_nbr_rows == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());

    new_nbr_rows = 10;
    size_t new_row_reach = 15;

    m.reshape_rows(new_nbr_rows, new_row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(new_row_reach == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());
}

TEST_CASE
(
    "IF m is nbr_rows, row_reach constructed\n"
    "AND THEN reshape_rows is called SUCH THAT new_row_reach != row_reach()\n"
    "THEN\n"
    "\tnbr_rows == rows()\n"
    "\tAND new_row_reach == row_reach()\n"
    "\tAND rows() <= row_reach()\n"
    "\tAND ncols == cols()\n"
    "\tAND ncols == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 3 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN row_reach == 6 STRONG]"
    "[GIVEN DYN new_row_reach = 10 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t row_reach = 6;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach);

    size_t new_row_reach = 10;
    m.reshape_rows(nbr_rows, new_row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(nbr_rows == m.rows());
    REQUIRE(new_row_reach == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());
}

TEST_CASE
(
    "IF m is nbr_rows, row_reach constructed\n"
    "AND THEN reshape_rows is called SUCH THAT new_nbr_rows <= row_reach() AND new_row_reach == row_reach()\n"
    "THEN\n"
    "\tnew_nbr_rows == rows()\n"
    "\tAND new_row_reach == row_reach == row_reach()\n"
    "\tAND rows() <= row_reach()\n"
    "\tAND ncols == cols()\n"
    "\tAND ncols == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 6 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN row_reach == 6 STRONG]"
    "[GIVEN DYN new_nbr_rows = 5 STRONG]"
    "[GIVEN DYN new_row_reach = 6 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 6;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t row_reach = 6;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach);

    size_t new_nbr_rows = 5;

    m.reshape_rows(new_nbr_rows, row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());

    new_nbr_rows = 2;

    m.reshape_rows(new_nbr_rows, row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());

    new_nbr_rows = 2;

    m.reshape_rows(new_nbr_rows, row_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());
}

// dynamic rows, static cols rect init list constructed but ncols != literal2D.begin()->size()
// then runtime_error is thrown
TEST_CASE
(
    "IF m is literal2D constructed BUT ncols != literal2D.begin()->size()\n"
    "THEN runtime_error is thrown.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 6]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"

)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 6;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<int64_t> data_in_1 = {{-11, -10, -9},
                                          {-8, -7, -6},
                                          {-5, -4, -3},
                                          {-2, -1, 0},
                                          {1, 2, 3},
                                          {4, 5, 6},
                                          {7, 8, 9},
                                          {10, 11, 12}};

    REQUIRE_THROWS
    (
        matrix_storage_engine<dtype, atype, nrows, ncols, ltype>(data_in_1)
    );

    const literal2D<int64_t> data_in_2 = {{-1, -2, -3, -4, -5, -6, -7, -8},
                                          {1, 2, 3, 4, 5, 6, 7, 8},
                                          {-9, 10, -11, 12 -13, 14, -15, 16}};
    
    REQUIRE_THROWS
    (
        matrix_storage_engine<dtype, atype, nrows, ncols, ltype>(data_in_2)
    );

}

// dynamic rows, static cols rect init list constructed
// then rows(), cols(), row_reach() and col_reach() matches input data rows, cols
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() == rows() == row_reach() AND literal2D.begin()->size() == cols() == col_reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in_1 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    size_t nbr_din_rows_1 = data_in_1.size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in_1);

    REQUIRE(nbr_din_rows_1 == m1.rows());
    REQUIRE(nbr_din_rows_1 == m1.row_reach());
    REQUIRE(ncols == m1.cols());
    REQUIRE(ncols == m1.col_reach());

    const literal2D<double> data_in_2 = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                         {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                         {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                         {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                         {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                         {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};
    
    size_t nbr_din_rows_2 = data_in_2.size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in_2);

    REQUIRE(nbr_din_rows_2 == m2.rows());
    REQUIRE(nbr_din_rows_2 == m2.row_reach());
    REQUIRE(ncols == m2.cols());
    REQUIRE(ncols == m2.col_reach());
}

// dynamic rows, static cols, rect init list constructed, 
//then size() and reach() consistent with input data size
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() * literal2D.begin()->size() == size() == reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in_1 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    size_t nbr_din_rows_1 = data_in_1.size();
    size_t nbr_din_cols_1 = data_in_1.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in_1);

    REQUIRE(nbr_din_rows_1 * nbr_din_cols_1 == m1.size());
    REQUIRE(nbr_din_rows_1 * nbr_din_cols_1 == m1.reach());

    const literal2D<double> data_in_2 = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                         {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                         {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                         {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                         {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                         {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};
    
    size_t nbr_din_rows_2 = data_in_2.size();
    size_t nbr_din_cols_2 = data_in_2.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in_2);

    REQUIRE(nbr_din_rows_2 * nbr_din_cols_2 == m2.size());
    REQUIRE(nbr_din_rows_2 * nbr_din_cols_2 == m2.reach());
}

// dynamic rows, static cols literal2D constructed,
// then data matches input data


// dynamic rows, static cols default constructed, literal2D assigned,
// then data matches input data
