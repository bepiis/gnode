//
//  test_givens.cpp
//  Created by Ben Westcott on 10/9/24.
//

#include "../givens.h"
#include "../gram_schmidt.h"

using namespace transformation::givens;

TEST_CASE("bruh")
{
    
    //matrix<double> basic(5, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8, 2, -1, -2, -4, 6, 3});
    //matrix<double> basic = {{2, -1, 2, 2}, {-4, 6, 3, 3}, {-4, -2, 8, 8}, {1, 2, 3, 4}};
    //matrix<double> basic = {{2, -1, 2}, {5, 6, 3}, {5, -1, 8}};
    //matrix<double> basic = {{2, 5, 2}, {-1, 5, 3}, {6, -1, 8}};
    
    matrix<double> basic = {{2, -1, 2, 2}, {-4, 6, 3, 3}, {-4, -2, 8, 8}, {-4, 2, 3, 4}};
    //matrix<double> basic = matrix<double>::random_dense_matrix(3, 3, -1000, 1000);
    std::cout << basic << "\n";
    //matrix<double> basic = {{-2, -1}, {-4, 6}};
    matrix<double> bcpy(basic);
    
    basic = QRfast(basic);
    
    auto out = transformation::GS::QR(bcpy);
    
    
    matrix<double> Q = QRaccumulate(basic);
    
    std::cout << out.Q << "\n";
    std::cout << Q << "\n";
    

    
    
    std::cout << basic << "\n";
    
    basic.fill_lower_triangle(0.0);
    
    matrix<double> QT = Q.transpose();
    auto res = mat_mul_alg1(&Q, &basic);
    
    std::cout << res << "\n";


    
}
