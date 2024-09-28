//
//  main.cpp
//  newmat
//
//  Created by Ben Westcott on 9/25/24.
//

#include <iostream>
#include "matrix.h"
#include "products.h"
#include "householder.h"

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

int main(int argc, const char * argv[])
{
    matrix<double> hhbig = matrix<double>::random_dense_matrix(50, 20, 1000, -1000);
    //matrix<double> hhbig(5, 3, {-468.92, -383.638, 591.607, -663.279, 833.265, -612.95, -817.78, -704.544, -740.979, 37.389, -150.828, -323.814, -872.728, -222.411, 589.552});
    matrix<double> hhbigcpy(hhbig);
    print_matrix(&hhbig);
    
    std::cout << "\n\n";
    
    auto bigout = transformation::house::QR(hhbig);
    
    print_matrix(&bigout.first);
    std::cout << "\n\n";
    print_matrix(&bigout.second);
    auto res = mat_mul_alg1(&bigout.first, &bigout.second);
    
    print_matrix(&bigout.second);
    std::cout << "\n\n\n\n";
    
    print_matrix(&bigout.first);
    std::cout << "\n\n\n\n";
    
    matrix<double> absdiff(50, 20);
    for(size_t i=0; i < absdiff.size(); i++)
    {
        absdiff[i] = std::abs(res[i]) - std::abs(hhbigcpy[i]);
        if(absdiff[i] < 10E-9)
        {
            absdiff[i] = 0;
        }
    }
    
    print_matrix(&absdiff);
    
    matrix<double> hh(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto outhh = transformation::house::QR(hh);
    
    std::cout << "\n\n";
    
    print_matrix(&outhh.first);
    
    std::cout << "\n\n";
    
    print_matrix(&outhh.second);
    
    /*
    
    matrix<double> hessbig = matrix<double>::random_dense_matrix(50, 50, 1000, -1000);
    matrix<double> hessbigcpy(hessbig);
    
    auto hessout = transformation::house::hessenberg(hessbig);
    
    matrix<double> Q = transformation::house::Qaccumulate(hessout.first, hessout.second, hessout.first.cols(), 1);
    matrix<double> QT = Q.transpose();
    
    
    //hessout.first.set_lower_hessenberg();
    for(size_t c=0; c < hessout.first.cols() - 2; c++)
    {
        for(size_t r = c + 2; r < hessout.first.rows(); r++)
        {
            hessout.first(r, c) = 0.0;
        }
    }
    
    print_matrix(&hessout.first);
    
    
    
    matrix<double> res2 = mat_mul_alg1(&Q, &hessout.first);
    
    
    matrix<double> res3 = mat_mul_alg1(&res2, &QT);
    //matrix<double> res = mat_mul_alg1(&res2, &QT);
   // std::cout << "\n\n\n\n";
    //print_matrix(&QT);
    
    print_matrix(&res3);
    
    matrix<double> absdiff(50, 50);
    for(size_t i=0; i < absdiff.size(); i++)
    {
        absdiff[i] = std::abs(res3[i]) - std::abs(hessbigcpy[i]);
        if(absdiff[i] < 10E-9)
        {
            absdiff[i] = 0;
        }
    }
    
    print_matrix(&absdiff);*/

    
    
}


/*
int main(int argc, const char * argv[])
{
    
    // default constructor
    std::cout << "default constructor: \n";
    
    matrix<double> md;
    std::cout << "\n\n";
    
    // size w/ data
    std::cout << "size and data constructor: \n";
    
    const double dat0[9] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
    matrix<double> msd(9, dat0);
    std::cout << "\n\n";

    
    // row and cols
    std::cout << "row and col constructor: \n";

    matrix<double> mrc(3, 3);
    std::cout << "\n\n";

    
    // rows, cols, data
    std::cout << "row, col, data constructor: \n";

    matrix<double> mrcd(3, 3, dat0);
    std::cout << "\n\n";

    
    // copy constructor
    std::cout << "copy constructor: \n";

    matrix<double> mcpy(mrcd);
    std::cout << "\n\n";
    
    // initializer list
    std::cout << "initializer list constructor: \n";
    matrix<double> mi({1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << "\n\n";
    
    // size, initializer list
    std::cout << "size, initializer list constructor: \n";

    matrix<double> msi(9, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << "\n\n";

    
    // rows, cols, initializer list
    std::cout << "row, col, initializer list: \n";
    
    matrix<double> mrci(3, 3, {2, 4, 6, 8, 10, 12, 14, 16, 18});
    std::cout << "\n\n";

    
    // copy assignment
    std::cout << "copy assignment: \n";

    matrix<double> mrcicpy = mrci;
    
    std::cout << "\n\n";

    
    // move constructor
    std::cout << "move constructor: \n";

    matrix<double> msimove(std::move(msi));
    std::cout << "\n\n";

    
    
}
*/
