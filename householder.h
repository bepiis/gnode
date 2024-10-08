//
//  householder.h
//  Created by Ben Westcott on 9/12/24.
//

#pragma once

#include <cstdint>
#include "matrix.h"
#include "products.h"
#include "result.h"

// refs:
// [1] Matrix Computations 4th ed. Golub, Van Loan
// [2] https://en.wikipedia.org/wiki/Householder_transformation
// [3] https://www.cs.cornell.edu/~bindel/class/cs6210-f12/notes/lec16.pdf
// [4] https://nhigham.com/2020/09/15/what-is-a-householder-matrix/

namespace transformation
{

namespace house
{

struct house
{
    matrix<double> vec;
    double beta;
    
    house() = default;
    
    house(matrix<double> const& v, double b)
    : vec(v), beta(b) {}
};


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
house housevec(matrix<double> const& cvec, size_t a)
{
    //double sig = inner_prod_1D(cvec, cvec, 1);
    double sig = 0.0;
    for(size_t j=0; j < cvec.size(); j++)
    {
        if(j == a)
        {
            continue;
        }
        sig += (cvec[j] * cvec[j]);
    }
    
    double beta, mu;
        
    matrix<double> hv(cvec);
    hv[a] = 1.0;

    double c0 = cvec[a];
    double h0 = hv[a];
    
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
            hv[a] = c0 - mu;
        }
        else
        {
            hv[a] = -sig/(c0 + mu);
        }
        
        h0 = hv[a];
        beta = 2*(h0*h0)/(sig + h0*h0);
        hv = (1/h0) * hv;
    }
    
    // Cost: roughly 3m flops
    return house(hv, beta);
}

house houseinit(matrix<double>& A, size_t& M, size_t& N, size_t& n)
{
    M = A.rows();
    N = A.cols();
    n = N;
    
    if(N == M)
    {
        n--;
    }
    
    return house();
}

matrix<double>& housestep(matrix<double>& A, house& h, size_t j)
{
    h = housevec(A.sub_col(j, A.rows() - j, j), 0);
    
    matrix<double> Asub = A.sub_matrix(j, A.rows() - j, j, A.cols() - j);
    Asub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Asub));
    
    A.set_sub_matrix(Asub, j, j);
    
    return A;
}

matrix<double>& colstep(matrix<double>& A, house& h, size_t i, size_t k, size_t hc, size_t s)
{
    h = housevec(A.sub_col(k, A.rows() - i, hc), s);
    
    matrix<double> Asub = A.sub_matrix(k, A.rows() - i, k, A.cols() - i);
    
    Asub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Asub));
    
    A.set_sub_matrix(Asub, k, k);
    return A;
}

matrix<double>& QRstep(matrix<double>& A, house& h, size_t i)
{
    return colstep(A, h, i, i, i, 0);
}

/*
 * Computes the QR factorization of input matrix A
 * and returns upper triangular matrix R and beta coefficients
 *
 * note that the lower triangle of R contains the essential house vectors
 * and thus the return type contains the factorized form of Q, i.e beta, v
 */
matrix<double>& QRfast(matrix<double>& A)
{
    size_t M, N, n;
    house h = houseinit(A, M, N, n);
    
    for(size_t j=0; j < n; j++)
    {
        A = QRstep(A, h, j);

        if(j < M)
        {
            size_t i=1;
            for(size_t k = j+1; k < M; k++, i++)
            {
                A(k, j) = h.vec(i, 0);
            }
        }
    }
    
    return A;
}

/*
 * Accumulates the matrix Q = Q0 * Q1 * ... Qn
 * from its factorized form, being the lower triangle of R, and B
 * where the LT of R contains essential house vectors, and B is a vector
 * of corresponding beta coeffs
 *
 * Works with QR and QHR (hessenberg) reductions.
 *      For QR reductions, col_bias = 0
 *      For QH reductions, col_bias = 1
 *          In hessenburg reduction, the jth house vector gets stored in col j - 1
 *          since col j is one index too short to store the entire vector.
 *
 * Q *= Qj for j = 0, 1, 2, ... n - 1
 *
 * TODO: work with k optimization
 */
matrix<double> QRaccumulate(matrix<double> const& F, size_t k, size_t col_bias)
{
    size_t M = F.rows();
    size_t N = F.cols();
    
    int64_t n = (int64_t)N;
    
    if(M == N)
    {
        n--;
    }
    //std::cout << "betalen = " << B.cols() << "\n";
    
    matrix<double> Im = matrix<double>::eye(M);
    matrix<double> Q = Im.sub_matrix(0, M, 0, M);
    matrix<double> vhouse;
    
    matrix<double> Qsub;
    
    for(int64_t j = n - 1 - (int64_t)col_bias; j >= 0; j--)
    {
        vhouse = matrix<double>(M - j - col_bias, 1);
        vhouse(0, 0) = 1.0;
        
        matrix<double> Fjp1 = F.sub_col(j + 1 + col_bias, M - j - 1 - col_bias, j);
        
        vhouse.set_sub_col(Fjp1, 1, 0);

        Qsub = Q.sub_matrix(j + col_bias, Q.rows()-j - col_bias, j + col_bias , Q.rows()-j - col_bias);
        
        double beta = 2/(1+col_norm2sq_from(Fjp1, 0, 0));

        // Q <- (Im - beta*v*v^T)Q
        Qsub -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Qsub));
        
        Q.set_sub_matrix(Qsub, j + col_bias, j + col_bias);

    }
    return Q;
}

/*
 * Uses the above householder and Qaccumulate to obtain the traditional QR factorization.
 * Note that explicitly forming Q is usually not needed, and thus
 * using this fn instead of householder() is usually not needed, saving the
 * extra computation required to un-factorize Q
 */
result::QR<double> QR(matrix<double> const& A)
{
    // TODO: add exception throw for M < N
    result::QR<double> res;
    
    res.Y = matrix<double>(A);
    res.Y = QRfast(res.Y);
    
    res.Q = QRaccumulate(res.Y, res.Y.rows(), 0);
    
    // cleanup
    res.Y.fill_lower_triangle(0.0);
    
    return res;
}

/*
 * Computes an upper hessenberg reduction for a N x N square matrix A
 * and stores essential house vectors in the zeroed portion of A (lower hess))
 * note that house vectors will be offset to the left by one column since A becomes upper hessenberg.
 * Thus, if Qaccumulate needs to be used, col_bias = 1
 *
 * A <- QAQ^T, where Q is orthogonal, and A becomes upper hessenberg
 */
matrix<double>& QRHfast(matrix<double> &A /* must be square*/ )
{
    size_t N = A.rows();
    house h;
    matrix<double> Ablk, Apar;
    
    for(size_t k=0; k < N - 2; k++)
    {
        h = housevec(A.sub_col(k + 1, N - k - 1, k), 0);
                
        Ablk = A.sub_matrix(k + 1, N - k - 1, k, N - k);
        
        //std::cout << "Ablk:\n";
        //std::cout << Ablk << "\n";
        
        // A <- QA
        Ablk -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Ablk));
        
        A.set_sub_matrix(Ablk, k + 1, k);
        
        Apar = A.sub_matrix(0, N, k + 1, N - k - 1);
        
        //std::cout << "Apar:\n";
        //std::cout << Apar << "\n";
        
        // A <- A(Q^T)
        Apar -= h.beta * outer_prod_1D(inner_right_prod(Apar, h.vec), h.vec);
        
        A.set_sub_matrix(Apar, 0, k + 1);
        
        size_t i=1;
        for(size_t j = k + 2; j < N; j++, i++)
        {
            A(j, k) = h.vec(i, 0);
        }
    }
    return A;
}

result::QRH<double> QRH(matrix<double> const& A)
{
    // A must be square.
    result::QRH<double> res;
    
    res.H = matrix<double>(A);
    res.H = QRHfast(res.H);
    
    res.Q = QRaccumulate(res.H, res.H.rows(), 1);
    
    res.H.fill_lower_hessenberg(0.0);
    
    return res;
}

result::FPr<double> colpiv_QRfast(matrix<double>& A)
{
    size_t M, N, n;
    house h = houseinit(A, M, N, n);
    
    matrix<double> c = cols_norm2sq(A);
    matrix<size_t> piv = matrix<size_t>::unit_permutation_matrix(A.cols());
    
    size_t r = 0;
    double tau = matrix<double>::abs_max_element(c, 0);
    
    while(tau > 0 && r < n)
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
        
        //piv(r, 0) = k;
        piv.swap_rows(r, k);
        A.swap_cols(r, k);
        c.swap_cols(r, k);
        
        A = QRstep(A, h, r);
        
        if(r < M)
        {
            size_t i = 1;
            for(size_t j = r + 1; j < M; j++, i++)
            {
                A(j, r) = h.vec(i, 0);
            }
        }
        
        // update norm vector c by subtrating out A(r, :)**2
        for(size_t j = r + 1; j < N; j++)
        {
            c(0, j) -= A(r, j) * A(r, j);
        }
        
        tau = matrix<double>::abs_max_element(c, ++r);
    }
    
    return result::FPr<double>(A, piv, r);
}

matrix<double>& QLstep(matrix<double>& A, house& h, size_t i)
{
    return colstep(A, h, i, 0, A.cols() - i - 1, A.rows() - i - 1);
}

matrix<double>& QLfast(matrix<double>& A)
{
    size_t M, N, n;
    house h = houseinit(A, M, N, n);
    
    for(size_t j=0; j < n; j++)
    {
        A = QLstep(A, h, j);
        
        //std::cout << A << "\n";
        
        //std::cout << "vhouse" << j << "\n";
        //std::cout << h.vec << "\n";
        
        if(j < M)
        {
            for(size_t k=0; k < M - j - 1; k++)
            {
                A(k, N - j - 1) = h.vec(k, 0);
            }
        }
    }
    
    return A;
}

matrix<double> QLaccumulate(matrix<double> const& F, size_t col_bias)
{
    size_t M = F.rows();
    size_t N = F.cols();
    
    int64_t n = (int64_t)N;
    int64_t end_cond = 0;
    int64_t nhrows = M;
    int64_t cb = (int64_t)col_bias;
    
    if(M == N)
    {
        end_cond++;
    }
    
    matrix<double> Q = matrix<double>::eye(M);
    matrix<double> vhouse;
    matrix<double> Qsub;
    
    //std::cout << "F = \n";
    //std::cout << F << "\n";
    
    for(int64_t j = n - 1 - cb; j >= end_cond; j--)
    {
        vhouse = matrix<double>(nhrows - cb, 1);
        vhouse(nhrows - 1 - cb, 0) = 1.0;
        
        //std::cout << "nhcols: " << nhcols << "\n";
        matrix<double> hj = F.sub_col(0, nhrows - 1 - cb, j + cb);
        
        vhouse.set_sub_col(hj, 0, 0);
        //std::cout << "vhouse: \n";
        //std::cout << vhouse << "\n";
        
        Qsub = Q.sub_matrix(0, M - cb, 0, nhrows - cb);
        
        //std::cout << "Qsub: \n";
        //std::cout << Qsub << "\n";
        //std::cout << vhouse << "\n";
        
        double beta = 2/(1 + col_norm2sq_from(hj, 0, 0));
        
        //Qsub -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Qsub));
        Qsub -= outer_prod_1D(inner_right_prod(Qsub, vhouse), beta * vhouse);
        
        //std::cout << "Qsubp: \n";
        //std::cout << Qsub << "\n";
        
        Q.set_sub_matrix(Qsub, 0, 0);
        
        //std::cout << "Q: \n";
        //std::cout << Q << "\n";
        
        //std::cout << "Q" << j << "\n";
        //std::cout << Q << "\n";
        
        nhrows--;
    }
    
    return Q;
}

result::QL<double> QL(matrix<double> const& A)
{
    result::QL<double> res;
    
    res.Y = matrix<double>(A);
    res.Y = QLfast(res.Y);
    
    res.Q = QLaccumulate(res.Y, 0);
    
    int64_t exrows = 2;
    for(int64_t c = res.Y.cols() - 1; c >= 0; c--)
    {
        for(int64_t r = res.Y.rows() - exrows; r >= 0; r--)
        {
            res.Y(r, c) = 0.0;
        }
        exrows++;
    }
    
    return res;
}

matrix<double>& QLHfast(matrix<double>& A)
{
    size_t M = A.rows();
    house h;
    matrix<double> Ablk, Apar;
    
    for(size_t j=0; j < M - 2; j++)
    {
        h = housevec(A.sub_col(0, M - j - 1, M - j - 1), M - j - 2);
        
        Ablk = A.sub_matrix(0, M - j - 1, 0, M - j);
        
        //std::cout << "Ablk:\n";
        //std::cout << Ablk << "\n";
        Ablk -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Ablk));
        
        A.set_sub_matrix(Ablk, 0, 0);
        
        Apar = A.sub_matrix(0, M, 0, M - j - 1);
        
        //std::cout << "Apar:\n";
        //std::cout << Apar << "\n";
        Apar -= h.beta * outer_prod_1D(inner_right_prod(Apar, h.vec), h.vec);
        
        A.set_sub_matrix(Apar, 0, 0);
        
        for(size_t k = 0; k < M - j - 2; k++)
        {
            A(k, M - j - 1) = h.vec(k, 0);
        }
    }
    
    return A;
}

result::QLH<double> QLH(matrix<double> const& A)
{
    result::QLH<double> res;
    res.H = matrix<double>(A);
    res.H = QLHfast(res.H);
    
    res.Q = QLaccumulate(res.H, 1);
    
    int64_t exrows = 3;
    for(int64_t c = res.H.cols() - 1; c >= 0; c--)
    {
        for(int64_t r = res.H.rows() - exrows; r >= 0; r--)
        {
            res.H(r, c) = 0.0;
        }
        exrows++;
    }
    
    return res;
}



/*
void bidiagonalize(matrix<double>& A)
{
    size_t M, N, n;
    house h = houseinit(A, M, N, n);
    
    matrix<double> Asub;
    
    for(size_t j=0; j < n; j++)
    {
        A = housestep(A, h, j);
        
        size_t i=1;
        for(size_t k = j + 1; j < M; j++, i++)
        {
            A(j, r) = h.vec(i, 0);
        }
        
        if(j < n - 1)
        {
            h = house(A.sub_row(j, j + 1, N - j - 1));
            
            Asub = A.sub_matrix(j, M - j, j + 1, N - j - 1);
            
            Asub -= h.beta * outer_prod_1D(inner_right_prod(Asub, h.vec), h.vec);
            
            Asub.set_sub_matrix(Asub, j, j + 1);
        }
    }
}*/



}

}
