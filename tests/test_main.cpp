#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include "../matrix.h"
#include "../result.h"

//#define PY_SSIZE_T_CLEAN
//#include <Python.h>

#define S_RAND(N) ((size_t)(std::rand() % N))

template<class T>
void print_matrix(matrix<T>* mat)
{
    for(int i=0; i < mat->rows(); i++)
    {
        for(int j=0; j < mat->cols(); j++)
        {
            std::cout << mat->get_value(i, j) << "\t";
        }
        std::cout << "\n";
    }
}

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

class RunListener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;
    
    void testRunStarting(const Catch::TestRunInfo &info) override
    {
        std::cout << "started\n";
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

//#include "test_mat.cpp"
//#include "test_stats.cpp"
#include "test_householder.cpp"
//#include "test_prods.cpp"
//#include "test_gram_schmidt.cpp"


