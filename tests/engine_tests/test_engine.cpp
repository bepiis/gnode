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
using literal2D = std::initializer_list<std::initializer_list<T>>;

template<typename T>
using literal1D = std::initializer_list<T>;

const literal2D<int64_t> ex_1x1i = {{2}};
const literal2D<int64_t> ex_1x2i = {{2, 3}};
const literal2D<int64_t> ex_1x4i = {{9, 8, 7, 6}};
const literal2D<int64_t> ex_2x1i = {{2}, {3}};
const literal2D<int64_t> ex_4x1i = {{1},{2},{3},{4}};

const literal2D<int64_t> ex_3x3ia = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
const literal2D<int64_t> ex_3x3ib = {{0, 0, 0}, {3, 2, 1}, {9, -1, -100}};

const literal2D<int64_t> ex_3x3ic = {{INT64_MAX, -INT64_MAX, INT64_MAX}, 
                                          {-INT64_MAX, -INT64_MAX, -INT64_MAX}, 
                                          {INT64_MAX, INT64_MAX, INT64_MAX}};

const literal2D<int64_t> ex_boundsi = ex_3x3ic;
const literal2D<int64_t> ex_invalidi = {{2, 3, 3}, {2, 1, 2}, {2, 1}};

const literal2D<int64_t> ex_8x3ia = {{1, 2, 3}, 
                                          {4, 5, 6}, 
                                          {7, 8, 9}, 
                                          {10, 11, 12},
                                          {13, 14, 15},
                                          {16, 17, 18},
                                          {19, 20, 21},
                                          {22, 23, 24}};

const literal2D<int64_t> ex_8x3ib = {{-11, -10, -9},
                                          {-8, -7, -6},
                                          {-5, -4, -3},
                                          {-2, -1, 0},
                                          {1, 2, 3},
                                          {4, 5, 6},
                                          {7, 8, 9},
                                          {10, 11, 12}};

const literal2D<double> ex_8x3da = {{e, pi, e}, 
                                         {-e, -pi, -e},
                                         {pi, pi, pi}, 
                                         {e, e, e}, 
                                         {-pi, -pi, -pi}, 
                                         {-e, -e, -e},
                                         {pi, e, pi}, 
                                         {-pi, -e, -pi}};

const literal2D<double> ex_8x3db = {{sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2},
                                         {sqrt2, sqrt2, sqrt2},
                                         {1/sqrt2, 1/sqrt2, 1/sqrt2}};

const literal2D<int64_t> ex_3x8ia = {{-100, 0, 90, 0, -80, 0, 70, 0},
                                          {-60, 0, 50, 0, -40, 0, 30, 0},
                                          {-20, 0, 10, 0, 0, -10, 0, 10}};

const literal2D<int64_t> ex_3x8ib = {{-1, -2, -3, -4, -5, -6, -7, -8},
                                          {1, 2, 3, 4, 5, 6, 7, 8},
                                          {-9, 10, -11, 12 -13, 14, -15, 16}};

const literal2D<double> ex_3x8da =  {{-1E0, -2E1, -3E2, -4E3, -5E4, -6E5, -7E6, -8E7},
                                          {1E0, 2E1, 3E2, 4E3, 5E4, 6E5, 7E6, 8E7},
                                          {-9E8, 10E9, -11E10, 12E11, -13E12, 14E13, 15E14, -16E15}};  

const literal2D<double> ex_3x8db = {{-1E-12, -1E-11, -1E-10, -1E-9, -1E-8, -1E-7, -1E-6, -1E-5},
                                         {-1E-4, -1E-3, -1E-2, -1E-1, -1E0, 1E0, 1E1, 1E2},
                                         {1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10}};

const literal2D<double> ex_6x6da = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                    {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                    {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                    {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                    {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                    {1.30, 1.31, 1.32, 1.33, 1.34, 1.35}};

const literal2D<double> ex_6x8da  = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05, -1.00, -1.10},
                                     {1.06, 1.07, 1.08, 1.09, 1.10, 1.11, -2.00, -2.20},
                                     {1.12, 1.13, 1.14, 1.15, 1.16, 1.17, -3.00, -3.30},
                                     {1.18, 1.19, 1.20, 1.21, 1.22, 1.23, -4.00, -4.40},
                                     {1.24, 1.25, 1.26, 1.27, 1.28, 1.29, -5.00, -5.50},
                                     {1.30, 1.31, 1.32, 1.33, 1.34, 1.35, -6.00, -6.60}};

const literal2D<int64_t> ex_6x8ia = {{0, 1, 2, 3, 4, 5, 6, 7},
                                     {8, 9, 10, 11, 12, 13, 14, 15},
                                     {16, 17, 18, 19, 20, 21, 22, 23},
                                     {24, 25, 26, 27, 28, 29, 30, 31},
                                     {32, 33, 34, 35, 36, 37, 38, 39},
                                     {40, 41, 42, 43, 44, 45, 46, 47}};


const literal2D<double> ex_8x6da = {{1.00, 1.01, 1.02, 1.03, 1.04, 1.05},
                                    {1.06, 1.07, 1.08, 1.09, 1.10, 1.11},
                                    {1.12, 1.13, 1.14, 1.15, 1.16, 1.17},
                                    {1.18, 1.19, 1.20, 1.21, 1.22, 1.23},
                                    {1.24, 1.25, 1.26, 1.27, 1.28, 1.29},
                                    {1.30, 1.31, 1.32, 1.33, 1.34, 1.35},
                                    {1.36, 1.37, 1.38, 1.39, 1.40, 1.41},
                                    {1.42, 1.43, 1.44, 1.45, 1.46, 1.47}};

const literal2D<int64_t> ex_8x6ia = {{-48, -46, -44, -42, -40, -38},
                                    {-36, -34, -32, -30, -28, -24},
                                    {-22, -20, -18, -16, -14, -12},
                                    {-10, -8, -6, -4, -2, 0},
                                    {2, 4, 6, 8, 10, 12},
                                    {14, 16, 18, 20, 22, 24}};

const literal2D<double> ex_3x5da = {{ln2, -ln2, ln2, -ln2, ln2},
                                    {-ln2, ln2, -ln2, ln2, -ln2},
                                    {ln2, -ln2, ln2, -ln2, ln2}};


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
    "validate_literal2D continues execution for well defined 2D initializer lists",
    "[engine_helper][validate_literal2D(lst)]"
)
{
    const literal2D<int64_t> l1 = {{-100, 0, 90, 0, -80, 0, 70, 0},
                                        {-60, 0, 50, 0, -40, 0, 30, 0},
                                        {-20, 0, 10, 0, 0, -10, 0, 10}};

    const literal2D<int64_t> l2 = {{2}};
    const literal2D<int64_t> l3 = {{1},{2},{3},{4}};

    REQUIRE_NOTHROW(eh::validate_literal2D(l1));
    REQUIRE_NOTHROW(eh::validate_literal2D(l2));
    REQUIRE_NOTHROW(eh::validate_literal2D(l3));
}

TEST_CASE
(
    "validate_literal2D stops execution for improperly sized 2D initializer lists",
    "[engine_helper][validate_literal2D(lst)]"
)
{

    const literal2D<int64_t> l1 = {{2, 3, 3}, {2, 1, 2}, {2, 1}};
    const literal2D<int32_t> l2 = {{6, 5, 4},{2, 1, 0, -1}, {-2, -3, -4}};
    const literal2D<int8_t> l3 = {{6, 5, 4, 3},{2, 1, 0}, {-2, -3, -4}};

    REQUIRE_THROWS(eh::validate_literal2D(l1));
    REQUIRE_THROWS(eh::validate_literal2D(l2));
    REQUIRE_THROWS(eh::validate_literal2D(l3));
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

#include "test_storage_engine_full_static.cpp"
#include "test_storage_engine_col_static.cpp"
#include "test_storage_engine_row_static.cpp"
#include "test_storage_engine_full_dynamic.cpp"


// static rows, dynamic cols rect init list constructed, then rows(), cols() matches input data rows, cols
// static rows, dynamic cols, rect init list constructed, then size() and reach() consistent with input data size

// static rows, dynamic cols rect init list constructed then data matches input data
// static rows, dynamic cols default constructed, rect init list assigned then data matches input data

/*
TEST_CASE
(
    "matrix_storage_engine is copy constructable and shares shame "
)*/

 