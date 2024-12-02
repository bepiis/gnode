//
//  test_storage_engine_col_static.cpp
//  Created by Ben Westcott on 11/22/24.
//

TEST_CASE
(
    "IF M is a storage engine with static cols and dynamic rows\n"
    "THEN it is an owning engine\n"
    "THEN it has engine base types\n"
    "THEN it has mutable access\n"
    "THEN it has immutable access\n"
    "THEN it has a consistent mutable reference type\n"
    "THEN it has a consistent immutable reference type\n"
    "THEN it has consistent return sizes\n"
    "THEN it has consistent return lengths\n"
    "THEN it is row reshapeable\n"
    "THEN it is NOT col reshapeable\n"
    "THEN it is NOT reshapeable\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == has_owning_engine_type_alias<M>::is_owning);
    REQUIRE(true == base_types<M>);
    REQUIRE(true == consistent_mutable_ref_type<M>);
    REQUIRE(true == consistent_immutable_ref_type<M>);
    REQUIRE(true == consistent_return_sizes<M>);
    REQUIRE(true == consistent_return_lengths<M>);
    REQUIRE(true == mutable_access<M>);
    REQUIRE(true == immutable_access<M>);

    REQUIRE(false == reshapeable_engine<M>);
    REQUIRE(true == row_reshapeable_engine<M>);
    REQUIRE(false == col_reshapeable_engine<M>);
}

TEST_CASE
(
    "IF M is a storage engine with static cols and dynamic rows\n"
    "THEN it is a readable engine\n"
    "THEN it is a writable engine\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
}

TEST_CASE
(
    "IF M is a storage engine with static cols and dynamic rows\n"
    "THEN it is copy constructible\n"
    "THEN it is move constructible\n"
    "THEN it is default constructible\n"
    "THEN it is destructable\n"
    "THEN it is size_t, size_t constructible\n"
  //  "THEN it is copy constructible from another engine type\n"
  //  "THEN it is copy assignable from another engine type\n"
    "THEN it is constructible from a literal2D\n"
    "THEN it is assignable from a literal2D\n"
    "THEN it is nothrow swappable.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == std::is_copy_constructible_v<M>);
    REQUIRE(true == std::is_move_constructible_v<M>);
    REQUIRE(true == std::is_default_constructible_v<M>);
    REQUIRE(true == std::is_destructible_v<M>);
    REQUIRE(true == std::constructible_from<M, size_t, size_t>);
    REQUIRE(false == std::constructible_from<M, size_t, size_t, size_t, size_t>);
    //REQUIRE(false == std::constructible_from<)
    REQUIRE(true == std::constructible_from<M, literal2D<dtype>>);
    REQUIRE(true == std::assignable_from<M&, literal2D<dtype>>);
    REQUIRE(true == std::is_nothrow_swappable_v<M&>);
}


TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape_rows is called SUCH THAT new_nbr_rows is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == -1 STRONG]"
    "[GIVEN DYN new_row_reach == 3]"
    "[core/storage_engine::matrix_storage_engine]"

)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = -1;
    int64_t new_row_reach = 3;

    REQUIRE_THROWS(m.reshape_rows(new_nbr_rows, new_row_reach));
}

TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape_rows is called SUCH THAT new_row_reach is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == 3]"
    "[GIVEN DYN new_row_reach == -5 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = 3;
    int64_t new_row_reach = -5;

    REQUIRE_THROWS(m.reshape_rows(new_nbr_rows, new_row_reach));
}

TEST_CASE
(
    "IF m is default constructed\n"
    "THEN\n"
    "\t0 == rows() == row_reach()\n"
    "\tAND ncols == cols() == col_reach().\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(0 == m.rows());
    REQUIRE(0 == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());

}

TEST_CASE
(
    "IF m is default constructed\n"
    "THEN storage flags set consistently.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 1,3]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols1 = 1;

    using ltype = matrix_orientation::row_major;

    matrix_storage_engine<dtype, atype, nrows, ncols1, ltype> m1;

    REQUIRE(true == m1.is_row_major);
    REQUIRE(false == m1.is_col_major);
    REQUIRE(false == m1.is_static_row_vector);
    REQUIRE(true == m1.is_static_col_vector);
    REQUIRE(true == m1.is_row_dynamic);
    REQUIRE(false == m1.is_col_dynamic);
    REQUIRE(false == m1.is_fully_dynamic);
    REQUIRE(false == m1.is_fully_static);

    constexpr size_t ncols2 = 3;

    matrix_storage_engine<dtype, atype, nrows, ncols2, ltype> m2;

    REQUIRE(true == m2.is_row_major);
    REQUIRE(false == m2.is_col_major);
    REQUIRE(false == m2.is_static_row_vector);
    REQUIRE(false == m2.is_static_col_vector);
    REQUIRE(true == m2.is_row_dynamic);
    REQUIRE(false == m2.is_col_dynamic);
    REQUIRE(false == m2.is_fully_dynamic);
    REQUIRE(false == m2.is_fully_static);
}

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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

    using ltype = matrix_orientation::row_major;

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
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN m_data_ij == data_in_ij.\n",
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

    using ltype = matrix_orientation::row_major;


    const literal2D<int64_t> data_in = {{-48, -46, -44, -42, -40, -38},
                                        {-36, -34, -32, -30, -28, -24},
                                        {-22, -20, -18, -16, -14, -12},
                                        {-10, -8, -6, -4, -2, 0},
                                        {2, 4, 6, 8, 10, 12},
                                        {14, 16, 18, 20, 22, 24}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(data_in);

    size_t i, j;
    auto din_i = data_in.begin();

    i = 0;
    for(; din_i < data_in.end(); i++, din_i++)
    {
        auto din_j = din_i->begin();
        j = 0;

        for(; din_j < din_i->end(); j++, din_j++)
        {
            REQUIRE(m(i, j) == *din_j);
        }
    }
}

// dynamic rows, static cols default constructed, literal2D assigned,
// then data matches input data
TEST_CASE
(
    "IF m is default construced\n"
    "AND THEN literal2D is assigned to m\n"
    "THEN m_data_ij == data_in_ij.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major;

    const literal2D<int64_t> data_in = {{0, 1, 2, 3, 4, 5, 6, 7},
                                     {8, 9, 10, 11, 12, 13, 14, 15},
                                     {16, 17, 18, 19, 20, 21, 22, 23},
                                     {24, 25, 26, 27, 28, 29, 30, 31},
                                     {32, 33, 34, 35, 36, 37, 38, 39},
                                     {40, 41, 42, 43, 44, 45, 46, 47}};
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    m = data_in;

    size_t i, j;
    auto din_i = data_in.begin();

    i = 0;
    for(; din_i < data_in.end(); i++, din_i++)
    {
        auto din_j = din_i->begin();
        j = 0;

        for(; din_j < din_i->end(); j++, din_j++)
        {
            REQUIRE(m(i, j) == *din_j);
        }
    }
}

// if m1 is literal2D constructed, and then m2 is copy constructed from m1
// then m2 == m1
TEST_CASE
(
    "IF m1 is literal 2D constructed\n"
    "AND THEN m2 is copy constructed from m1\n"
    "THEN exact compare m1, and m2 returns true\n"
    "THEN IF m2 is resized SUCH THAT the size of m1 != size of m2\n"
    "THEN exact compare m1 and m2 returns false.\n"
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
    constexpr size_t ncols = 6;

    using ltype = matrix_orientation::row_major;

    const literal2D<double> data_in = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                    {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                    {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                    {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                    {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                    {1.30, 1.31, 1.32, 1.33, 1.34, 1.35},
                                    {1.36, 1.37, 1.38, 1.39, 1.40, 1.41},
                                    {1.42, 1.43, 1.44, 1.45, 1.46, 1.47}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(m1);

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    size_t new_nbr_rows = 8;
    size_t new_row_reach = 10;

    m2.reshape_rows(new_nbr_rows, new_row_reach);
    
    REQUIRE(true == eh::compare2D_exact(m1, m2));
}


// if m1 is literal2D constructed, and then m2 is copy assigned from m1
// then m2 == m1
TEST_CASE
(
    "IF m1 is literal2D constructed\n"
    "AND THEN m2 is also literal2D constructed\n"
    "THEN exact compare of m1 and m2 returns false\n"
    "AND IF m2 is copy assigned from m1\n"
    "THEN exact compare of m1 and m2 returns true.\n"
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

    using ltype = matrix_orientation::row_major;

    const literal2D<double> data_in1  = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                     {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                     {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                     {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                     {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                     {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};

    const literal2D<double> data_in2 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in1);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in2);

    REQUIRE(false == eh::compare2D_exact(m1, m2));

    m2 = m1;

    REQUIRE(true == eh::compare2D_exact(m1, m2));
}

// if m1 is literal2D constructed and then m2 is move constructed from m1
// then m2 matches what m1 was
TEST_CASE
(
    "IF m1 is literal2D constructed\n"
    "AND m2 is also literal2D constructed with the same literal2D\n"
    "THEN compare exact m1, m2 returns true\n"
    "THEN IF m3 is move constructed from m2\n"
    "THEN compare exact between m1 and m3 returns true\n"
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 5]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 5;

    using ltype = matrix_orientation::col_major;

    const literal2D<double> data_in = {{ln2, -ln2, ln2, -ln2, ln2},
                                    {-ln2, ln2, -ln2, ln2, -ln2},
                                    {ln2, -ln2, ln2, -ln2, ln2}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in);

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m3(std::move(m2));

    REQUIRE(true == eh::compare2D_exact(m1, m3));
}

// if m1 is literal2D constructed and then m2 is move assigned from m1
// then m2 matches what m1 was
TEST_CASE
(
    "IF m1 is literal2D constructed\n"
    "AND m2 is also literal2D constructed with same data\n"
    "THEN compare exact m1, m2 returns true\n"
    "THEN m3 is literal2D constructed w/ different data and then move assigned from m2\n"
    "THEN compare exact m3, m1 returns true.\n"
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == 5]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::col_major;

    const literal2D<double> data_in1 = {{sqrt3, -sqrt3, sqrt3},
                                    {-sqrt3, sqrt3, -sqrt3},
                                    {sqrt3, -sqrt3, sqrt3},
                                    {-sqrt3, sqrt3, -sqrt3},
                                    {sqrt3, -sqrt3, sqrt3}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in1);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in1);

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    const literal2D<double> data_in2 = {{e, pi, e}, 
                                         {-e, -pi, -e},
                                         {pi, pi, pi}, 
                                         {e, e, e}, 
                                         {-pi, -pi, -pi}, 
                                         {-e, -e, -e},
                                         {pi, e, pi}, 
                                         {-pi, -e, -pi}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m3(data_in2);

    REQUIRE(false == eh::compare2D_exact(m1, m3));

    m3 = std::move(m2);

    REQUIRE(true == eh::compare2D_exact(m1, m3));
}
