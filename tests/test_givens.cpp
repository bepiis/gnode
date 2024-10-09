//
//  test_givens.cpp
//  Created by Ben Westcott on 10/9/24.
//

#include "../givens.h"
#include "../gram_schmidt.h"

using namespace transformation::givens;

TEST_CASE("test")
{
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    matrix<double> bcpy(basic);
    
    basic = QRfast(basic);
    
    bcpy = transformation::GS::MGS(bcpy);
    
    std::cout << basic << "\n";
    
    std::cout << bcpy << "\n";
    
}
