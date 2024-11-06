//
//  lu_decomp.h
//  Created by Ben Westcott on 9/8/24.
//

#pragma once

#include "matrix.hpp"
#include "pivot.h"
#include <cmath>

namespace transformation
{

struct LU_work
{
    LU_work(matrix<double> _U, matrix<double> _L, matrix<size_t> _P, bool deg, size_t np, size_t _N)
    : U(_U), L(_L), P(_P), nperms(np), degenerate(deg), N(_N){}
    
    matrix<double> U, L;
    matrix<size_t> P;
    size_t N;
    size_t nperms;
    bool degenerate;
};

LU_work LU(const matrix<double>& A, const double tol)
{
    
    double max_val, curr_val, max_i;
    size_t nswaps = 0;
    size_t N = A.rows();
    
    matrix<double> U(A);
    matrix<double> L = matrix<double>::identity(N);
    
    bool degenerate = false;
    
    std::vector<matrix<double>> lops;
    
    // 1xN matrix whose entires are their corresponding indicies
    // i.e. P_4 = [0, 1, 2, 3]^T
    // keeps track of row swaps during partial pivoting
    matrix<size_t> P = matrix<size_t>::unit_permutation_matrix(N);
    
    
    for(size_t i=0; i < N; i++)
    {
        max_val = 0.0;
        max_i = i;
        
        
        // find row below i which has the maximum value
        // in the pivot column
        for(size_t k=i; k < N; k++)
        {
            curr_val = std::abs(U(k, i));
            if(curr_val > max_val)
            {
                max_val = curr_val;
                max_i = k;
            }
        }
        
        /*
         * Partial row pivoting:
         *  ensures that values on main diagonal are
         *  sufficiently greater than zero to avoid
         *  division by zero, and/or division by a small number
         *  resulting in large values of k during the
         *  elimination step.
         */
        
        if(max_i != i)
        {
            nswaps++;
            
            U.swap_rows(max_i, i);
            P.swap_rows(max_i, i);
            
            for(size_t j = 0; j < i; j++)
            {
                double t = L(max_i, j);
                L(max_i, j) = L(i, j);
                L(i, j) = t;
            }
        }
        
        if(max_val < tol)
        {
            /*
             * If pivot value search failed to find a
             * value below current row i that is greater than
             * the supplied tolerance, then its considered to be zero
             * and the matrix does not have a single solution.
             * Thus, L can be arbitrarily chosen.
             */
            degenerate = true;
            
            for(size_t j = i+1; j < N; j++)
            {
                L(j, i) = 0;
            }
        }
        else
        {
            /*
             * Preform gauss elimination on the column
             * under the current diagonal element U(i, i)
             */
            for(size_t j=i+1; j < N; j++)
            {
                double k = U(j, i)/U(i, i);
                L(j, i) = k;
                
                U.sub_rows(j, i, k);
            }
        }
    }
    
    return LU_work(U, L, P, degenerate, nswaps, N);
}
}

/*
 * Preforms solution to a lower triangular system
 * b = Lx, where L is lower triangular.
 *
 * Note that:
 *      b_1 = l_11 * x_1
 *      b_2 = l_21 * x_1 + l_22 * x_2
 *      ...
 *      b_n = l_n1 * x_1 + l_n2 * x_2 + ... + l_nn * x_n
 
 * Thus in general:
 *       x_j = (b_j - (l_j0 * x_0 + l_j1 * x1 + ... + l_j(n-1) * x_(n-1))/l_jj
 */
void forward_substitution(const matrix<double>& L, const matrix<double>& b, matrix<double>& soln, size_t N)
{
    soln(0, 0) = b(0, 0)/L(0, 0);
    
    for(size_t i=1; i < N; i++)
    {
        double sum = 0.0;
        
        for(size_t j=0; j < i; j++)
        {
            sum += L(i, j) * soln(j, 0);
        }
        
        soln(i, 0) = (b(i, 0) - sum)/L(i, i);
    }
}

/*
 * Same as forward sub, except b = Uy
 * where U is upper triangular. Thus,
 * we basically preform forward substitution
 * in reverse.
 */
void backward_substitution(const matrix<double>& U, const matrix<double> &b, matrix<double>& soln, size_t N)
{
    int last_i = (int)N - 1;
    soln(last_i, 0) = b(last_i, 0)/U(last_i, last_i);
    
    for(int i = last_i - 1; i >= 0; i--)
    {
        double sum = 0.0;
        
        for(int j=i+1; j < N; j++)
        {
            sum += U(i, j) * soln(j, 0);
        }
        soln(i, 0) = (b(i, 0) - sum)/U(i, i);
    }
}


/*
 * If we want to solve Ax = b, then
 *     - decompose A = LU so that (LU)x = b
 *     - solve for y = Ux via forward sub: L(Ux) = Ly = b
 *     - solve Ux = y for x via back sub
 */
matrix<double> LU_solve(const matrix<double>& A, const matrix<double>& b)
{
    using namespace transformation;
    
    LU_work result = LU(A, std::numeric_limits<double>::min());
    size_t N = result.N;
    
    matrix<double> input(N, 1);
    for(size_t r = 0; r < N; r++)
    {
        input(r, 0) = b(result.P(r, 0), 0);
    }
    
    matrix<double> y(N, 1), x(N, 1);
    
    forward_substitution(result.L, input, y, N);
    backward_substitution(result.U, y, x, N);
    
    return x;
}

}
