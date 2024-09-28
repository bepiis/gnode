//
//  householder.h
//  Created by Ben Westcott on 9/12/24.
//

#pragma once

#include "matrix.h"
#include "products.h"

// refs:
// [1] Matrix Computations 4th ed. Golub, Van Loan
// [2] https://en.wikipedia.org/wiki/Householder_transformation
// [3] https://www.cs.cornell.edu/~bindel/class/cs6210-f12/notes/lec16.pdf
// [4] https://nhigham.com/2020/09/15/what-is-a-householder-matrix/

namespace transformation
{

namespace house
{

// TODO: Just overload ostream operator<< so I dont have to keep writing print fns all over the place.
template<class T>
void pprint_matrix(const matrix<T>& mat)
{
    for(int i=0; i < mat.rows(); i++)
    {
        for(int j=0; j < mat.cols(); j++)
        {
            std::cout << mat.get_value(i, j) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

/*
 * Determines the vector v and constant beta from input x = cvec such that
 * for matrix P = I(m) - beta*v*(v^T), Px = norm2(x)I(:, 1) = norm2(x)e1
 * and P is orthogonal.
 *
 * house(x) more specifically computes the essential form of v since
 * for each householder iteration, v will append a zero to the top of v
 * which doesnt need to be stored.
 *
 * lastly, v is normalized, i.e. v(0) = 1 so that v can be conviently stored in
 * the lower triangle of upper triangular matrix R, where zeros have been introduced
 *
 */
// TODO: get rid of all pair return types.. I think I hate them...
std::pair<matrix<double>, double> house(const matrix<double>& cvec)
{
    double sig = inner_prod_1D(cvec, cvec, 1);
    double beta, mu;
    
    //std::cout << "house(): \n";
    
    matrix<double> hv(cvec);
    //std::cout << "house, cvec: " << cvec.size() << "\n";
    //pprint_matrix(cvec);
    //std::cout << "\n\n";
    hv(0, 0) = 1.0;
    
    double c0 = cvec(0, 0);
    double h0 = hv(0, 0);
    
    if(sig == 0 && c0 >= 0)
    {
        beta = 0;
    }
    else if(sig == 0 && c0 < 0)
    {
        beta = -2;
    }
    else
    {
        mu = std::sqrt(c0 * c0 + sig);
        if(c0 <= 0)
        {
            hv(0, 0) = c0 - mu;
        }
        else
        {
            hv(0, 0) = -sig/(c0 + mu);
        }
        
        h0 = hv(0, 0);
        beta = 2*(h0*h0)/(sig + h0*h0);
        hv = (1/h0) * hv;
        
        //std::cout << "\tmu = " << mu << "\n";

    }
    
    //std::cout << "\tsig = " << sig << "\n";
    //std::cout << "\tbeta = " << beta << "\n\n\n";
    
    
    // Cost: roughly 3m flops
    return std::make_pair(hv, beta);
}


/*
 * Computes the QR factorization of input matrix A
 * and returns upper triangular matrix R and beta coefficients
 *
 * note that the lower triangle of R contains the essential house vectors
 * and thus the return type contains the factorized form of Q, i.e beta, v
 */
matrix<double>& householder(matrix<double>& A)
{
    size_t M = A.rows();
    size_t N = A.cols();
    
    std::pair<matrix<double>, double> phouse;
    matrix<double> Asub;
    matrix<double> vhouse;
    
    for(size_t j=0; j < N; j++)
    {
        // 3m flops
        //std::cout << "sub_col: " << j << "\n";
        //pprint_matrix(A.sub_col(j, M-j, j));
        //std::cout << "\n\n";
        
        phouse = house(A.sub_col(j, M - j, j));
        
        //std::cout << "vhouse" << j << " = \n";
        //vhouse.print();
        //pprint_matrix(phouse.first);
        
        matrix<double> vhouse = phouse.first;
        double beta = phouse.second;
        
        Asub = A.sub_matrix(j, M - j, j, N - j);
        
        //std::cout << "Qsub" << j << " = \n";
        //Qsub.print();
        //pprint_matrix(Asub);
        
        //std::cout << "beta = " << beta << "\n\n";
    

        //std::cout << "inner_left_prod " << j << "\n";
        //pprint_matrix(inner_left_prod(vhouse, Asub));
        
        //std::cout << "outer_prod " << j << "\n";
        //pprint_matrix(outer_prod_1D(vhouse, inner_left_prod(vhouse, Asub)));
        
        // 2mn + 2n flops
        Asub -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Asub));
        
        //std::cout << "Qsubp" << j << " = \n";
        //Qsub.print();
        //pprint_matrix(Asub);
        
        A.set_sub_matrix(Asub, j, j);
        
        if(j < M)
        {
            size_t i=1;
            for(size_t k = j+1; k < M; k++, i++)
            {
                A(k, j) = vhouse(i, 0);
            }
        }
        
        //std::cout << "A" << j << " = \n";
        //Qsub.print();
        //pprint_matrix(A);
        
    }
    
    return A;
}

void colpiv_householder(matrix<double>& A)
{
    matrix<double> c = cols_norm2sq(A);
    matrix<size_t> piv = matrix<size_t>::unit_permutation_matrix(A.cols());
    
    size_t r = 0;
    double tau = matrix<double>::abs_max_element(c, 0);
    
    size_t M = A.rows();
    size_t N = A.cols();
    
    std::pair<matrix<double>, double> phouse;
    matrix<double> Asub;
    matrix<double> Im = matrix<double>::eye(M);
    
    while(tau > 0 && r < N)
    {
        
        // looking for first index k which satisfies c(k) = tau
        size_t k = r;
        for(; k < N; k++)
        {
            if(c(0, k) == tau)
            {
                break;
            }
        }
        
        piv(r, 0) = k;
        
        A.swap_cols(r, k);
        c.swap_cols(r, k);
                
        phouse = house(A.sub_col(r, M - r, r));
        double beta = phouse.second;
        
        Asub = A.sub_matrix(r, M-r, r, N-r);
        
        Asub -= beta * outer_prod_1D(phouse.first, inner_left_prod(phouse.first, Asub));
        
        size_t i = 1;
        for(size_t j = r + 1; j < M; j++, i++)
        {
            A(j, r) = phouse.first(i, 0);
        }
        
        // update norm vector c by subtrating out A(r, :)**2
        for(size_t j = r + 1; j < N; j++)
        {
            c(0, j) -= A(r, j) * A(r, j);
        }
        
        tau = matrix<double>::abs_max_element(c, ++r);
    }
    
}

/*
 * Accumulates the matrix Q = Q0 * Q1 * ... Qn
 * from its factorized form, being the lower triangle of R, and B
 * where the LT of R contains essential house vectors, and B is a vector
 * of corresponding beta coeffs
 *
 * Works with QR and QH (hessenberg) reductions.
 *      For QR reductions, col_bias = 0
 *      For QH reductions, col_bias = 1
 *          In hessenburg reduction, the jth house vector gets stored in col j - 1
 *          since col j is one index too short to store the entire vector.
 *
 * Q *= Qj for j = 0, 1, 2, ... n - 1
 *
 * TODO: work with k optimization
 */
matrix<double> Qaccumulate(const matrix<double>& R, size_t k, size_t col_bias)
{
    size_t M = R.rows();
    size_t N = R.cols();
    
    int n = (int)N;
    
    //std::cout << "betalen = " << B.cols() << "\n";
    
    matrix<double> Im = matrix<double>::eye(M);
    matrix<double> Q = Im.sub_matrix(0, M, 0, M);
    matrix<double> vhouse;
    
    matrix<double> Qsub;
    
    for(int j = n - 1 - (int)col_bias; j >= 0; j--)
    {
        vhouse = matrix<double>(M - j - col_bias, 1);
        vhouse(0, 0) = 1.0;
        
        matrix<double> Ajp1 = R.sub_col(j + 1 + col_bias, M - j - 1 - col_bias, j);
        
        vhouse.set_sub_col(Ajp1, 1, 0);
        
        //std::cout << "vhouse" << j << " = \n";
        //vhouse.print();
        //pprint_matrix(vhouse);

        Qsub = Q.sub_matrix(j + col_bias, Q.rows()-j - col_bias, j + col_bias , Q.rows()-j - col_bias);
        
        //std::cout << "Qsub" << j << " = \n";
        //Qsub.print();
        //pprint_matrix(Qsub);
        
        //std::cout << "beta" << j << " = " << B(0, j) <<  "\n";
        double beta = 2/(1+col_norm2sq_from(Ajp1, 0, 0));
        // 2mn + 2n FLOPS
        //std::cout << "beta" << j << " = " << B(0, j) << "\n";

        // Q <- (Im - beta*v*v^T)Q
        Qsub -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Qsub));
        
        //std::cout << "Qsubp" << j << " = \n";
        //pprint_matrix(Qsub);
        //Qsub.print();

        Q.set_sub_matrix(Qsub, j + col_bias, j + col_bias);
        
    }
    
    
    return Q;
}


/*
 * Computes an upper hessenberg reduction for a N x N square matrix A
 * and stores essential house vectors in the zeroed portion of A (lower hess))
 * note that house vectors will be offset to the left by one column since A becomes upper hessenberg.
 * Thus, if Qaccumulate needs to be used, col_bias = 1
 *
 * A <- QAQ^T, where Q is orthogonal, and A becomes upper hessenberg
 */
matrix<double>& hessenberg(matrix<double> &A /* must be square*/ )
{
    size_t N = A.rows();
    std::pair<matrix<double>, double> phouse;
    matrix<double> vhouse, Ablk, Apar;
    
    for(size_t k=0; k < N - 2; k++)
    {
        phouse = house(A.sub_col(k + 1, N - k - 1, k));
        
        vhouse = phouse.first;
        double beta = phouse.second;
        //std::cout << "beta = " << beta << "\n";
        
        Ablk = A.sub_matrix(k + 1, N - k - 1, k, N - k);
        
        //std::cout<< "Ablk = \n";
        //Ablk.print();
        // A <- QA
        Ablk -= outer_prod_1D(beta * vhouse, inner_left_prod(vhouse, Ablk));
        
        A.set_sub_matrix(Ablk, k + 1, k);
        
        //std::cout << "vhouse = \n";
        //vhouse.print();
        
        //std::cout<< "A = \n";
        //A.print();
        
        Apar = A.sub_matrix(0, N, k + 1, N - k - 1);
        
        //std::cout<< "Apar = \n";
        //Apar.print();
        
        // A <- A(Q^T)
        Apar -= outer_prod_1D(inner_right_prod(Apar, vhouse), beta * /*TODO: */vhouse.transpose());
        

        
        A.set_sub_matrix(Apar, 0, k + 1);
        
        //std::cout << "Ap = \n";
        //A.print();
        
        // as usual, store essential house vectors where zeros
        // have been introduced
        
        size_t i=1;
        for(size_t j = k + 2; j < N; j++, i++)
        {
            A(j, k) = vhouse(i, 0);
        }
        
    }
    
    return A;
}

/*
 * Uses the above householder and Qaccumulate to obtain the traditional QR factorization.
 * Note that explicitly forming Q is usually not needed, and thus
 * using this fn instead of householder() is usually not needed, saving the
 * extra computation required to un-factorize Q
 */
std::pair<matrix<double>, matrix<double>> QR(const matrix<double>& A)
{
    matrix<double> R(A);
    matrix<double>& house_result = householder(R);
    
    matrix<double> Q = Qaccumulate(house_result, house_result.rows(), 0);
    
    // cleanup
    R.fill_lower_triangle(0.0);
    
    return std::make_pair(Q, R);
}

}

}
