//
//  jacobi.h
//  Created by Ben Westcott on 9/12/24.
//

#pragma once

#include "matrix.h"
#include "linalg_exceptions.h"
#include <cmath>
#include <tuple>

// refs:
// [1] https://en.wikipedia.org/wiki/Jacobi_rotation
// [2] https://web.stanford.edu/class/cme335/lecture7.pdf
// [3] https://vtechworks.lib.vt.edu/server/api/core/bitstreams/74c5529d-200e-4951-bbb3-22630e79cb15/content

// TODO: implement tridiagonal and bidiagonal reduction algorithms
// TODO: generalize to complex hermitian matrices

namespace transformation
{

// finds the abs max value for row k in upper triangle of input matrix mat
size_t off_diag_row_abs_max(size_t k, size_t N, size_t bias, const matrix<double>& mat)
{
    size_t m = k + 1 + bias;
    for(size_t i = k + 2; i < N; i++)
    {
        if(std::abs(mat(k, i)) > std::abs(mat(k, m)))
        {
            m = i;
        }
    }
    return m;
}

struct eigen_result
{
    matrix<double> values, vectors;
    
    eigen_result(const matrix<double>& evals, const matrix<double>& evecs)
    : values(evals), vectors(evecs) {}
};

namespace jacobi
{

/*
 * 1) theta = atan(2*A(p, q)/(A(q, q) - A(p, p)) if A(p, p) != A(q, q) else pi/2
 * 2) form rotation matrix with cos(theta)'s and sin(theta)'s
 *
 * which is equivalent to doing:
 *
 * 1) beta = (A(q, q) - A(p, p))/(2*A(p, q))
 * 2) t = sgn(beta)/(abs(beta) + sqrt(beta^2 + 1))
 * 3) cos(theta) = 1/sqrt(t^2 + 1)
 * 4) sin(theta) = t/sqrt(t^2 + 1)
 *
 */
void factors(double App, double Aqq, double Apq, double& c, double& s, double& t)
{
    // TODO: Apq could be zero or close to zero
    if(App != Aqq)
    {
        double beta = (Aqq - App)/(2*Apq);
        t = msgn(beta)/(std::abs(beta) + std::sqrt(beta * beta + 1));
        
        c = 1/std::sqrt(t * t + 1);
        s = t*c;
    }
    else
    {
        // e^(j*pi/2) = 0 + j1
        c = 0;
        s = 1;
        
    }
}

matrix<double> rotation_matrix(const matrix<double>& A, size_t p, size_t q)
{
    /*
     if(p == q)
     {
     throw non_distinct_indicies_error();
     }
     
     if(std::max(p, q) >= mat.rows())
     {
     throw std::range_error("Index p, q is out of range.");
     }*/
    
    double t, s, c;
    matrix<double> Q = matrix<double>::identity(A.rows());
    
    factors(A(p, p), A(q, q), A(p, q), c, s, t);
    
    Q(p, p) = Q(q, q) = c;
    Q(p, q) = s;
    Q(q, p) = -s;
    
    return Q;
}

void k_update(size_t k, double& ek, double t, size_t& state, bool& ck)
{
    double y = ek;
    ek = y + t;
    
    // relies on double precision float rounding,
    // i.e. t is so small that y + t makes no change to y
    // TODO: Add tolerance
    if(ck && y == ek)
    {
        ck = false;
        state--;
    }
    else if(!ck && y != ek)
    {
        ck = true;
        state++;
    }
}

/*
 * preforms:
 *
 * Akl    c  -s     Akl
 * Aij =  s   c  *  Aij
 *
 */
void rotate(matrix<double>& A, size_t k, size_t l, size_t i, size_t j, double s, double c)
{
    double akl = A(k, l);
    double aij = A(i, j);
    
    A(k, l) = c*akl - s*aij;
    A(i, j) = s*akl + c*aij;
}

eigen_result eigen(matrix<double>& A)
{
    size_t N = A.rows();
    size_t state = N;
    
    matrix<double> eigvalues(1, A.cols());
    matrix<double> eigvectors(matrix<double>::identity(A.rows()));
    // keeps track of whether Akl = Apq has changed in the current iteration
    matrix<bool> changed(1, N);
    matrix<size_t> max_iv(1, N);
        
    for(size_t k=0; k < N; k++)
    {
        max_iv(0, k) = off_diag_row_abs_max(k, N, 0, A);
        eigvalues(0, k) = A(k, k);
        changed(0, k) = true;
    }
    
    while(state != 0)
    {
        size_t m = 0;
        
        // pick out maximum in the upper triangular of A
        for(size_t j = 1; j < N-1; j++)
        {
            if(std::abs(A(j, max_iv(0, j))) > std::abs(A(m, max_iv(0, m))))
            {
                m = j;
            }
        }
        
        size_t k = m;
        size_t l = max_iv(0, m);
        double p = A(k, l);
        double c, s, t;
        
        factors(eigvalues(0, l), eigvalues(0, k), p, c, s, t);
        
        t *= -p;
        s = -s;
        
        std::cout << t << "\n";
        
        A(k, l) = 0.0;
        
        // update eigenvalues, state and changed arrays corresponding to entry A(k, l)
        k_update(k, eigvalues(0, k), -t, state, changed(0, k));
        k_update(l, eigvalues(0, l), t, state, changed(0, l));
        
        // here, I am only rotating the indicies in the upper triangle
        // since rotating the lower triangle is the transpose of rotating
        // the upper triangle
        
        for(size_t i=0; i < k; i++)
        {
            rotate(A, i, k, i, l, s, c);
        }
        
        for(size_t i=k+1; i < l; i++)
        {
            rotate(A, k, i, i, l, s, c);
        }
        
        for(size_t i=l+1; i < N; i++)
        {
            rotate(A, k, i, l, i, s, c);
        }
        for(size_t i=0; i < N; i++)
        {
            rotate(eigvectors, i, k, i, l, s, c);
            
            // update abs max locations for each row in upper triangle of A
            max_iv(0, i) = off_diag_row_abs_max(i, N, 0, A);
        }
    }
    
    return eigen_result(eigvalues, eigvectors);
}

matrix<double>& diagonalize(matrix<double>& A)
{
    eigen_result res = eigen(A);
    A = res.values.diag();
    
    return A;
}

}

}
