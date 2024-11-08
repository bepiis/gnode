//
//  playground.cpp
//  Created by Ben Westcott on 9/8/24.
//

/*
 * Plan of action for new structure:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * engine_support:
 *      size_compare: returns whether two engines are the same size
 * 
 *      verify_initializer_list: returns whether supplied intializer list is valid
 *                  - i.e. {{1,2,3}, {4,5,6}} is valid
 *                  - but {{1,2,3}, {4,5}} is not
 * 
 *      verify_size: uses size_compare to verify if two engines have compatible sizes
 *      
 *      swap noexcept: swaps two things T1 and T2 via std::move
 * 
 *      verify_and_reshape: takes a destination, and source rows + cols and reshapes dst to fit source
 *          - checks if reshape need to occur (ie. dst rows & cols != src rows & cols)
 * 
 *      assign_from: takes an engine, and some other type and assigns the data of the other type to the engine
 *              - engine -> engine
 *              - mdspan -> engine
 *              - initializer_list (2D) -> engine
 *              - 1D random access container -> engine
 *       
 *      fill_(rows|cols) : fills r|c from pos a0 to a1
 * 
 *      move_elements: move elements from engine1 to engine2 starting at (0,0)??? to (n, m)
 * 
 *      exact_compare: compares element-wise (after checking size_compare) whether an engine is exactly equal to another engine or related type
 * 
 * 
 * 
 */

#include <memory>
TEST_CASE("std::allocator")
{
    std::allocator<int> alloc1;

    static_assert(std::is_same_v<int, decltype(alloc1)::value_type>);
    
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

/*
TEST_CASE("givens timecheck")
{
    using namespace transformation::givens;

    matrix<double> randm = matrix<double>::random_dense_matrix(10, 10, -1000, 1000);

    std::cout << randm << "\n";

    randm = QRfast(randm);

}*/
/*
TEST_CASE("permutes")
{
    matrix<size_t> Fp = permutation::flip(6);
    matrix<double> Im = matrix<double>::eye(6);

    std::cout << Fp << "\n";

    permutation::onrows(Im, Fp);

    std::cout << Im << "\n";
}*/


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