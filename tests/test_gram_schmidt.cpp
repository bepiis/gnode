//
//  test_gram_schdmit.cppp
//  Created by Ben Westcott on 9/25/24.
//

#include "../gram_schmidt.h"
#include "../householder.h"
/*
TEST_CASE("gram schmidt")
{
    matrix<double> gs(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto outgs = transformation::GS::QR(gs);
    
    print_matrix(&outgs.first);
    std::cout << "\n\n";
    
    print_matrix(&outgs.second);
    std::cout << "\n\n";
}*/

TEST_CASE("householder")
{
    matrix<double> hh(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto outhh = transformation::house::QR(hh);
    
/*
    print_matrix(&outhh.first);
    
    std::cout << "\n\n";
    
    print_matrix(&outhh.second);
    
    std::cout << "\n\n";
    
    auto smallchk = mat_mul_alg1(&outhh.first, &outhh.second);
    
    print_matrix(&smallchk);
    
    std::cout << "\n\n";
    */
    
    matrix<double> hhbig = matrix<double>::random_dense_matrix(50, 13, 1000, -1000);
    matrix<double> hhbigcpy(hhbig);
    print_matrix(&hhbig);
    
    auto bigout = transformation::house::householder(hhbig);
    /*
    //print_matrix(&bigout.first);
    std::cout << "\n\n\n\n";

    //print_matrix(&bigout.second);
    
    matrix<double> chk = mat_mul_alg1(&bigout.first, &bigout.second);
    
    print_matrix(&chk);
    
    matrix<double> absdiff(50, 14);
    for(size_t i=0; i < absdiff.size(); i++)
    {
        absdiff[i] = std::abs(hhbigcpy[i]) - std::abs(chk[i]);
        if(absdiff[i] < 0.0000001)
        {
            absdiff[i] = 0.0;
        }
    }
    
    std::cout << "\n\n";
    
    print_matrix(&absdiff);
    
    std::cout << "\n\n";
    
    //print_matrix(&bigout.second);
    */
    
    
}
