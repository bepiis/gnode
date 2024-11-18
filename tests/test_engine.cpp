//
//  test_engine.cpp
//  Created by Ben Westcott on 9/8/24.
//

#include "core/engine.h"
#include <numbers>

using namespace std::numbers;
using std::size_t;
using eh = engine_helper;

template<typename T>
using rect_init_list = std::initializer_list<std::initializer_list<T>>;

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


TEST_CASE("engine_helper::sizes_equal(r, c)")
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

TEST_CASE("engine_helper::sizes_equal(r1, c1, r2, c2)")
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

TEST_CASE("engine_helper::sizes_compatible(r1, c1, r2, c2)")
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

TEST_CASE("engine_helper::validate_length(x)")
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

TEST_CASE("engine_helper::validate_reach(x)")
{
    size_t its = 2;
    int64_t iti64 = -4;
    uint32_t itui32 = 0;
    int8_t iti8 = -8;

    REQUIRE_NOTHROW(eh::validate_reach(its));
    REQUIRE_THROWS(eh::validate_reach(iti64));
    REQUIRE_NOTHROW(eh::validate_reach(itui32));
    REQUIRE_THROWS(eh::validate_reach(iti8));
    REQUIRE_NOTHROW(eh::validate_reach(5));
    REQUIRE_THROWS(eh::validate_reach(-10));
}

TEST_CASE("engine_helper::validate_size(x, y)")
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

    REQUIRE_THROWS(eh::validate_size(it2s, it1i8));
    REQUIRE_THROWS(eh::validate_size(it2i64, it1i8));
}

TEST_CASE("engine_helper::validate_init_list(lst)")
{
    REQUIRE_NOTHROW(eh::validate_init_list(ex_1x1i));
    REQUIRE_NOTHROW(eh::validate_init_list(ex_4x1i));
    REQUIRE_NOTHROW(eh::validate_init_list(ex_3x8da));
    REQUIRE_THROWS(eh::validate_init_list(ex_invalidi));
}

template<typename T>
using rm_dyn_mse = matrix_storage_engine<T, std::allocator<T>, std::dynamic_extent, std::dynamic_extent, matrix_orientation::row_major_t>;

template<typename T>
using cm_dyn_mse = matrix_storage_engine<T, std::allocator<T>, std::dynamic_extent, std::dynamic_extent, matrix_orientation::col_major_t>;



TEST_CASE("initializer list constructor")
{
    // can be default constructed

    rm_dyn_mse<int64_t> m1(ex_1x1i);
    cm_dyn_mse<int64_t> m2(ex_1x1i);
}

 