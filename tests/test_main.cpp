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
#include "gram_schmidt.h"
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
/*
TEST_CASE("qrhfast")
{
    using namespace transformation::house;
    matrix<double> basic = {{2, -1, 2}, {-4, 6, 3}, {-4, -1, 8}};
    uint64_t elapsed = 0;

    auto rbas = time_exec(elapsed, QR, basic);
    auto lbas = time_exec(elapsed, QL, basic);


    std::cout << rbas.Y << "\n";
    std::cout << lbas.Y << "\n";
    std::cout << elapsed << "\n";
}*/

TEST_CASE("givens timecheck")
{
    using namespace transformation::givens;

    matrix<double> randm = matrix<double>::random_dense_matrix(10, 10, -1000, 1000);

    std::cout << randm << "\n";

    randm = QRfast(randm);

}

#define TEST_FULL_VERBOSE_OUTPUT


#ifndef TEST_NONE

//#include "test_mat.cpp"
//#include "test_stats.cpp"
//#include "test_householder.cpp"
#include "test_givens.cpp"
//#include "test_prods.cpp"
//#include "test_gram_schmidt.cpp"

#endif


/*
#include <experimental/simd>
#include <chrono>
#include "../householder.h"

#include <cstdint>
#include <mdspan>

TEST_CASE("mdspan")
{
    
    matrix<double> A(4, 4,
                     {
        2, -1, -2, 2,
        -4, 6, 3, 4,
        -4, -2, 8, 6,
        -3, -4, 5, -4
    });
    
    using ext_t = std::extents<uint32_t, 4, 4>;
    
    auto ms = std::mdspan<double, ext_t, std::layout_right>(A.data());
    auto cs = std::mdspan<double, ext_t, std::layout_left>(A.data());
    
    for(uint32_t i=0; i < ms.extent(0); i++)
    {
        for(uint32_t j=0; j < ms.extent(1); j++)
        {
            std::cout << ms[i, j] << "\t";
        }
        std::cout << "\n";
    }
    
    for(uint32_t i=0; i < cs.extent(0); i++)
    {
        for(uint32_t j=0; j < cs.extent(1); j++)
        {
            std::cout << cs[i, j] << "\t";
        }
        std::cout << "\n";
    }
    
}*/
/*
TEST_CASE("test")
{
    
    using namespace transformation::house;
    
    matrix<double> A(4, 4,
                     {
        2, -1, -2, 2,
        -4, 6, 3, 4,
        -4, -2, 8, 6,
        -3, -4, 5, -4
    });
    
    size_t N = A.rows();
    house h;
    matrix<double> Ablk, Apar;
    
    for(size_t k=0; k < N - 2; k++)
    {
        h = housevec(A.sub_col(k + 1, N - k - 1, k), 0);
        
        Ablk = A.sub_matrix(k + 1, N - k - 1, k, N - k);
        
        std::cout << "Ablk:\n";
        std::cout << Ablk << "\n";
        
        // A <- QA
        //A.set_sub_matrix(Ablk, k + 1, k);
        
        Apar = A.sub_matrix(0, N, k + 1, N - k - 1);
        
        std::cout << "Apar:\n";
        std::cout << Apar << "\n";
        
        // A <- A(Q^T)
        //Apar -= h.beta * outer_prod_1D(inner_right_prod(Apar, h.vec), h.vec);
        
        //A.set_sub_matrix(Apar, 0, k + 1);
        
        size_t i=1;
        for(size_t j = k + 2; j < N; j++, i++)
        {
            A(j, k) = h.vec(i, 0);
        }
    }
    
    std::cout << "\n\n\n";
    
    A = QLHfast(A);
    
}*/


/*
#include "../householder.h"

TEST_CASE("RQ TEST")
{
    matrix<double> basic = {{2, -1, 2}, {-4, 6, 3}, {-4, -1, 8}};
    
    std::cout << basic << "\n";
    
    //transformation::house::QRfast(basic);
    
    basic = transformation::house::LQfast(basic);
    
    matrix<double> Qa = transformation::house::LQaccumulate(basic);

    basic.fill_upper_triangle(0.0);

    std::cout << basic << "\n";
    
    matrix<double> out = mat_mul_alg1(&basic, &Qa);
    
    std::cout << "out = \n";
    std::cout << out << "\n";
    
    std::cout << "\n====================================\n";
    
    matrix<double> b2 = {{2, -1, 2, 3, 5}, {-4, 6, 3, 6, 7}, {-4, -1, 8, 1, 2}};
    
    b2 = transformation::house::LQfast(b2);
    
    matrix<double> Qb = transformation::house::LQaccumulate(b2);

    b2.fill_upper_triangle(0.0);
    std::cout << b2 << "\n";
    
    matrix<double> outb = mat_mul_alg1(&b2, &Qb);
    
    std::cout << "out = \n";
    std::cout << outb << "\n";

    std::cout << "\n====================================\n";

    matrix<double> b3 = {{2, -1, 2, 3, 5}, {-4, 6, 3, 6, 7}, {-4, -1, 8, 1, 2}};

    b3 = transformation::house::RQfast(b3);

    matrix<double> Q3 = transformation::house::RQaccumulate(b3);
    
    size_t ex_cols = 2;
    for(size_t r=0; r < b3.rows(); r++, ex_cols++)
    {
        for(size_t c=0; c < ex_cols; c++)
        {
            b3(r, c)= 0.0;
        }
    }

    std::cout << b3 << "\n";

    matrix<double> out3 = mat_mul_alg1(&b3, &Q3);

    std::cout << out3 << "\n";
    
}*/
/*
TEST_CASE("test")
{
    matrix<double> b(4, 4,
    {
        2, -1, -2, 2,
        -4, 6, 3, 4,
        -4, -2, 8, 6,
        -3, -4, 5, -4
    });
    
    std::cout << b << "\n";
    
    auto res = transformation::house::QRH(b);
    
    std::cout << "Q = \n";
    std::cout << res.Q << "\n\n";
    
    std::cout << "R = \n";
    std::cout << res.H << "\n\n";
    
    b = transformation::house::QLHfast(b);
    
    matrix<double> Q = transformation::house::QLaccumulate(b, 1);
    
    std::cout << "\n\n\n\n\n";
    
    int64_t exrows = 3;
    for(int64_t c = b.cols() - 1; c >= 0; c--)
    {
        for(int64_t r = b.rows() - exrows; r >= 0; r--)
        {
            b(r, c) = 0.0;
        }
        exrows++;
    }
    std::cout << b << "\n";
    matrix<double> QT = Q.transpose();

    matrix<double> chk = mat_mul_alg1(&Q, &b);
    matrix<double> rchk = mat_mul_alg1(&chk, &QT);
    
    std::cout << rchk << "\n";
    
    matrix<double> Qchk = mat_mul_alg1(&Q, &QT);
    
    std::cout << Qchk << "\n";
    
}*/

//#include <mdspan>

 /*
 TEST_CASE("mdspan")
 {


    std::vector v 
    {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16
    };
    
    constexpr size_t M = 4;
    constexpr size_t N = 4;

    std::mdspan<int, 
            std::extents<size_t, M, N>,
            std::layout_right> mv{v.data()};

    for (std::size_t r = 0; r < mv.extent(0); r++)
    {
        for (std::size_t c = 0; c < mv.extent(1); c++)
        {
            std::cout << mv[r, c] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\n\n";

    std::mdspan<int, 
                  std::extents<size_t, std::dynamic_extent, std::dynamic_extent>, 
                  std::layout_stride> 

    mk{ v.data(), 
       { std::dextents<size_t,2>{8, 2}, 
         std::array<std::size_t, 2>{3, 1}}};

    for (std::size_t r = 0; r < mk.extent(0); r++)
    {
        for (std::size_t c = 0; c < mk.extent(1); c++)
        {
            std::cout << mk[r, c] << ' ';
        }
        std::cout << '\n';
    }

   
    std::dextents<size_t,2> shape{2, 2};
    std::array<size_t, 2> strides{shape.extent(0), 4};

    std::mdspan mk{v.data(), std::layout_stride::mapping{shape, strides}};

    for (std::size_t r = 0; r < mk.extent(0); r++)
    {
        for (std::size_t c = 0; c < mk.extent(1); c++)
        {
            std::cout << mk[r, c] << ' ';
        }
        std::cout << '\n';
    }

    std::mdspan mp(v.data(), N, M);
    for(size_t c=0; c < mp.extent(1); c++)
    {
        std::extents sub_ext(mp.extent(0));
        std::array strides{mp.stride(0)};

        std::layout_right::map sub_map(sub_ext, strides);
        std::mdspan col_c(&mp[0, c], sub_map);

    }

 }*/