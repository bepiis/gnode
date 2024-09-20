//
//  gram_schmidt.h
//  mmnn
//
//  Created by Ben Westcott on 9/18/24.
//

#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include "matrix.hpp"

namespace transformation
{

namespace GS
{

/*
 * Determines the QR factorization of a non-singular system V
 * via modified Gram Schmidt (MGS).
 *
 * Only recommended for use in iterative methods due to less
 * numerical stability than householder, and jacobi/givens transformations
 */
std::pair<matrix<double>, matrix<double>> QR(const matrix<double>& V)
{
    matrix<matrix<double>> cvecs = as_cvecs(V);
    matrix<matrix<double>> Qc(cvecs);
    
    size_t M = cvecs.cols();
    matrix<double> R(M, M);

    for(size_t j=0; j < M; j++)
    {
        R(j, j) = matrix<double>::col_norm2(cvecs(0, j), 0);
        Qc(0, j) = (1/R(j, j)) * cvecs(0, j);
        
        for(size_t k=j+1; k < M; k++)
        {
            R(j, k) = inner_prod_1D(Qc(0, j), cvecs(0, k));
            cvecs(0, k) -= R(j, k) * Qc(0, j);
        }
    }
    
    matrix<double> Q = from_cvecs(Qc);
    
    for(size_t j=0; j < M; j++)
    {
        delete Qc(0, j).base_ptr();
        delete cvecs(0, j).base_ptr();
    }
    
    return std::make_pair(Q, R);
}

/*
 * modified Gram-Schmidt (MGS) othonormalizes the column space of
 * a non-singular system V.
 */
matrix<double> MGS(const matrix<double>& V)
{
    std::pair<matrix<double>, matrix<double>> res_pair = QR(V);
    delete res_pair.second.base_ptr();
    
    return res_pair.first;
}


}




}

#endif /* GRAM_SCHMIDT_H */
