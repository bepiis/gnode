//
//  test_storage_engine_full_dynamic.cpp
//  Created by Ben Westcott on 11/23/24.
//

// if m in default constructed, and reshape is called with invalid new_nbr_rows
// then runtime_error is thrown
TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN reshape is called SUCH THAT new_nbr_rows is invalid\n"
    "THEN runtime error is thrown.\n"
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
    "THEN runtime error is thrown.\n"
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
    "THEN runtime error is thrown.\n"
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
    "THEN runtime error is thrown.\n"
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
    "\tAND 0 == rows() == row_reach()\n"
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
    "THEN storage tags set consistently.\n"
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
    "\tAND col_reach == col_reach.\n"
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
    "\tAND col_reach == col_reach().\n"
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
    "\tAND col_reach == col_reach().\n"
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
    "\tAND new_nbr_cols == col_reach.\n"
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
    "\tnew_col_reach == col_reach().\n"
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
    "\tAND new_nbr_cols == cols() == col_reach.\n"
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
    "\tAND new_col_reach == col_reach()"
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
    "\tAND new_nbr_cols == cols()\n"
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
    "\tAND new_nbr_cols == cols()\n"
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
    "\tAND new_nbr_cols == cols()\n"
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
}