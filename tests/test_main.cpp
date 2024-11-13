#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <chrono>

//#define PY_SSIZE_T_CLEAN
//#include <Python.h>

#define S_RAND(N) ((size_t)(std::rand() % N))

int* generate_random_ints(size_t nbr, size_t modulus)
{
    int *res = new int[nbr]();
    
    for(size_t i=0; i < nbr; i++)
    {
        res[i] = S_RAND(modulus);
    }
    
    return res;
}

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include "matrix.h"
#include "result.h"
#include "products.h"
#include "stats.h"
#include "householder.h"
#include "givens.h"
#include "jacobi.h"
#include "gram_schmidt.h"
#include "permutation.h"
#include "tdpool.h"

constexpr size_t mult_pool_size = 6;
tdpool mult_pool(mult_pool_size);

class RunListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;
    
    void testRunStarting(const Catch::TestRunInfo &info) override
    {
        //mult_pool = tdpool(mult_pool_size);
        std::cout << "started mult pool with size " << mult_pool_size << "\n";
        //Py_Initialize();
        //PyRun_SimpleString("from time import time,ctime\nprint('Today is', ctime(time()))\n");
    }
    
    void testRunEnded(const Catch::TestRunStats &info) override
    {
        //Py_FinalizeEx();
    }
};

CATCH_REGISTER_LISTENER(RunListener);

#include <catch2/generators/catch_generators_all.hpp>
#include <random>

struct mat_size_wrap
{
    size_t M;
    size_t N;
    
    mat_size_wrap() = default;
    
    mat_size_wrap(size_t m, size_t n)
    : M(m), N(n) {}
    
    mat_size_wrap(int m, int n)
    : mat_size_wrap(static_cast<size_t>(m), static_cast<size_t>(n)) {}
};

class RandomMatSizeGenerator final : public Catch::Generators::IGenerator<mat_size_wrap>
{
    std::minstd_rand m_rand;
    std::uniform_int_distribution<> m_dist;
    mat_size_wrap current_size;
    bool square;
    
public:
    
    RandomMatSizeGenerator(size_t low, size_t high, bool sq)
    : m_rand(std::random_device{}()), m_dist(low, high), square(sq)
    {
        static_cast<void>(next());
    }
    
    mat_size_wrap const& get(void) const override;
    
    bool next(void) override
    {
        int m = m_dist(m_rand);
        int n = square ? m : m_dist(m_rand);
        
        current_size = mat_size_wrap(m, n);
        return true;
    }
};

mat_size_wrap const& RandomMatSizeGenerator::get(void) const
{
    return current_size;
}

Catch::Generators::GeneratorWrapper<mat_size_wrap> randmatsize(size_t low, size_t high, bool square)
{
    return Catch::Generators::GeneratorWrapper<mat_size_wrap>
    (
        new RandomMatSizeGenerator(low, high, square)
     );
}

TEST_CASE("RandomMatSizeGenerator maybe-square")
{
    auto i = GENERATE(take(5, randmatsize(2, 100, false)));
    REQUIRE(i.M >= 2);
    REQUIRE(i.M <= 100);
    REQUIRE(i.N >= 2);
    REQUIRE(i.N <= 100);
}

TEST_CASE("RandomMatSizeGenerator square")
{
    auto j = GENERATE(take(5, randmatsize(2, 100, true)));
    REQUIRE(j.M >= 2);
    REQUIRE(j.M <= 100);
    REQUIRE(j.M == j.N);
}

Catch::Generators::GeneratorWrapper<mat_size_wrap> rd_randmatsize(size_t low, size_t high)
{
    return Catch::Generators::filter
    (
        [](mat_size_wrap s) { return s.M >= s.N; },
        Catch::Generators::GeneratorWrapper<mat_size_wrap>
        (
            new RandomMatSizeGenerator(low, high, false)
        )
    );
}

TEST_CASE("row dominant RandMatSizeGenerator")
{
    auto i = GENERATE(take(5, rd_randmatsize(2, 100)));
    
    REQUIRE(i.M >= 2);
    REQUIRE(i.M <= 100);
    REQUIRE(i.N >= 2);
    REQUIRE(i.N <= 100);
    REQUIRE(i.M >= i.N);
}

#include "engine.h"
#include <numbers>

using namespace std::numbers;

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

//template<typename T>
//using rm_mse = matrix_storage_engine<T, std::allocator<T>, matrix_orientation::row_major_t>;

//template<typename T>
//using cm_mse = matrix_storage_engine<T, std::allocator<T>, matrix_orientation::col_major_t>;

TEST_CASE("initializer list constructor")
{
    //rm_mse<int64_t> m1(ex_1x1i);
    //cm_mse<int64_t> m2(ex_1x1i);

    

}

#define TEST_FULL_VERBOSE_OUTPUT


#ifndef TEST_NONE

//#include "test_mat.cpp"
//#include "test_stats.cpp"
//#include "test_householder.cpp"
//#include "test_givens.cpp"
//#include "test_prods.cpp"
//#include "test_gram_schmidt.cpp"
//#inlcude "test_jacobi.cpp"

#endif
