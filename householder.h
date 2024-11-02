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

/*
 * TODO:
 * I shouldnt actually need to explicitly write algorithms for
 * XQ algs since they are just transposed problems of QX problems.
 * 
 * This would require some "TransposeView" object which will effectively
 * wrap a matrix and reverse rows/cols in all its methods such that 
 * its treated as a transposed matrix. Then we can just run the XQ algs
 * on the TransposeView. 
 * 
 * This is kind of encroaching on the whole idea of making the matrix
 * class much less monolithic. I.e. if we have a TransposeView, then
 * this begs the question whether data access should even be defined in the 
 * matrix class, and the answer is no it shouldnt be. 
 * 
 * Instead, we should have a matrix engine which handles the data
 * and matrix contains an reference to an owning engine. Then when we want
 * to have alternative views of the data, we create a non-owning engine which
 * takes an owning reference, and views the data in the respective way. 
 * 
 * This extends even farther into the ability to view submatrices without
 * having to explicitly make a copy of the relevant data. It should be possible
 * to have a SubMatrixView which can read/write sections of the underlying data 
 * without having to copy it into a smaller matrix. 
 */

namespace transformation
{

namespace house
{

struct house
{
    matrix<double> vec;
    double beta;
    
    house() = default;
    house(matrix<double> const& v, double b);
    house(matrix<double> const& vessential, size_t normi);

    //house(matrix<double> const& x, size_t norm_indx);
};

house::house(matrix<double> const& v, double b)
: vec(v), beta(b)
{ 
}

/*
 * Constructs a house vector from its essential form, 
 * i.e. if          vhouse = [0 0 0 ... 0 1 v1 v2 ... vn]
 *      then        vessential = [v1 v2 ... vn]
 *      and         vec = [1 v1 v2 ... vn]
 */
house::house(matrix<double> const& vessential, size_t normi)
{
    vec = matrix<double>(vessential.size() + 1, 1);
    vec[normi] = 1.0;
    
    double norm2 = 0.0;
    size_t ve, a;

    ve = a = 0;
    for(; a < vec.size(); a++)
    {
        if(a == normi)
        {
            continue;
        }
        vec[a] = vessential[ve++];
        norm2 += vec[a] * vec[a];
    }

    beta = 2/(1 + norm2);
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

matrix<double>& housestep(matrix<double>& A, house& h, size_t j)
{
    h = housevec(A.sub_col(j, A.rows() - j, j), 0);
    
    matrix<double> Asub = A.sub_matrix(j, A.rows() - j, j, A.cols() - j);
    Asub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Asub));
    
    A.set_sub_matrix(Asub, j, j);
    
    return A;
}

/*
 * A : input matrix
 * i : nbr of rows/cols off from M x N to consider
 * k : corner value. This changes per iteration of QR and related factorizations.
 * hc: column to be considered in current iteration
 * s : index in house vector which is normalized. For QR, this is zero, for QL is the last elem
 */
matrix<double>& colstep(matrix<double>& A, house& h, size_t i, size_t k, size_t hc, size_t s)
{
    h = housevec(A.sub_col(k, A.rows() - i, hc), s);
    //std::cout << A.sub_col(k, A.rows() - i, hc) << "\n";
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
    house h;
    //house h = houseinit(A, M, N, n);

    M = A.rows();
    N = n = A.cols();

    if(M == N)
    {
        n--;
    }
    
    for(size_t j=0; j < n; j++)
    {
        A = QRstep(A, h, j);

        if(j < M)
        {
            size_t i=1;
            for(size_t k = j+1; k < M; k++, i++)
            {
                A(k, j) = h.vec[i];
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
matrix<double> QRaccumulate(matrix<double> const& F, size_t k, size_t cb)
{
    size_t M = F.rows();
    size_t N = F.cols();
    
    int64_t n = (int64_t)N;

    house h;
    size_t normi = 0;
    
    if(M == N)
    {
        n--;
    }
    //std::cout << "betalen = " << B.cols() << "\n";
    
    matrix<double> Im = matrix<double>::eye(M);
    matrix<double> Q = Im.sub_matrix(0, M, 0, M);
    matrix<double> vhouse;
    matrix<double> Qsub;
    
    for(int64_t j = n - 1 - (int64_t)cb; j >= 0; j--)
    {
        //vhouse = matrix<double>(M - j - col_bias, 1);
        //vhouse(0, 0) = 1.0;
        
        //matrix<double> Fjp1 = F.sub_col(j + 1 + col_bias, M - j - 1 - col_bias, j);
        
        //vhouse.set_sub_col(Fjp1, 1, 0);

        h = house(F.sub_col(j + cb + 1, M - j - cb - 1, j), normi);

        Qsub = Q.sub_matrix(j + cb, Q.rows()-j - cb, j + cb , Q.rows()-j - cb);
        
        //double beta = 2/(1+col_norm2sq_from(Fjp1, 0, 0));

        // Q <- (Im - beta*v*v^T)Q
        Qsub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Qsub));
        
        Q.set_sub_matrix(Qsub, j + cb, j + cb);

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
matrix<double>& QRHfast(matrix<double> &A /* must be square*/)
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
            A(j, k) = h.vec[i];
        }
    }
    return A;
}

result::QRH<double> QRH(matrix<double> const& A)
{
    // A must be square.
    result::QRH<double> res;
    
    res.Y = matrix<double>(A);
    res.Y = QRHfast(res.Y);
    
    res.Q = QRaccumulate(res.Y, res.Y.rows(), 1);
    
    res.Y.fill_lower_hessenberg(0.0);
    
    return res;
}

result::FPr<double> colpiv_QRfast(matrix<double>& A)
{
    size_t M, N, n;
    house h;
    //house h = houseinit(A, M, N, n);

    M = A.rows();
    N = n = A.cols();

    if(M == N)
    {
        n--;
    }
    
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
                A(j, r) = h.vec[i];
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
    /*
    matrix<double>& colstep(matrix<double>& A, house& h, size_t i, size_t k, size_t hc, size_t s)


    h = housevec(A.sub_col(k, A.rows() - i, hc), s);
    matrix<double> Asub = A.sub_matrix(k, A.rows() - i, k, A.cols() - i);
    
    Asub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Asub));
    A.set_sub_matrix(Asub, k, k);
    */
    return colstep(A, h, i, 0, A.cols() - i - 1, A.rows() - i - 1);
}

matrix<double>& QLfast(matrix<double>& A)
{
    size_t M, N, n;
    house h;
    //house h = houseinit(A, M, N, n);

    M = A.rows();
    N = n = A.cols();

    if(M == N)
    {
        n--;
    }
    
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
                A(k, N - j - 1) = h.vec[k];
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
    
    house h;
    size_t normi = nhrows - cb - 1;
    
    if(M == N)
    {
        end_cond++;
    }
    
    matrix<double> Q = matrix<double>::eye(M);
    matrix<double> vhouse, Qsub;
    
    //std::cout << "F = \n";
    //std::cout << F << "\n";
    
    for(int64_t j = n - 1 - cb; j >= end_cond; j--)
    {
        //vhouse = matrix<double>(nhrows - cb, 1);
        //vhouse(nhrows - 1 - cb, 0) = 1.0;
        
        //std::cout << "nhcols: " << nhcols << "\n";
        //matrix<double> hj = F.sub_col(0, nhrows - 1 - cb, j + cb);

        h = house(F.sub_col(0, nhrows - 1 - cb, j + cb), normi);

        //vhouse.set_sub_col(hj, 0, 0);
        //std::cout << "vhouse: \n";
        //std::cout << vhouse << "\n";
        
        // TODO: why is this M - cb and not M - j - cb????
        Qsub = Q.sub_matrix(0, M - cb, 0, nhrows - cb);
        
        //std::cout << "Qsub: \n";
        //std::cout << Qsub << "\n";
        //std::cout << vhouse << "\n";
        
        //double beta = 2/(1 + col_norm2sq_from(hj, 0, 0));
        
        //Qsub -= beta * outer_prod_1D(vhouse, inner_left_prod(vhouse, Qsub));
        Qsub -= h.beta * outer_prod_1D(inner_right_prod(Qsub, h.vec), h.vec);
        
        //std::cout << "Qsubp: \n";
        //std::cout << Qsub << "\n";
        
        Q.set_sub_matrix(Qsub, 0, 0);
        
        //std::cout << "Q: \n";
        //std::cout << Q << "\n";
        
        //std::cout << "Q" << j << "\n";
        //std::cout << Q << "\n";
        
        nhrows--;
        normi--;
    }
    
    return Q;
}

result::QL<double> QL(matrix<double> const& A)
{
    result::QL<double> res;
    
    res.Y = matrix<double>(A);
    res.Y = QLfast(res.Y);
    
    res.Q = QLaccumulate(res.Y, 0);

    matrix<double>::set_lower_tri(res.Y, 0.0, 2);
    
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
            A(k, M - j - 1) = h.vec[k];
        }
    }
    
    return A;
}

result::QLH<double> QLH(matrix<double> const& A)
{
    result::QLH<double> res;
    res.Y = matrix<double>(A);
    res.Y = QLHfast(res.Y);
    
    res.Q = QLaccumulate(res.Y, 1);

    matrix<double>::set_lower_tri(res.Y, 0.0, 3);
    
    return res;
}

matrix<double>& RQfast(matrix<double>& A)
{
    size_t M, N, m;
    house h;
    
    M = m = A.rows();
    N = A.cols();

    if(M == N)
    {
        m--;
    }
    
    matrix<double> Asub;

    for(size_t i=0; i < m; i++)
    {
        std::cout << "i= " << i << "\n";
        std::cout << A.sub_row(M - i - 1, 0, N - i) << "\n";

        h = housevec(A.sub_row(M - i - 1, 0, N - i), N - i - 1);
        std::cout << "vhouse = \n";
        std::cout << h.vec << "\n";

        Asub = A.sub_matrix(0, M - i, 0, N - i);

        std::cout << "Asub = \n";
        std::cout << Asub << "\n";

        Asub -= h.beta * outer_prod_1D(inner_right_prod(Asub, h.vec), h.vec);

        A.set_sub_matrix(Asub, 0, 0);

        if(i < N)
        {
            for(size_t k=0; k < N - i - 1; k++)
            {
                A(M - i - 1, k) = h.vec[k];
            }
        }

        std::cout << "A = \n";
        std::cout << A << "\n";
        std::cout << "\n-------------------------------------\n";

    }
    return A;
}

matrix<double> RQaccumulate(matrix<double> const& F/*, size_t col_bias*/)
{
    size_t M = F.rows();
    size_t N = F.cols();
    
    int64_t m = (int64_t)M;
    int64_t end_cond = 0;
    int64_t nhcols = N;
    //int64_t cb = (int64_t)col_bias;
    
    house h;
    size_t normi = nhcols /*- cb*/ - 1;

    if(M == N)
    {
        end_cond++;
    }

    matrix<double> Q = matrix<double>::eye(N);
    matrix<double> vhouse, Qsub;
    
    //std::cout << "F = \n";
    //std::cout << F << "\n";
    
    for(int64_t i = m - 1 /*-cb*/; i >= end_cond; i--)
    {
        std::cout << "nhcols = " << nhcols << "\tnormi = " << normi << "\n";
        std::cout << F.sub_row(i, 0, nhcols - 1);
        h = house(F.sub_row(i, 0, nhcols - 1), normi);
        std::cout << h.vec << "\n";

        Qsub = Q.sub_matrix(0, nhcols/*-cb*/, 0, N /*-cb*/);

        Qsub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Qsub));
        Q.set_sub_matrix(Qsub, 0, 0);

        std::cout << "Q = \n";
        std::cout << Q << "\n";

        nhcols--;
        normi--;
    }

    return Q;
}



matrix<double>& LQfast(matrix<double>& A)
{
    size_t M, m, N;
    house h;
    matrix<double> Asub;

    M = m = A.rows();
    N = A.cols();
    
    if(M == N)
    {
        m--;
    }
    
    std::cout << A << "\n";
    
    for(size_t i=0; i < m; i++)
    {
        std::cout << "i= " << i << "\n";
        std::cout << A.sub_row(i, i, N - i) << "\n\n";

        h = housevec(A.sub_row(i, i, N - i), 0);
        
        Asub = A.sub_matrix(i, M - i, i, N - i);
        
        Asub -= h.beta * outer_prod_1D(inner_right_prod(Asub, h.vec), h.vec);
        
        A.set_sub_matrix(Asub, i, i);
        
        std::cout << "hvec = \n";
        std::cout << h.vec << "\n";
        
        if(i < N)
        {
            size_t k=1;
            for(size_t j=i+1; j < N; j++, k++)
            {
                A(i, j) = h.vec(0, k);
            }
        }
    }
    
    std::cout << A << "\n";
    
    return A;
}

matrix<double> LQaccumulate(matrix<double> const& F)
{
    size_t M = F.rows();
    size_t N = F.cols();
    
    int64_t m = (int64_t)M;
    
    if(M == N)
    {
        m--;
    }
    
    matrix<double> Q, vhouse, Qsub;

    house h;
    size_t normi = 0;

    Q = matrix<double>::eye(N);
    
    for(int64_t i = m - 1; i >= 0; i--)
    {
        //vhouse = matrix<double>(1, N - i);
        //vhouse[0] = 1.0;
        
        //Fh = F.sub_row(i, i + 1, N - i - 1);
        
        //vhouse.set_sub_row(Fh, 0, 1);

        h = house(F.sub_row(i, i + 1, N - i - 1), normi);
        
        Qsub = Q.sub_matrix(i, N - i, i, N - i);
        
        //double beta = 2/(1 + vec_norm2sq_from(Fh, 0));
        Qsub -= h.beta * outer_prod_1D(inner_right_prod(Qsub, h.vec), h.vec);
        //Qsub -= h.beta * outer_prod_1D(h.vec, inner_left_prod(h.vec, Qsub));
        
        std::cout << "Qsub = \n";
        std::cout << Qsub << "\n";
        
        //std::cout << "vhouse = \n";
        //std::cout << vhouse << "\n";
        
        Q.set_sub_matrix(Qsub, i, i);
    }
    
    return Q;
}

result::LQ<double> LQ(matrix<double> const& A)
{
    result::LQ<double> res;

    res.Y = matrix<double>(A);
    LQfast(res.Y);
    res.Q = LQaccumulate(res.Y);
    res.Y.fill_upper_triangle(0.0);

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
