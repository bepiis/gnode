//
//  test_storage_engine_both_static.cpp
//  Created by Ben Westcott on 11/22/24.
//

TEST_CASE
(
    "Type traits are consistent with template specialization.\n"
    "[GIVEN ST dtype double STRONG]"
    "[GIVEN ST atype std STRONG]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype row_major STRONG]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    using mse_type = matrix_storage_engine<dtype, atype, nrows, ncols, ltype>;

    REQUIRE(std::same_as<atype, mse_type::allocator_type> == true);
    REQUIRE(std::same_as<dtype, mse_type::data_type> == true);
    REQUIRE(std::same_as<dtype&, mse_type::reference> == true);
    REQUIRE(std::same_as<dtype const&, mse_type::const_reference> == true);
    REQUIRE(std::same_as<std::size_t, mse_type::index_type> == true);
    REQUIRE(std::same_as<ltype, mse_type::orientation_type> == true);
}


TEST_CASE
(
    "IF default constructed\n"
    "THEN rows() == nrows AND cols() == ncols.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 0]"
    "[GIVEN ST ncols 0]"
    "[GIVEN ST ltype row_major STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 0;
    constexpr size_t ncols = 0;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;
    REQUIRE(m.rows() == nrows);
    REQUIRE(m.cols() == ncols);
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN size() == reach() == nrows * ncols.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 1]"
    "[GIVEN ST ncols 2]"
    "[GIVEN ST ltype row_major STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = 2;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;
    REQUIRE(m.size() == (nrows * ncols));
    REQUIRE(m.reach() == (nrows * ncols));
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN rows() == nrows AND cols() == ncols.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 0]"
    "[GIVEN ST ncols 0]"
    "[GIVEN ST ltype col_major STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 0;
    constexpr size_t ncols = 0;

    using ltype = matrix_orientation::col_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;
    REQUIRE(m.rows() == nrows);
    REQUIRE(m.cols() == ncols);    
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN size() == reach() == nrows * ncols.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 1]"
    "[GIVEN ST ncols 2]"
    "[GIVEN ST ltype col_major STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = 2;

    using ltype = matrix_orientation::col_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;
    REQUIRE(m.size() == (nrows * ncols));
    REQUIRE(m.reach() == (nrows * ncols));
}


TEST_CASE
(
    "IF default constructed\n"
    "THEN filled with dtype{}.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 3]"
    "[GIVEN ST ncols 3]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    for(size_t i = 0; i < m.rows(); i++)
    {
        for(size_t j = 0; j < m.rows(); j++)
        {
            REQUIRE(m(i, j) == static_cast<dtype>(0));
        }
        
    }
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN expected row and col major storage tags.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows 3]"
    "[GIVEN ST ncols 3]"
    "[GIVEN ST ltype row_major STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;


    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;


    REQUIRE(m.is_row_major == true);
    REQUIRE(m.is_col_major == false);
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN expected static and dynamic storage tags.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows > 1 STRONG]"
    "[GIVEN ST ncols > 1 STRONG]"
    "[GIVEN ST ltype col_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::col_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(m.is_row_major == false);
    REQUIRE(m.is_col_major == true);
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN expected static and dynamic storage tags.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 1 STRONG]"
    "[GIVEN ST ncols > 1 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 1;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(m.is_static_row_vector == true);
    REQUIRE(m.is_static_col_vector == false);
    REQUIRE(m.is_row_dynamic == false);
    REQUIRE(m.is_col_dynamic == false);
    REQUIRE(m.is_fully_dynamic == false);
    REQUIRE(m.is_fully_static == true);
}

TEST_CASE
(
    "IF default constructed\n"
    "THEN expected static and dynamic storage tags.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows > 1 STRONG]"
    "[GIVEN ST ncols == 1 STRONG]"
    "[GIVEN ST ltype col_major]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 1;

    using ltype = matrix_orientation::col_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(m.is_static_row_vector == false);
    REQUIRE(m.is_static_col_vector == true);
    REQUIRE(m.is_row_dynamic == false);
    REQUIRE(m.is_col_dynamic == false);
    REQUIRE(m.is_fully_dynamic == false);
    REQUIRE(m.is_fully_static == true);
}

TEST_CASE
(
    "IF rect init list constructed\n"
    "THEN data matches input list.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 8]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 8;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<int64_t> data_in = {{-11, -10, -9},
                                              {-8, -7, -6},
                                              {-5, -4, -3},
                                              {-2, -1, 0},
                                              {1, 2, 3},
                                              {4, 5, 6},
                                              {7, 8, 9},
                                              {10, 11, 12}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m(data_in);

    size_t i, j;
    auto din_i = data_in.begin();
    
    i = 0;
    for(; i < m.rows(); i++, din_i++)
    {
        auto din_j = din_i->begin();
        j = 0;

        for(; j < m.cols(); j++, din_j++)
        {
            REQUIRE(m(i, j) == *din_j);
        }
    }
}

TEST_CASE
(
    "IF default constructed\n"
    "AND THEN rect init list assigned\n"
    "THEN data matches input list\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    const literal2D<int64_t> data_in = {{-100, 0, 90, 0, -80, 0, 70, 0},
                                             {-60, 0, 50, 0, -40, 0, 30, 0},
                                             {-20, 0, 10, 0, 0, -10, 0, 10}};

    m = data_in;

    size_t i, j;
    auto din_i = data_in.begin();

    i=0;
    for(; i < m.rows(); i++, din_i++)
    {
        auto din_j = din_i->begin();
        j = 0;

        for(; j < m.cols(); j++, din_j++)
        {
            REQUIRE(m(i, j) == *din_j);
        }
    }
}

TEST_CASE
(
    "IF m1 is rect init list constructed\n"
    "AND THEN m2 is copy constructed from m1\n"
    "THEN m2 data matches input data.\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == 8]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{

    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 8;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                             {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                             {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1(data_in);

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2(m1);

    size_t i, j;
    auto din_i = data_in.begin();

    i=0;
    for(; i < m2.rows(); i++, din_i++)
    {
        auto din_j = din_i->begin();
        j = 0;

        for(; j < m2.cols(); j++, din_j++)
        {
            REQUIRE(m2(i, j) == *din_j);
        }
    }
}

TEST_CASE
(
    "IF m1 AND m2 are default constructed\n"
    "AND THEN m1 is rect init list assigned\n"
    "AND THEN m2 is copy assigned from m1\n"
    "THEN m2 data matches input data.\n",
    "[GIVEN ST dtype int64_t]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3]"
    "[GIVEN ST ncols == 3]"
    "[GIVEN ST ltype col_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = int64_t;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::col_major_t;

    const literal2D<int64_t> data_in = {{0, 0, 0}, {3, 2, 1}, {9, -1, -100}};
    const literal2D<int64_t> data_in_cm = {{0, 3, 9}, {0, 2, -1}, {0, 1, -100}};

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m1;
    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m2;

    m1 = data_in;
    m2 = m1;

    size_t i, j;
    auto din_j = data_in_cm.begin();

    j = 0;
    for(; j < m2.cols(); j++, din_j++)
    {
        auto din_i = din_j->begin();
        i = 0;

        for(; i < m2.rows(); i++, din_i++)
        {
            REQUIRE(m2(i, j) == *din_i);
        }
    }
}

TEST_CASE
(
    "IF m is rect init list constructed\n"
    "BUT m and input data have inconsistent sizes\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3 STRONG]"
    "[GIVEN ST ncols == 3 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in =  {{sqrt2, sqrt2, sqrt2},
                                             {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                             {sqrt2, sqrt2, sqrt2},
                                             {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                             {sqrt2, sqrt2, sqrt2},
                                             {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                             {sqrt2, sqrt2, sqrt2},
                                             {1/sqrt2, 1/sqrt2, 1/sqrt2}};

    // TODO: why doesnt REQUIRE_THROWS_AS work here?
    REQUIRE_THROWS
    (
        matrix_storage_engine<dtype, atype, nrows, ncols, ltype>(data_in)
        //,std::runtime_error
    );
}

TEST_CASE
(
    "IF m is default constructed\n"
    "AND THEN m is rect init list assigned\n"
    "BUT m and input data have inconsistent sizes\n"
    "THEN runtime error is thrown.\n",
    "[GIVEN ST dtype double]"
    "[GIVEN ST atype std]"
    "[GIVEN ST nrows == 3 STRONG]"
    "[GIVEN ST ncols == 3 STRONG]"
    "[GIVEN ST ltype row_major]"
    "[GIVEN ST data_in STRONG]"
    "[core/storage_engine::matrix_storage_engine]"
)
{
    using dtype = double;
    using atype = std::allocator<dtype>;

    constexpr size_t nrows = 3;
    constexpr size_t ncols = 3;

    using ltype = matrix_orientation::row_major_t;

    const literal2D<double> data_in =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                             {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                             {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};  

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE_THROWS
    (
        m = data_in
        //,std::runtime_error
    );
}