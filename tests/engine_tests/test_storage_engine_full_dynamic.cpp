//
//  test_storage_engine_full_dynamic.cpp
//  Created by Ben Westcott on 11/23/24.
//

// TODO: move, copy constructor tests

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and dynamic rows\n"
    "THEN it is an owning engine\n"
    "THEN it has engine base types\n"
    "THEN it has mutable access\n"
    "THEN it has immutable access\n"
    "THEN it has a consistent mutable reference type\n"
    "THEN it has a consistent immutable reference type\n"
    "THEN it has consistent return sizes\n"
    "THEN it has consistent return lengths\n"
    "THEN it is reshapeable\n"
    "THEN it is row reshapeable\n"
    "THEN it is reshapeable\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
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

    REQUIRE(true == reshapeable_engine<M>);
    REQUIRE(true == row_reshapeable_engine<M>);
    REQUIRE(true == col_reshapeable_engine<M>);

}

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and dynamic rows\n"
    "THEN it is a readable engine\n"
    "THEN it is a writable engine\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == readable_engine<M>);
    REQUIRE(true == writable_engine<M>);
}

TEST_CASE
(
    "IF M is a storage engine with dynamic cols and dynamic rows\n"
    "THEN it is copy constructible\n"
    "THEN it is move constructible\n"
    "THEN it is default constructible\n"
    "THEN it is destructable\n"
    "THEN it is size_t, size_t constructible\n"
    "THEN it is size_t, size_t, size_t, size_t constructible\n"
   // "THEN it is copy constructible from another engine type\n"
   // "THEN it is copy assignable from another engine type\n"
    "THEN it is constructible from a literal2D\n"
    "THEN it is assignable from a literal2D\n"
    "THEN it is nothrow swappable.\n"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    using M = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(true == std::is_copy_constructible_v<M>);
    REQUIRE(true == std::is_move_constructible_v<M>);
    REQUIRE(true == std::is_default_constructible_v<M>);
    REQUIRE(true == std::is_destructible_v<M>);
    REQUIRE(true == std::constructible_from<M, size_t, size_t>);
    REQUIRE(true == std::constructible_from<M, size_t, size_t, size_t, size_t>);
    REQUIRE(true == std::constructible_from<M, literal2D<dtype>>);
    REQUIRE(true == std::assignable_from<M&, literal2D<dtype>>);
    REQUIRE(true == std::is_nothrow_swappable_v<M&>);
}

TEST_CASE
(
    "IF m is default constructed with dynamic rows and dynamic cols\n"
    "THEN storage flags set consistently"
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major, col_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype1 = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype1> m1;

    REQUIRE(true == m1.is_row_major);
    REQUIRE(false == m1.is_col_major);
    REQUIRE(false == m1.is_static_row_vector);
    REQUIRE(false == m1.is_static_col_vector);
    REQUIRE(true == m1.is_row_dynamic);
    REQUIRE(true == m1.is_col_dynamic);
    REQUIRE(true == m1.is_fully_dynamic);
    REQUIRE(false == m1.is_fully_static);

    using ltype2 = matrix_orientation::col_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype2> m2;

    REQUIRE(false == m2.is_row_major);
    REQUIRE(true == m2.is_col_major);
    REQUIRE(false == m2.is_static_row_vector);
    REQUIRE(false == m2.is_static_col_vector);
    REQUIRE(true == m2.is_row_dynamic);
    REQUIRE(true == m2.is_col_dynamic);
    REQUIRE(true == m2.is_fully_dynamic);
    REQUIRE(false == m2.is_fully_static);
}

// if m in default constructed, and reshape is called with invalid new_nbr_rows
// then runtime_error is thrown
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape is called SUCH THAT new_nbr_rows is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == -1 STRONG]"
    "[GIVEN DYN new_row_reach == 2]"
    "[GIVEN DYN new_nbr_cols == 2]"
    "[GIVEN DYN new_col_reach == 3]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = -1;
    int64_t new_row_reach = 2;
    int64_t new_nbr_cols = 2;
    int64_t new_col_reach = 3;

    REQUIRE_THROWS
    (
        m.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach)
    );

}

// if m is default constructed and reshape is called with invalid new_row_reach
// then runtime_error is thrown
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape is called SUCH THAT new_row_reach is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == 2]"
    "[GIVEN DYN new_row_reach == -1 STRONG]"
    "[GIVEN DYN new_nbr_cols == 2]"
    "[GIVEN DYN new_col_reach == 3]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = 2;
    int64_t new_row_reach = -1;
    int64_t new_nbr_cols = 2;
    int64_t new_col_reach = 3;

    REQUIRE_THROWS
    (
        m.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach)
    );
}

// if m is default constructed, and reshape is called with invalid new_nbr_cols
// then runtime_error is thrown
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape is called SUCH THAT new_nbr_cols is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == 2]"
    "[GIVEN DYN new_row_reach == 2]"
    "[GIVEN DYN new_nbr_cols == -1 STRONG]"
    "[GIVEN DYN new_col_reach == 3]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = 2;
    int64_t new_row_reach = 2;
    int64_t new_nbr_cols = -1;
    int64_t new_col_reach = 3;

    REQUIRE_THROWS
    (
        m.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach)
    );
}



// if m is default constructed and reshape is called with invalid new_col_reach
// then runtime_error is thrown
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape is called SUCH THAT new_col_reach is invalid\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN new_nbr_rows == 2]"
    "[GIVEN DYN new_row_reach == 2]"
    "[GIVEN DYN new_nbr_cols == 3]"
    "[GIVEN DYN new_col_reach == -1 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    int64_t new_nbr_rows = 2;
    int64_t new_row_reach = 2;
    int64_t new_nbr_cols = 3;
    int64_t new_col_reach = -1;

    REQUIRE_THROWS
    (
        m.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach)
    );
}

// if m is default constructed, 
// then 0 == cols() == col_reach() and 0 == rows() == row_reach()
TEST_CASE
(
    "IF m is default constructed\n"
    "THEN\n"
    "\t0 == cols() == col_reach()\n"
    "\tAND 0 == rows() == row_reach()\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(0 == m.rows());
    REQUIRE(0 == m.row_reach());
    REQUIRE(0 == m.cols());
    REQUIRE(0 == m.col_reach());
}

// if m is default constructed
// then storage flags set consistently
TEST_CASE
(
    "IF m is default constructed\n"
    "THEN storage tags set consistently.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent STRONG]"
    "[GIVEN ST ncols == dyn_extent STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(true == m.is_row_major);
    REQUIRE(false == m.is_col_major);
    REQUIRE(false == m.is_static_row_vector);
    REQUIRE(false == m.is_static_col_vector);
    REQUIRE(true == m.is_row_dynamic);
    REQUIRE(true == m.is_col_dynamic);
    REQUIRE(true == m.is_fully_dynamic);
    REQUIRE(false == m.is_fully_static);
}

// if m is dimension constructed, then if new_nbr_rows > row_reach(), then 
// new_nbr_rows == rows(), new_nbr_rows == row_reach(), nbr_cols == cols(), col_reach == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_nbr_rows > row_reach()\n"
    "THEN\n"
    "\tnew_nbr_rows == rows()\n"
    "\tAND new_nbr_rows == row_reach()\n"
    "\tAND nbr_cols == cols()\n"
    "\tAND col_reach == col_reach.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN nbr_cols == 3]"
    "[GIVEN DYN row_reach == 5]"
    "[GIVEN DYN col_reach == 5]"
    "[GIVEN DYN new_nbr_rows == 6]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 5;
    size_t col_reach = 5;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 6;

    m.reshape(new_nbr_rows, row_reach, nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(new_nbr_rows == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * nbr_cols == m.size());
    REQUIRE(new_nbr_rows * col_reach == m.reach());
}

// if m is dimension constructed then if new_row_reach != row_reach(), then
// nbr_rows == rows(), new_row_reach == row_reach(), nbr_cols == cols() and col_reach == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_row_reach != row_reach()\n"
    "THEN\n"
    "\tnbr_rows == rows()\n"
    "\tAND new_row_reach == row_reach\n"
    "\tAND nbr_cols == cols()\n"
    "\tAND col_reach == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN nbr_cols == 3]"
    "[GIVEN DYN row_reach == 5 STRONG]"
    "[GIVEN DYN col_reach == 5]"
    "[GIVEN DYN new_row_reach == 10 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 5;
    size_t col_reach = 5;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_row_reach = 10;

    m.reshape(nbr_rows, new_row_reach, nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(nbr_rows == m.rows());
    REQUIRE(new_row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(nbr_rows * nbr_cols == m.size());
    REQUIRE(new_row_reach * col_reach == m.reach());
}

// if m is dimension construced then if new_row_reach < row_reach() and new_nbr_rows > row_reach() then,
// new_nbr_rows == rows() == row_reach() and nbr_cols == cols() == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_row_reach < row_reach AND new_nbr_rows > row_reach()\n"
    "THEN\n"
    "\tnew_nbr_rows == rows() == row_reach()\n"
    "\tnbr_cols == cols()\n"
    "\tAND col_reach == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN nbr_cols == 3]"
    "[GIVEN DYN row_reach == 5 STRONG]"
    "[GIVEN DYN col_reach == 5]"
    "[GIVEN DYN new_nbr_rows == 6 STRONG]"
    "[GIVEN DYN new_row_reach == 3 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 5;
    size_t col_reach = 5;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 6;
    size_t new_row_reach = 3;

    m.reshape(new_nbr_rows, new_row_reach, nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(new_nbr_rows == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * nbr_cols == m.size());
    REQUIRE(new_nbr_rows * col_reach == m.reach());
}

// if m is dimension constructed, then if new_nbr_cols > col_reach(), then 
// nbr_rows == rows(), row_reach == row_reach(), new_nbr_cols == cols(), new_nbr_cols == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_nbr_cols > col_reach()\n"
    "THEN\n"
    "\tnbr_rows == rows()\n"
    "\tAND row_reach == row_reach()\n"
    "\tAND new_nbr_cols == col()\n"
    "\tAND new_nbr_cols == col_reach.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN row_reach == 7]"
    "[GIVEN DYN col_reach == 6 STRONG]"
    "[GIVEN DYN new_nbr_cols == 12 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 7;
    size_t col_reach = 6;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_cols = 12;

    m.reshape(nbr_rows, row_reach, new_nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(new_nbr_cols == m.col_reach());

    REQUIRE(nbr_rows * new_nbr_cols == m.size());
    REQUIRE(row_reach * new_nbr_cols == m.reach());
}

// if m is dimension constructed then if new_col_reach != col_reach(), then
// nbr_rows == rows(), row_reach == row_reach(), nbr_cols == cols() and new_col_reach == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_col_reach != col_reach()\n"
    "THEN\n"
    "\tnbr_rows == rows()\n"
    "\trow_reach == row_reach()\n"
    "\tnbr_cols == cols()\n"
    "\tnew_col_reach == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN row_reach == 8]"
    "[GIVEN DYN col_reach == 8]"
    "[GIVEN DYN new_col_reach == 9]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 8;
    size_t col_reach = 8;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_col_reach = 9;

    m.reshape(nbr_rows, row_reach, nbr_cols, new_col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(nbr_cols == m.cols());
    REQUIRE(new_col_reach == m.col_reach());

    REQUIRE(nbr_rows * nbr_cols == m.size());
    REQUIRE(row_reach * new_col_reach == m.reach());
}

// if m is dimension construced then if new_col_reach < col_reach() and new_nbr_cols > col_reach() then,
// nbr_rows == rows() == row_reach() and new_nbr_cols == cols() == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_col_reach < col_reach()\n"
    "\tAND new_nbr_cols > col_reach\n"
    "THEN\n"
    "\tnbr_rows == rows()\n"
    "\trow_reach == row_reach()\n"
    "\tAND new_nbr_cols == cols() == col_reach.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN row_reach == 20]"
    "[GIVEN DYN col_reach == 30 STRONG]"
    "[GIVEN DYN new_col_reach == 20 STRONG]"
    "[GIVEN DYN new_nbr_cols == 40 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 20;
    size_t col_reach = 30;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_col_reach = 20;
    size_t new_nbr_cols = 40;

    m.reshape(nbr_rows, row_reach, new_nbr_cols, new_col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(new_nbr_cols == m.col_reach());

    REQUIRE(nbr_rows * new_nbr_cols == m.size());
    REQUIRE(row_reach * new_nbr_cols == m.reach());
}

// if m is dimension constructed then if new_row_reach > new_nbr_rows, and new_col_reach > new_nbr_cols then
// new_nbr_rows == rows(), new_row_reach == row_reach(), new_nbr_cols() == cols() and new_col_reach == col_reach()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_row_reach > new_nbr_rows\n"
    "\tAND new_col_reach > new_nbr_cols\n"
    "THEN\n"
    "\tnew_nbr_rows == rows()\n"
    "\tAND new_row_reach == row_reach()\n"
    "\tAND new_nbr_cols == cols()\n"
    "\tAND new_col_reach == col_reach().\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN row_reach == 5 STRONG]"
    "[GIVEN DYN col_reach == 5 STRONG]"
    "[GIVEN DYN new_nbr_rows == 15 STRONG]"
    "[GIVEN DYN new_nbr_cols == 17 STRONG]"
    "[GIVEN DYN new_row_reach == 20 STRONG]"
    "[GIVEN DYN new_col_reach == 18 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 5;
    size_t col_reach = 5;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 15;
    size_t new_nbr_cols = 17;
    size_t new_row_reach = 20;
    size_t new_col_reach = 18;

    m.reshape(new_nbr_rows, new_row_reach, new_nbr_cols, new_col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(new_row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(new_col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * new_nbr_cols == m.size());
    REQUIRE(new_row_reach * new_col_reach == m.reach());

}

// if m is dimension constructed then if new_nbr_rows <= row_reach, new_nbr_cols <= col_reach, new_row_reach == row_reach()
// and new_col_reach == col_reach() and new_nbr_rows < rows() and new_nbr_cols < cols()
// then row_reach == row_reach(), col_reach == col_reach(), new_nbr_rows == rows() and new_nbr_cols == cols()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_nbr_rows <= row_reach()\n"
    "\tAND new_nbr_cols <= col_reach()\n"
    "\tAND new_row_reach == row_reach()\n"
    "\tAND new_col_reach == col_reach()\n"
    "\tAND new_nbr_rows < rows()\n"
    "\tAND new_nbr_cols < cols().\n"
    "THEN\n"
    "\trow_reach == row_reach()\n"
    "\tAND col_reach == col_reach()\n"
    "\tAND new_nbr_rows == rows()\n"
    "\tAND new_nbr_cols == cols()\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 9 STRONG]"
    "[GIVEN DYN nbr_cols == 18 STRONG]"
    "[GIVEN DYN row_reach == new_row_reach == 10 STRONG]"
    "[GIVEN DYN col_reach == new_col_reach == 20 STRONG]"
    "[GIVEN new_nbr_rows == 8 STRONG]"
    "[GIVEN new_nbr_cols == 16 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 9;
    size_t nbr_cols = 18;
    size_t row_reach = 10;
    size_t col_reach = 20;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 8;
    size_t new_nbr_cols = 16;

    m.reshape(new_nbr_rows, row_reach, new_nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * new_nbr_cols == m.size());
    REQUIRE(row_reach * col_reach == m.reach());
}

// if m is dimension constructed then if new_nbr_rows <= row_reach, new_nbr_cols <= col_reach, new_row_reach == row_reach()
// and new_col_reach == col_reach() and new_nbr_rows > rows() and new_nbr_cols > cols()
// then row_reach == row_reach(), col_reach == col_reach(), new_nbr_rows == rows() and new_nbr_cols == cols()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_nbr_rows <= row_reach()\n"
    "\tAND new_nbr_cols <= col_reach()\n"
    "\tAND new_row_reach == row_reach()\n"
    "\tAND new_col_reach == col_reach()\n"
    "\tAND new_nbr_rows > rows()\n"
    "\tAND new_nbr_cols > cols().\n"
    "THEN\n"
    "\trow_reach == row_reach()\n"
    "\tAND col_reach == col_reach()\n"
    "\tAND new_nbr_rows == rows()\n"
    "\tAND new_nbr_cols == cols()\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 3 STRONG]"
    "[GIVEN DYN nbr_cols == 3 STRONG]"
    "[GIVEN DYN row_reach == new_row_reach == 10 STRONG]"
    "[GIVEN DYN col_reach == new_col_reach == 20 STRONG]"
    "[GIVEN new_nbr_rows == 10 STRONG]"
    "[GIVEN new_nbr_cols == 20 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;
    
    size_t nbr_rows = 3;
    size_t nbr_cols = 3;
    size_t row_reach = 10;
    size_t col_reach = 20;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 10;
    size_t new_nbr_cols = 20;

    m.reshape(new_nbr_rows, row_reach, new_nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * new_nbr_cols == m.size());
    REQUIRE(row_reach * col_reach == m.reach());
}

// if m is dimension constructed then if new_nbr_rows <= row_reach, new_nbr_cols <= col_reach, new_row_reach == row_reach()
// and new_col_reach == col_reach() and new_nbr_rows == rows() and new_nbr_cols == cols()
// then row_reach == row_reach(), col_reach == col_reach(), nbr_rows == rows() and nbr_cols == cols()
TEST_CASE
(
    "IF m is dimension constructed\n"
    "AND THEN reshape is called SUCH THAT\n"
    "\tnew_nbr_rows <= row_reach()\n"
    "\tAND new_nbr_cols <= col_reach()\n"
    "\tAND new_row_reach == row_reach()\n"
    "\tAND new_col_reach == col_reach()\n"
    "\tAND new_nbr_rows == rows()\n"
    "\tAND new_nbr_cols == cols().\n"
    "THEN\n"
    "\trow_reach == row_reach()\n"
    "\tAND col_reach == col_reach()\n"
    "\tAND new_nbr_rows == rows()\n"
    "\tAND new_nbr_cols == cols()\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN DYN nbr_rows == 10 STRONG]"
    "[GIVEN DYN nbr_cols == 15 STRONG]"
    "[GIVEN DYN row_reach == new_row_reach == 10 STRONG]"
    "[GIVEN DYN col_reach == new_col_reach == 20 STRONG]"
    "[GIVEN new_nbr_rows == 10 STRONG]"
    "[GIVEN new_nbr_cols == 15 STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    size_t nbr_rows = 10;
    size_t nbr_cols = 15;
    size_t row_reach = 10;
    size_t col_reach = 20;
    
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(nbr_rows, row_reach, nbr_cols, col_reach);

    size_t new_nbr_rows = 10;
    size_t new_nbr_cols = 15;

    m.reshape(new_nbr_rows, row_reach, new_nbr_cols, col_reach);

    REQUIRE(m.rows() <= m.row_reach());
    REQUIRE(new_nbr_rows == m.rows());
    REQUIRE(row_reach == m.row_reach());

    REQUIRE(m.cols() <= m.col_reach());
    REQUIRE(new_nbr_cols == m.cols());
    REQUIRE(col_reach == m.col_reach());

    REQUIRE(new_nbr_rows * new_nbr_cols == m.size());
    REQUIRE(row_reach * col_reach == m.reach());
}

// if m is literal2D constructed
/// then rows(), row_reach(), cols() and col_reach matches input data rows, cols
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() == rows() == row_reach() AND literal2D.begin()->size() == cols() == col_reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<double>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                          {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                          {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};

    size_t nbr_din_rows = data_in.size();
    size_t nbr_din_cols = data_in.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(data_in);

    REQUIRE(nbr_din_rows == m.rows());
    REQUIRE(nbr_din_rows == m.row_reach());
    REQUIRE(nbr_din_cols == m.cols());
    REQUIRE(nbr_din_cols == m.col_reach());
}

// if m is literal2D constructed
// then size() and reach() consistent with input data size
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN literal2D.size() * literal2D.begin()->size() == size() = reach().\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<double>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in  = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                     {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                     {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                     {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                     {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                     {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};

    size_t nbr_din_rows = data_in.size();
    size_t nbr_din_cols = data_in.begin()->size();

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(data_in);

    REQUIRE(nbr_din_rows * nbr_din_cols == m.size());
    REQUIRE(nbr_din_rows * nbr_din_cols == m.reach());
}

// if m is literal2D constructed
// then m_data matches data_in
TEST_CASE
(
    "IF m is literal2D constructed\n"
    "THEN m_data_ij = data_in_ij.\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<double>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                    {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                    {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                    {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                    {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                    {1.30, 1.31, 1.32, 1.33, 1.34, 1.35},
                                    {1.36, 1.37, 1.38, 1.39, 1.40, 1.41},
                                    {1.42, 1.43, 1.44, 1.45, 1.46, 1.47}};

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

// if m is default constructed and then literal2D assigned
// then m_data matches data_in
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN m is literal2D assigned\n"
    "THEN m_data_ij == data_in_ij.\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == dyn_extent]"
    "[GIVEN ST ncols == dyn_extent]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<double>;

    constexpr size_t nrows = std::dynamic_extent;
    constexpr size_t ncols = std::dynamic_extent;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in = {{1.00, 1.06, 1.12, 1.18, 1.24, 1.30},
                                     {1.01, 1.07, 1.13, 1.19, 1.25, 1.31},
                                     {1.02, 1.08, 1.14, 1.20, 1.26, 1.32},
                                     {1.03, 1.09, 1.15, 1.21, 1.27, 1.33},
                                     {1.04, 1.10, 1.16, 1.22, 1.28, 1.34},
                                     {1.05, 1.11, 1.17, 1.23, 1.29, 1.35},
                                     {-1.00, -2.00, -3.00, -4.00, -5.00, -6.00},
                                     {-1.10, -2.20, -3.30, -4.40, -5.50, -6.60}};

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
