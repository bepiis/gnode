//
//  test_householder.cpp
//  Created by Ben Westcott on 9/28/24.
//

#include "../householder.h"
#include "../gram_schmidt.h"

using namespace transformation::house;

TEST_CASE("test house")
{
    std::cout << "test transformation::house::house: \n";
    
    size_t M = S_RAND(100);
    matrix<double> x = matrix<double>::random_dense_matrix(M, 1, -1000, 1000);
    
    auto housepair = house(x);
    matrix<double> vhouse = housepair.first;
    double beta = housepair.second;
    
    matrix<double> P = matrix<double>::eye(M) - beta * outer_prod_1D(vhouse, vhouse);
    matrix<double> xe1 = inner_right_prod(P, x);

    double x2norm = std::sqrt(col_norm2sq(x, 0));
    double zero_tol = 1E-12;
    double reldist = 1E-12;
    double max_mag = std::max(x2norm, xe1(0, 0));
    double absdiff = std::abs(x2norm - xe1(0,0));
    
    std::cout << "\t-norm2 = " << x2norm << "\n";
    std::cout << "\t-errmax = " << absdiff << "\n";
    
    REQUIRE(absdiff < reldist * max_mag);
    
    for(size_t r=1; r < xe1.rows(); r++)
    {
        REQUIRE(std::abs(xe1(r, 0)) < zero_tol);
    }
}
// TODO: maybe one more non-square test, but householder + Qaccumulate will be tested rigorously to verify that the algorithm works
TEST_CASE("householder")
{
    std::cout << "test transformation::house::householder:\n";
    
    // very basic test first
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto basic_chk = transformation::GS::QR(basic);
    basic = transformation::house::householder(basic);
    
    //matrix<double> basic_chk(3, 3, {6, -3, -8, 1, 5.65685, -3.53553, 1, 2.41421, 0.707107});
    basic.fill_lower_triangle(0.0);
    
    double errmax = matrix<double>::abs_max_err(basic, basic_chk.second);
    size_t errcnt = matrix<double>::abs_max_excess_err(basic, basic_chk.second, 1E-14);
    
    std::cout << "\t-errcnt = " << errcnt << "\n";
    std::cout << "\t-errmax = " << errmax << "\n";
}
