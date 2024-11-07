//
//  gram_schmidt.h
//  Created by Ben Westcott on 9/18/24.
//

#pragma once

#include "matrix.h"
#include "products.h"
#include "result.h"

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
result::QR<double> QR(const matrix<double>& X)
{
    matrix<double> V(X);
    matrix<double> Qc(V.rows(), V.cols());
    
    size_t M = V.cols();
    matrix<double> R(M, M);

    for(size_t j=0; j < M; j++)
    {
        R(j, j) = std::sqrt(col_norm2sq(V, j));
        
        matrix<double> curr_col = (1/R(j, j)) * V.col(j);
        
        Qc.set_sub_col(curr_col, 0, j);
        
        for(size_t k=j+1; k < M; k++)
        {
            R(j, k) = inner_prod_1D(curr_col, V.col(k));
            //cvecs(0, k) -= R(j, k) * Qc(0, j);
            matrix<double> sub = V.col(k);
            sub -= R(j, k) * curr_col;
            
            V.set_sub_col(sub, 0, k);
        }
    }
    
    //matrix<double> Q = from_cvecs(Qc);
    
    return result::QR<double>(Qc, R);
}

/*
 * modified Gram-Schmidt (MGS) othonormalizes the column space of
 * a non-singular system V.
 */
matrix<double> MGS(const matrix<double>& V)
{
    result::QR<double> res_pair = QR(V);
    return res_pair.Y;
}


}
}
