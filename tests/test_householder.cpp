//
//  test_householder.cpp
//  Created by Ben Westcott on 9/28/24.
//

#include "../householder.h"
#include "../gram_schmidt.h"

using namespace transformation::house;

TEST_CASE("test house")
{
    double zero_tol = 1E-11;
    double reldist = 1E-11;
    
    std::cout << "test transformation::house::house (basic):";
    
    size_t M = S_RAND(100);
    matrix<double> x = matrix<double>::random_dense_matrix(M, 1, -1000, 1000);
    
    auto housepair = house(x);
    matrix<double> vhouse = housepair.first;
    double beta = housepair.second;
    
    matrix<double> P = matrix<double>::eye(M) - beta * outer_prod_1D(vhouse, vhouse);
    matrix<double> xe1 = inner_right_prod(P, x);

    double x2norm = std::sqrt(col_norm2sq(x, 0));

    double max_mag = std::max(x2norm, xe1(0, 0));
    double absdiff = std::abs(x2norm - xe1(0,0));
    
    std::cout << "\tnorm2 = " << x2norm;
    std::cout << "\terrmax = " << absdiff;
    
    std::cout << "\n";
    
    REQUIRE(absdiff < reldist * max_mag);
    
    for(size_t r=1; r < xe1.rows(); r++)
    {
        REQUIRE(std::abs(xe1(r, 0)) < zero_tol);
    }
    
    REQUIRE(absdiff < zero_tol);
}
// TODO: maybe one more non-square test, but householder + Qaccumulate will be tested rigorously to verify that the algorithm works
TEST_CASE("householder basic")
{
    size_t cnt_tol = 0;
    double zero_tol =  1E-14;
    double errmax;
    size_t errcnt;
    
    std::cout << "test transformation::house::householder (basic):";
    
    // very basic test first
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto basic_chk = transformation::GS::QR(basic);
    basic = householder(basic);
    
    //matrix<double> basic_chk(3, 3, {6, -3, -8, 1, 5.65685, -3.53553, 1, 2.41421, 0.707107});
    basic.fill_lower_triangle(0.0);
    
    errcnt = matrix<double>::abs_max_excess_err(basic, basic_chk.second, zero_tol);
    errmax = matrix<double>::abs_max_err(basic, basic_chk.second);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("Qaccumulate basic, square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-14;
    double errmax;
    size_t errcnt;
    
    std::cout << "test transformation::house::Qaccumulate (basic, square):";
    
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    matrix<double> basic_cpy(basic);
    
    basic = householder(basic);
    
    //print_matrix(&basic);
    //std::cout << "\n\n";
    
    matrix<double> Qb = Qaccumulate(basic, basic.rows(), 0);
    
    //print_matrix(&Qb);
    //std::cout << "\n\n";
    basic.fill_lower_triangle(0.0);
    matrix<double> chk = mat_mul_alg1(&Qb, &basic);
    
    errcnt = matrix<double>::abs_max_excess_err(chk, basic_cpy, zero_tol);
    errmax = matrix<double>::abs_max_err(chk, basic_cpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
    
    //print_matrix(&chk);
    //std::cout << "\n\n";
}

/*
 j = n - 2 passed
 258.013    813.807    872.272    -832.68
 797.269    955.722    819.563    -88.8255
 844.486    -793.187    501.011    148.005
 645.232    -659.314    959.932    66.0464
 
 
*/
TEST_CASE("Qaccumulate rand, randsize, square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    size_t M = GENERATE(take(20, random(1, 100)));
    
    std::cout << "test transformation::house::Qaccumulate (rand, M = " << M << ", N = " << M << ", square): ";
    
    matrix<double> randm = matrix<double>::random_dense_matrix(20, 20, -1000, 1000);
    matrix<double> randmcpy(randm);
    
    randm = householder(randm);
    
    matrix<double> Qrand = Qaccumulate(randm, randm.rows(), 0);

    randm.fill_lower_triangle(0.0);
    matrix<double> rchk = mat_mul_alg1(&Qrand, &randm);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, randmcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, randmcpy);
    
    /*matrix<double> rabsd = matrix<double>::absdiff(rchk, randmcpy);
    for(size_t r=0; r < rabsd.rows(); r++)
    {
        for(size_t c=0; c < rabsd.cols(); c++)
        {
            if(rabsd(r, c) < zero_tol)
            {
                rabsd(r, c) = 0.0;
            }
        }
    }
    print_matrix(&randmcpy);*/
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
    
}

TEST_CASE("Qaccumulate rand, randsize")
{
    
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    const size_t M = 100;
    auto N = GENERATE(1, 100, take(20, filter([](size_t i) { return i <= 100; }, random(1, 100))));
    
    std::cout << "test transformation::house::Qaccumulate (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> tst = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    
    matrix<double> tstcpy(tst);
    tst = householder(tst);
    
    matrix<double> Qrand = Qaccumulate(tst, tst.rows(), 0);
    
    tst.fill_lower_triangle(0.0);
    matrix<double> rchk = mat_mul_alg1(&Qrand, &tst);
    
    //print_matrix(&rchk);
    //std::cout << "\n\n";
    
    //print_matrix(&tstcpy);
    
    //std::cout << "\n\n";
    
    //print_matrix(&Qrand);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, tstcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, tstcpy);
    
    /*matrix<double> rabsd = matrix<double>::absdiff(rchk, tstcpy);
    for(size_t r=0; r < rabsd.rows(); r++)
    {
        for(size_t c=0; c < rabsd.cols(); c++)
        {
            if(rabsd(r, c) < zero_tol)
            {
                rabsd(r, c) = 0.0;
            }
        }
    }
    print_matrix(&tstcpy);*/
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

/*
 1053.21    716.778    -262.262    -1265.85
 -0.666564    655.022    829.657    -86.4781
 0.318846    0.678307    849.109    656.642
 -1.04618    -1.64197    -22.4779    -813.778
 */

