//
//  householder.h
//  mmnn
//
//  Created by Ben Westcott on 9/12/24.
//

#ifndef HOUSEHOLDER_H
#define HOUSEHOLDER_H

#include "matrix.hpp"
#include <cmath>

// refs:
// [1] Matrix Computations 4th ed. Golub, Van Loan
// [2] https://en.wikipedia.org/wiki/Householder_transformation
// [3] https://www.cs.cornell.edu/~bindel/class/cs6210-f12/notes/lec16.pdf
// [4] https://nhigham.com/2020/09/15/what-is-a-householder-matrix/

namespace transformation
{

namespace house
{

template<class T>
void ppprint_matrix(matrix<T>& mat)
{
    for(int i=0; i < mat.rows(); i++)
    {
        for(int j=0; j < mat.cols(); j++)
        {
            std::cout << mat.get_value(i, j) << "\t";
        }
        std::cout << "\n";
    }
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
std::pair<matrix<double>, double> house(const matrix<double>& cvec)
{
    double sig = inner_prod_1D(cvec, cvec, 1);
    double beta, mu;
    
    matrix<double> hv(cvec);
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
    }
    
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
std::pair<matrix<double>&, matrix<double>> householder(matrix<double>& A)
{
    size_t M = A.rows();
    size_t N = A.cols();
    
    std::pair<matrix<double>, double> phouse;
    matrix<double> Asub;
    matrix<double> B(1, N - 1);
    matrix<double> vhouse;
    
    for(size_t j=0; j < N; j++)
    {
        // 3m flops
        phouse = house(A.sub_col(j, M - j, j));
        
        matrix<double> vhouse = phouse.first;
        double beta = phouse.second;
        B(0, j) = beta;
        
        Asub = A.sub_matrix(j, M - j, j, N - j);
        
        // TODO: does tranpose to column vector here, which doesnt need to happen since inner product just takes directly from bptr anyways
        // 2mn + 2n flops
        Asub -= outer_prod_1D((beta * vhouse), inner_left_prod(vhouse, Asub));
        
        A.set_sub_matrix(Asub, j, j);
        
        if(j < M)
        {
            size_t i=1;
            for(size_t k = j+1; k < M; k++, i++)
            {
                A(k, j) = vhouse(i, 0);
            }
        }
        
    }
    
    delete Asub.base_ptr();
    delete phouse.first.base_ptr();
    delete vhouse.base_ptr();
    
    return std::make_pair(std::reference_wrapper(A), B);
}

/*
 * Accumulates the matrix Q = Q0 * Q1 * ... Qn
 * from its factorized form, being the lower triangle of R, and B
 * where the LT of R contains essential house vectors, and B is a vector
 * of corresponding beta coeffs
 *
 * TODO: work with k optimization
 */
matrix<double> Qaccumulate(const matrix<double>& R, const matrix<double>& B, size_t k)
{
    size_t M = R.rows();
    size_t N = R.cols();
    
    int n = (int)N;
    
    //std::cout << "betalen = " << B.cols() << "\n";
    
    matrix<double> Im = matrix<double>::identity(M);
    matrix<double> Q = Im.sub_matrix(0, M, 0, k);
    matrix<double> vhouse;
    
    matrix<double> Qsub;
    
    for(int j = n - 2; j >= 0; j--)
    {
        vhouse = matrix<double>(M - j, 1);
        vhouse(0, 0) = 1.0;
        
        //matrix<double> Ajp1 = R.sub_col(j + 1, M - j - 1, j);
        vhouse.set_sub_col(R.sub_col(j + 1, M - j - 1, j), 1, 0);
        
        Qsub = Q.sub_matrix(j, Q.rows()-j, j , Q.cols()-j);
        
        //double beta = 2/(1+matrix<double>::col_norm2sq_from(Ajp1, 0, 0));
        // 2mn + 2n FLOPS
        //std::cout << "beta" << j << " = " << B(0, j) << "\n";
        
        // Q <- (Im - beta*v*v^T)Q
        Qsub -= B(0, j) * outer_prod_1D(vhouse, inner_left_prod(vhouse, Qsub));
        

        Q.set_sub_matrix(Qsub, j, j);
        
    }
    
    delete Im.base_ptr();
    delete vhouse.base_ptr();
    delete Qsub.base_ptr();
    
    return Q;
}

// TODO: Allow for Qaccumulate to obtain Q for hessenberg decomp.
std::pair<matrix<double>&, matrix<double>> hessenberg(matrix<double> &A /* must be square*/ )
{
    size_t N = A.rows();
    std::pair<matrix<double>, double> phouse;
    matrix<double> vhouse, Ablk, Apar;
    matrix<double> B(0, N - 2);
    
    for(size_t k=0; k < N - 2; k++)
    {
        phouse = house(A.sub_col(k + 1, N - k - 1, k));
        
        vhouse = phouse.first;
        double beta = phouse.second;
        B(0, k) = beta;
        
        Ablk = A.sub_matrix(k + 1, N - k - 1, k, N - k);
        
        // A <- QA
        Ablk -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Ablk));
        
        A.set_sub_matrix(Ablk, k + 1, k);
        
        Apar = A.sub_matrix(0, N, k + 1, N - k - 1);
        
        // A <- A(Q^T)
        Apar -= beta * outer_prod_1D(inner_right_prod(Apar, vhouse), vhouse.transpose());
        
        A.set_sub_matrix(Apar, 0, k + 1);
        
        
        // as usual, store essential house vectors where zeros
        // have been introduced
        size_t i=1;
        for(size_t j = k + 2; j < N; j++, i++)
        {
            A(j, k) = vhouse(i, 0);
        }
        
    }
    
    return std::make_pair(std::reference_wrapper(A), B);
}

/*
 * Uses the above two methods to obtain the traditional QR factorization.
 * Note that explicitly forming Q is usually not needed, and thus
 * using this fn instead of householder() is usually not needed, saving the
 * extra computation required to un-factorize Q
 */
std::pair<matrix<double>, matrix<double>> QR(const matrix<double>& A)
{
    matrix<double> R(A);
    std::pair<matrix<double>&, matrix<double>> house_result = householder(R);
    
    matrix<double> Q = Qaccumulate(house_result.first, house_result.second, house_result.first.cols());
    
    // cleanup
    R.zero_lower_triangle();
    delete house_result.second.base_ptr();
    
    return std::make_pair(Q, R);
}


//[[maybe_unused]]
matrix<double> householder_vector(size_t k, const matrix<double>& Ak)
{
    //double apk = std::pow(Ak(k+1, k), k+1);
    double apk = Ak(k+1, k);
    
    matrix<double> hv(Ak.rows(), 1);
    hv.zero();
    
    double nsq = matrix<double>::col_norm2sq_from(Ak, k, k + 1);
    
    double bk = -1.0 * msgn(apk) * std::sqrt(nsq);
    
    double r = std::sqrt(0.5*(std::pow(bk, 2) - apk*bk));
    
    hv(k + 1, 0) = (apk - bk)/(2*r);
    
    for(size_t j=k+2; j < hv.rows(); j++)
    {
        hv(j, 0) = Ak(j, k)/(2*r);
    }
    
    return hv;
}

matrix<double> householder_matrix(matrix<double>& hv)
{
    matrix<double> hm = matrix<double>::identity(hv.rows());
    
    matrix<double> hvT = hv.transpose();
    matrix<double> ho = outer_prod_1D(hv, hvT);
    
    hm -= 2.0 * ho;
    return hm;
}

matrix<double> householder_iteration(size_t k, const matrix<double>& A)
{
    matrix<double> hv = householder_vector(k, A);
    matrix<double> hm = householder_matrix(hv);
    
    matrix<double> rprod = mat_mul_alg1(&A, &hm);
    matrix<double> res = mat_mul_alg1(&hm, &rprod);
    
    return res;
}

}

}


#endif /* HOUSEHOLDER_H */
