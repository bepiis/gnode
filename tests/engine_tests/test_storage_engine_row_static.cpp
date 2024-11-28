//
//  test_storage_engine_row_static.cpp
//  Created by Ben Westcott on 11/22/24.
//

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and static rows\n"
    "THEN it is an owning engine\n"
    "THEN it has engine base types\n"
    "THEN it has mutable access\n"
    "THEN it has immutable access\n"
    "THEN it has a consistent mutable reference type\n"
    "THEN it has a consistent immutable reference type\n"
    "THEN it has consistent return sizes\n"
    "THEN it has consistent return lengths\n"
    "THEN it is col reshapeable\n"
    "THEN it is NOT row reshapeable\n"
    "THEN it is NOT reshapeable\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 8]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 8;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

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
    REQUIRE(false == row_reshapeable_engine<M>);
    REQUIRE(true == col_reshapeable_engine<M>);
}

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and static rows\n"
    "THEN it is a readable engine\n"
    "THEN it is a writable engine\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 8]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 8;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
}

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and static rows\n"
    "THEN it is copyable\n"
    "THEN it is copy constructible\n"
    "THEN it is move constructible\n"
    "THEN it is default constructible\n"
    "THEN it is destructable\n"
    "THEN it is size_t, size_t constructible\n"
   // "THEN it is copy constructible from another engine type\n"
   // "THEN it is copy assignable from another engine type\n"
    "THEN it is constructible from a literal2D\n"
    "THEN it is assignable from a literal2D\n"
    "THEN it is nothrow swappable.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 8]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 8;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

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
    "AND THEN reshape_cols is called SUCH THAT new_nbr_cols is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_cols == -1 STRONG]"
    "[GIVEN DYN new_col_reach == 3]"
    "[core/storage_engine::matrix_storage_engine]"

)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_cols = -1;
    int64_t new_col_reach = 3;

    REQUIRE_THROWS(m.reshape_cols(new_nbr_cols, new_col_reach));
}

TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape_cols is called SUCH THAT new_col_reach is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_cols == 3]"
    "[GIVEN DYN new_col_reach == -5 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_cols = 3;
    int64_t new_col_reach = -5;

    REQUIRE_THROWS(m.reshape_cols(new_nbr_cols, new_col_reach));
}

TEST_CASE
(
    "IF m is default constructed\n"
    "THEN\n"
    "\t0 == cols() == col_reach()\n"
    "\tAND nrows == rows() == row_reach().\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());
    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(0 == m.cols());
    REQUIRE(0 == m.col_reach());
}

TEST_CASE
(
    "IF m is default constructed\n"
    "THEN storage flags set consistently.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 1,3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t ncols = std::dynamic_extent;
    constexpr size_t nrows1 = 1;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows1, ncols, ltype> m1;

    REQUIRE(true == m1.is_row_major);
    REQUIRE(false == m1.is_col_major);
    REQUIRE(true == m1.is_static_row_vector);
    REQUIRE(false == m1.is_static_col_vector);
    REQUIRE(false == m1.is_row_dynamic);
    REQUIRE(true == m1.is_col_dynamic);
    REQUIRE(false == m1.is_fully_dynamic);
    REQUIRE(false == m1.is_fully_static);

    constexpr size_t nrows2 = 3;

    matrix_storage_engine<dtype, atype, nrows2, ncols, ltype> m2;

    REQUIRE(true == m2.is_row_major);
    REQUIRE(false == m2.is_col_major);
    REQUIRE(false == m2.is_static_row_vector);
    REQUIRE(false == m2.is_static_col_vector);
    REQUIRE(false == m2.is_row_dynamic);
    REQUIRE(true == m2.is_col_dynamic);
    REQUIRE(false == m2.is_fully_dynamic);
    REQUIRE(false == m2.is_fully_static);
}

TEST_CASE
(
    "IF m is nbr_cols, col_reach constructed\n"
    "AND THEN reshape_cols is called SUCH THAT new_nbr_cols > col_reach()\n"
    "THEN\n"
    "\tnew_nbr_cols == cols()\n"
    "\tAND col_reach == col_reach()\n"
    "\tAND cols() <= col_reach()\n"
    "\tAND nrows == rows()\n"
    "\tAND nrows == row_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN col_reach == 3 STRONG]"
    "[GIVEN DYN new_nbr_cols == 5 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;


    using ltype = matrix_orientation::row_major_t;

    size_t nbr_cols = 3;
    size_t col_reach = 3;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_cols, col_reach);

    size_t new_nbr_cols = 5;    

    m.reshape_cols(new_nbr_cols, col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(new_nbr_cols == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());

    new_nbr_cols = 10;
    size_t new_col_reach = 15;

    m.reshape_cols(new_nbr_cols, new_col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(new_col_reach == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());

}

TEST_CASE
(
    "IF m is nbr_cols, col_reach constructed\n"
    "AND THEN reshape_cols is called SUCH THAT new_col_reach != col_reach()\n"
    "THEN\n"
    "\tnbr_cols == cols()\n"
    "\tAND new_col_reach == col_reach()\n"
    "\tAND cols() <= col_reach()\n"
    "\tAND nrows == rows()\n"
    "\tAND nrows == row_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN col_reach == 6 STRONG]"
    "[GIVEN DYN new_col_reach = 10 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_cols = 3;
    size_t col_reach = 6;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_cols, col_reach);

    size_t new_col_reach = 10;
    m.reshape_cols(nbr_cols, new_col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(nbr_cols == m.cols());
    REQUIRE(new_col_reach == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());

}

TEST_CASE
(
    "IF m is nbr_cols, cols_reach constructed\n"
    "AND THEN reshape_cols is called SUCH THAT new_nbr_cols <= col_reach() AND new_col_reach == col_reach()\n"
    "THEN\n"
    "\tnew_nbr_cols == cols()\n"
    "\tAND new_col_reach == col_reach == col_reach()\n"
    "\tAND cols() <= col_reach()\n"
    "\tAND nrows == rows()\n"
    "\tAND nrow == row_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN col_reach == 6 STRONG]"
    "[GIVEN DYN new_nbr_cols = 5 STRONG]"
    "[GIVEN DYN new_col_reach = 6 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 6;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_cols = 3;
    size_t col_reach = 6;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_cols, col_reach);

    size_t new_nbr_cols = 5;

    m.reshape_cols(new_nbr_cols, col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());

    new_nbr_cols = 2;

    m.reshape_cols(new_nbr_cols, col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());

    new_nbr_cols = 2;

    m.reshape_cols(new_nbr_cols, col_reach);

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());
    REQUIRE(nrows == m.rows());
    REQUIRE(nrows == m.row_reach());
    
}

// static rows, dynamic cols literal2D constructed but nrows != literal2D.size(),
// then runtime_error is thrown.
TEST_CASE
(
    "IF m is literal2D constructed BUT nrows != literal2D.size()\n"
    "THEN runtime_error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype col_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 6;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

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

// static rows, dynamic cols literal2D constructed,
// then rows(), cols(), row_reach(), col_reach() matches input data rows, cols
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() == rows() == row_reach() AND literal2D.begin()->size() == cols() == col_reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in_1 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    size_t nbr_din_cols_1 = data_in_1.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in_1);

    REQUIRE(nrows == m1.rows());
    REQUIRE(nrows == m1.row_reach());
    REQUIRE(nbr_din_cols_1 == m1.cols());
    REQUIRE(nbr_din_cols_1 == m1.col_reach());

    const literal2D<double> data_in_2 = {{ln2, -ln2, ln2, -ln2, ln2},
                                         {-ln2, ln2, -ln2, ln2, -ln2},
                                         {ln2, -ln2, ln2, -ln2, ln2}};

    size_t nbr_din_cols_2 = data_in_2.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in_2);

    REQUIRE(nrows == m2.rows());
    REQUIRE(nrows == m2.row_reach());
    REQUIRE(nbr_din_cols_2 == m2.cols());
    REQUIRE(nbr_din_cols_2 == m2.col_reach());
}

// static rows, dynamic cols literal2D constructed,
// then size() and reach() consistent with input data size
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() * literal2D.begin()->size() == size() == reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in_1 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    size_t nbr_din_rows_1 = data_in_1.size();
    size_t nbr_din_cols_1 = data_in_1.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in_1);

    REQUIRE(nbr_din_rows_1 * nbr_din_cols_1 == m1.size());
    REQUIRE(nbr_din_rows_1 * nbr_din_cols_1 == m1.reach());

    const literal2D<double> data_in_2 = {{ln2, -ln2, ln2, -ln2, ln2},
                                         {-ln2, ln2, -ln2, ln2, -ln2},
                                         {ln2, -ln2, ln2, -ln2, ln2}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in_2);


    size_t nbr_din_rows_2 = data_in_2.size();
    size_t nbr_din_cols_2 = data_in_2.begin()->size();

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
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 6;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;


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
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extents STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 6;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

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

// if m1 is literal2D constructed, and then m2 is also literal2D constructed w/ same data
// then compare exact m1, m2 returns true
// then if m3 is move constructed from m2
// then compare exact m1, m3 returns true
TEST_CASE
(
    "IF m1 is literal2D constructed\n"
    "AND THEN m2 is also literal2D constructed from same data_in\n"
    "THEN compare exact m1, m2 returns TRUE\n"
    "THEN IF m3 is move constructed from m2\n"
    "THEN compare exact m1, m3 returns TRUE.\n"
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extents STRONG]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 6;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    const literal2D<double> data_in  = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                     {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                     {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                     {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                     {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                     {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in);

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m3(std::move(m2));

    REQUIRE(true == eh::compare2D_exact(m1, m3));
}


// if m1 is literal2D constructed, and then m2 is also literal2D constructed w/ different data
// then compare exact returns false
// and then if m2 is copy assigned from m1
// then compare exact returns true
// then if m2 is reshaped
// then compare exact m1, m2 returns false
TEST_CASE
(
    "IF m1 is literal2D constructed\n"
    "AND THEN m2 is literal2D constructed with different data_in\n"
    "THEN compare exact m1, m2 returns FALSE\n"
    "THEN if m2 is copy assigned from m1\n"
    "THEN compare exact m1, m2 returns TRUE\n"
    "THEN if m2 is reshaped SUCH THAT m1.size() != m2.size()\n"
    "THEN compare exact m1, m2 returns FALSE.\n"
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == dyn_extents STRONG]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    const literal2D<double> data_in1 =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                          {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                          {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};  

    const literal2D<double> data_in2 = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in1);
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(data_in2);

    REQUIRE(false == eh::compare2D_exact(m1, m2));

    m2 = m1;

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    size_t new_nbr_cols = 7;

    m2.reshape_cols(new_nbr_cols, new_nbr_cols);

    REQUIRE(false == eh::compare2D_exact(m1, m2));
}

// if m1 is default constructed, and then copy assigned from a literal2D
// then m2 is copy constructed from m1
// then compare exact returns true
// then if m3 is move assigned from m2
// then compare exact m1, m3 returns true
TEST_CASE
(
    "IF m1 is default constructed\n"
    "AND THEN m1 is copy assigned from a literal2D\n"
    "AND THEN m2 is copy constructed from m1\n"
    "THEN compare exact m1, m2 returns TRUE\n"
    "THEN IF m3 is move assigned from m2\n"
    "THEN compare exact m1, m3 returns TRUE.\n"
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 6]"
    "[GIVEN ST ncols == dyn_extents STRONG]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 8;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::col_major_t;

    const literal2D<double> data_in = {{sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1;

    m1 = data_in;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(m1);

    REQUIRE(true == eh::compare2D_exact(m1, m2));

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m3;

    m3 = std::move(m2);

    REQUIRE(true == eh::compare2D_exact(m1, m3));
}



