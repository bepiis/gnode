//
//  test_engine.cpp
//  Created by Ben Westcott on 9/8/24.
//

#include "core/engine.h"
#include <numbers>

/*
 * GNode roadmap:
 *  - base engine and storage_engine unit tests
 *  - simple banded_storage_engine + unit tests
 *      - handles sparse matrices with a main diagonal w/ lower and upper bandwidth
 *  - mutable and immutable view_engines + unit tests
 *      - negation
 *      - hermitian
 *      - conjugate
 *      - transpose
 *      - sub matrix
 *           - householder
 *      - row and col views 
 *           - givens views
 * 
 * 
 * 
 * 
 * 
 */

using namespace std::numbers;
using std::size_t;
using eh = engine_helper;

template<typename T>
using rect_init_list = std::initializer_list<std::initializer_list<T>>;

template<typename T>
using init_list = std::initializer_list<T>;

const rect_init_list<int64_t> ex_1x1i = {{2}};
const rect_init_list<int64_t> ex_1x2i = {{2, 3}};
const rect_init_list<int64_t> ex_1x4i = {{9, 8, 7, 6}};
const rect_init_list<int64_t> ex_2x1i = {{2}, {3}};
const rect_init_list<int64_t> ex_4x1i = {{1},{2},{3},{4}};

const rect_init_list<int64_t> ex_3x3ia = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
const rect_init_list<int64_t> ex_3x3ib = {{0, 0, 0}, {3, 2, 1}, {9, -1, -100}};

const rect_init_list<int64_t> ex_3x3ic = {{INT64_MAX, -INT64_MAX, INT64_MAX}, 
                                          {-INT64_MAX, -INT64_MAX, -INT64_MAX}, 
                                          {INT64_MAX, INT64_MAX, INT64_MAX}};

const rect_init_list<int64_t> ex_boundsi = ex_3x3ic;
const rect_init_list<int64_t> ex_invalidi = {{2, 3, 3}, {2, 1, 2}, {2, 1}};

const rect_init_list<int64_t> ex_8x3ia = {{1, 2, 3}, 
                                          {4, 5, 6}, 
                                          {7, 8, 9}, 
                                          {10, 11, 12},
                                          {13, 14, 15},
                                          {16, 17, 18},
                                          {19, 20, 21},
                                          {22, 23, 24}};

const rect_init_list<int64_t> ex_8x3ib = {{-11, -10, -9},
                                          {-8, -7, -6},
                                          {-5, -4, -3},
                                          {-2, -1, 0},
                                          {1, 2, 3},
                                          {4, 5, 6},
                                          {7, 8, 9},
                                          {10, 11, 12}};

const rect_init_list<double> ex_8x3da = {{e, pi, e}, 
                                         {-e, -pi, -e},
                                         {pi, pi, pi}, 
                                         {e, e, e}, 
                                         {-pi, -pi, -pi}, 
                                         {-e, -e, -e},
                                         {pi, e, pi}, 
                                         {-pi, -e, -pi}};

const rect_init_list<double> ex_8x3db = {{sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2}};

const rect_init_list<int64_t> ex_3x8ia = {{-100, 0, 90, 0, -80, 0, 70, 0},
                                          {-60, 0, 50, 0, -40, 0, 30, 0},
                                          {-20, 0, 10, 0, 0, -10, 0, 10}};

const rect_init_list<int64_t> ex_3x8ib = {{-1, -2, -3, -4, -5, -6, -7, -8},
                                          {1, 2, 3, 4, 5, 6, 7, 8},
                                          {-9, 10, -11, 12 -13, 14, -15, 16}};

const rect_init_list<double> ex_3x8da =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                          {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                          {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};  

const rect_init_list<double> ex_3x8db = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};


TEST_CASE
(
    "sizes_equal returns whether the two supplied numbers are equal regardless of their sign and integral type",
    "[engine_helper][sizes_equal]"
)
{
    size_t it1s = 0;
    int64_t it1i64 = 0;
    uint32_t it1ui32 = 0;
    int8_t it1i8 = 0;

    size_t it2s = 3;
    int64_t it2i64 = 3;
    uint32_t it2ui32 = 3;
    int8_t it2i8 = 3;
    
    REQUIRE(eh::sizes_equal(it1s, it1s) == true);
    REQUIRE(eh::sizes_equal(it1s, it1i64) == true);
    REQUIRE(eh::sizes_equal(it1i64, it1ui32) == true);
    REQUIRE(eh::sizes_equal(it1ui32, it1i8) == true);
    REQUIRE(eh::sizes_equal(it1s, it1i8) == true);

    REQUIRE(eh::sizes_equal(it2s, it2s) == true);
    REQUIRE(eh::sizes_equal(it2s, it2i64) == true);
    REQUIRE(eh::sizes_equal(it2i64, it2ui32) == true);
    REQUIRE(eh::sizes_equal(it2ui32, it2i8) == true);
    REQUIRE(eh::sizes_equal(it2s, it2i8) == true);

    REQUIRE(eh::sizes_equal(it2s, it1s) == false);
    REQUIRE(eh::sizes_equal(it2s, it1i64) == false);
    REQUIRE(eh::sizes_equal(it2i64, it1ui32) == false);
    REQUIRE(eh::sizes_equal(it2ui32, it1i8) == false);
    REQUIRE(eh::sizes_equal(it2s, it1i8) == false);
}

TEST_CASE
(
    "sizes equal checks if r1==r2 and c1==c2 such that r1*c1 == r2*c2",
    "[engine_helper][sizes_equal(r1, c1, r2, c2)]"
)
{
    size_t it1s = 2;
    int64_t it1i64 = 2;
    uint32_t it1ui32 = 2;
    int8_t it1i8 = 2;

    size_t it2s = 3;
    int64_t it2i64 = 3;
    uint32_t it2ui32 = 3;
    int8_t it2i8 = 3;

    REQUIRE(eh::sizes_equal(it2s, it1i8, it2i64, it1ui32) == true);
    REQUIRE(eh::sizes_equal(it2ui32, it1i64, it2i8, it1s) == true);

    REQUIRE(eh::sizes_equal(it2s, it2i8, it1i64, it1ui32) == false);
    REQUIRE(eh::sizes_equal(it2i64, it1i8, it1i64, it2ui32) == false);
}

TEST_CASE
(
    "sizes_compatible checks if r1*c1 == r2*c2 regardless of the types",
    "[engine_helper][sizes_compatible(r1, c1, r2, c2)]"
)
{
    size_t it1s = 2;
    int64_t it1i64 = 2;
    uint32_t it1ui32 = 2;
    int8_t it1i8 = 2;

    size_t it2s = 3;
    int64_t it2i64 = 3;
    uint32_t it2ui32 = 3;
    int8_t it2i8 = 3;

    REQUIRE(eh::sizes_compatible(it2s, it1i8, it2i64, it1ui32) == true);
    REQUIRE(eh::sizes_compatible(it2ui32, it1i64, it2i8, it1s) == true);

    REQUIRE(eh::sizes_compatible(it2s, it2i8, it1i64, it1ui32) == false);
    REQUIRE(eh::sizes_compatible(it2i64, it1i8, it1i64, it2ui32) == true);
}

TEST_CASE
(
    "validate_length continues execution when the length value is less than zero.",
    "[engine_helper][validate_length(x)]"
)
{
    size_t its = 2;
    int64_t iti64 = -4;
    uint32_t itui32 = 0;
    int8_t iti8 = -8;

    REQUIRE_NOTHROW(eh::validate_length(its));
    REQUIRE_THROWS(eh::validate_length(iti64));
    REQUIRE_THROWS(eh::validate_length(itui32));
    REQUIRE_THROWS(eh::validate_length(iti8));
    REQUIRE_NOTHROW(eh::validate_length(5));
    REQUIRE_THROWS(eh::validate_length(-10));
}

TEST_CASE
(
    "validate_reach continues execution when the reach value is is less than or equal to zero.",
    "[engine_helper][validate_reach(x)]"
)
{
    size_t its = 2;
    int64_t iti64 = -4;
    uint32_t itui32 = 0;
    int8_t iti8 = -8;

    REQUIRE_NOTHROW(eh::validate_reach(its));
    REQUIRE_NOTHROW(eh::validate_reach(itui32));
    REQUIRE_NOTHROW(eh::validate_reach(5));
}

TEST_CASE
(
    "validate_reach stops execution when a supplied reach value is less than zero.",
    "[engine_helper][validate_reach(x)]"
)
{
    int64_t iti64 = -4;
    int8_t iti8 = -8;

    REQUIRE_THROWS(eh::validate_reach(iti64));
    REQUIRE_THROWS(eh::validate_reach(iti8));
    REQUIRE_THROWS(eh::validate_reach(-10));
}

TEST_CASE
(
    "validate_size continues execution when x == y",
    "[engine_helper][validate_size(x, y)]"
)
{
    size_t it1s = 20;
    int64_t it1i64 = 20;
    uint32_t it1ui32 = 20;
    int8_t it1i8 = 20;

    size_t it2s = 3;
    int64_t it2i64 = 3;
    uint32_t it2ui32 = 3;
    int8_t it2i8 = 3;

    REQUIRE_NOTHROW(eh::validate_size(it1s, it1ui32));
    REQUIRE_NOTHROW(eh::validate_size(it1i64, it1i8));
}

TEST_CASE
(
    "validate_size stops execution when x != y",
    "[engine_helper][validate_size(x, y)]"
)
{
    size_t it1s = 20;
    int64_t it1i64 = 20;
    uint32_t it1ui32 = 20;
    int8_t it1i8 = 20;

    size_t it2s = 3;
    int64_t it2i64 = 3;
    uint32_t it2ui32 = 3;
    int8_t it2i8 = 3;

    REQUIRE_THROWS(eh::validate_size(it2s, it1i8));
    REQUIRE_THROWS(eh::validate_size(it2i64, it1i8));
}

TEST_CASE
(
    "validate_init_list continues execution for well defined 2D initializer lists",
    "[engine_helper][validate_init_list(lst)]"
)
{
    const rect_init_list<int64_t> l1 = {{-100, 0, 90, 0, -80, 0, 70, 0},
                                        {-60, 0, 50, 0, -40, 0, 30, 0},
                                        {-20, 0, 10, 0, 0, -10, 0, 10}};

    const rect_init_list<int64_t> l2 = {{2}};
    const rect_init_list<int64_t> l3 = {{1},{2},{3},{4}};

    REQUIRE_NOTHROW(eh::validate_init_list(l1));
    REQUIRE_NOTHROW(eh::validate_init_list(l2));
    REQUIRE_NOTHROW(eh::validate_init_list(l3));
}

TEST_CASE
(
    "validate_init_list stops execution for improperly sized 2D initializer lists",
    "[engine_helper][validate_init_list(lst)]"
)
{

    const rect_init_list<int64_t> l1 = {{2, 3, 3}, {2, 1, 2}, {2, 1}};
    const rect_init_list<int32_t> l2 = {{6, 5, 4},{2, 1, 0, -1}, {-2, -3, -4}};
    const rect_init_list<int8_t> l3 = {{6, 5, 4, 3},{2, 1, 0}, {-2, -3, -4}};

    REQUIRE_THROWS(eh::validate_init_list(l1));
    REQUIRE_THROWS(eh::validate_init_list(l2));
    REQUIRE_THROWS(eh::validate_init_list(l3));
}

/*
 * BEGIN matrix_storage_engine tests
 * NOTES:
 *    - Test cases may used mathods marked with test tags, be aware.
 *  
 * All test code for this suite must start with the following layout:
 * {
 *      using dtype = ...;      // engine data type
 *      using atype = ...;      // engine allocator type
 *      
 *      ... size_t nrows = ...;
 *      ... size_t ncols = ...;
 * 
 *      using ltype = ...;      // engine orientation
 *      
 *      matrix_storage_engine<dtype, atype, ROWS, COLS, ltype> m(...)
 *      
 *      // ... case specific test code
 * }
 */

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

    const rect_init_list<int64_t> data_in = {{-11, -10, -9},
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

    const rect_init_list<int64_t> data_in = {{-100, 0, 90, 0, -80, 0, 70, 0},
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

    const rect_init_list<double> data_in =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
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

    const rect_init_list<int64_t> data_in = {{0, 0, 0}, {3, 2, 1}, {9, -1, -100}};
    const rect_init_list<int64_t> data_in_cm = {{0, 3, 9}, {0, 2, -1}, {0, 1, -100}};

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

    const rect_init_list<double> data_in =  {{sqrt2, sqrt2, sqrt2},
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

    const rect_init_list<double> data_in =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                             {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                             {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};  

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE_THROWS
    (
        m = data_in
        //,std::runtime_error
    );
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

    using ltype = matrix_orientation::row_major_t;

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

    using ltype = matrix_orientation::row_major_t;

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

    using ltype = matrix_orientation::row_major_t;

    matrix_storage_engine<dtype, atype, nrows, ncols, ltype> m;

    REQUIRE(0 == m.rows());
    REQUIRE(0 == m.row_reach());
    REQUIRE(ncols == m.cols());
    REQUIRE(ncols == m.col_reach());

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

    using ltype = matrix_orientation::row_major_t;

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
    "[GIVEN ST ltype col_major]"
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
    
}


// dynamic rows, static cols rect init list constructed, then rows(), cols() matches input data rows, cols
// dynamic rows, static cols, rect init list constructed, then size() and reach() consistent with input data size
// dynamic rows, static cols rect init list constructed then data matches input data

// dynamic rows, static cols default constructed, rect init list assigned then data matches input data

// static rows, dynamic cols default constructed, reshape_cols throws on invalid col_rows
// static rows, dynamic cols default constructed, reshape_cols throws on invalid cll_reach
// static rows, dynamic cols default constructed, reshape_cols resizes when nbr_col > m_col_reach
// static rows, dynamic cols default constructed, reshape_cols resizes when col_reach != m_col_reach
// static rows, dynamic cols default constructed, reshape_cols extends m_rows to nbr_cols when nbr_cols <= m_col_reach and col_reach == m_col_reach

// static rows, dynamic cols rect init list constructed, then rows(), cols() matches input data rows, cols
// static rows, dynamic cols, rect init list constructed, then size() and reach() consistent with input data size

// static rows, dynamic cols rect init list constructed then data matches input data
// static rows, dynamic cols default constructed, rect init list assigned then data matches input data

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
    REQUIRE(0 == m.cols());
    REQUIRE(0 == m.col_reach());
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

}

TEST_CASE
(
    "IF m is nbr_cols, cols_reach constructed\n"
    "AND THEN reshape_cols is called SUCH THAT new_nbr_cols <= row_reach() AND new_col_reach == col_reach()\n"
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
    "[GIVEN ST ltype col_major]"
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
    
}



















/*
TEST_CASE
(
    "matrix_storage_engine is copy constructable and shares shame "
)*/

 