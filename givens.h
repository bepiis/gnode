//
//  givens.h
//  Created by Ben Westcott on 10/9/24.
//

#pragma once

#include "matrix.h"
#include "result.h"

namespace transformation
{

namespace givens
{

struct givens
{
    double s, c;
    
    givens(void) = default;
    givens(double a, double b);
    givens(double rho);
    
    double flat(void);
};

givens::givens(double a, double b)
{
    if(b == 0)
    {
        c = 1;
        s = 0;
    }
    else
    {
        double tau;
        if(std::abs(b) > std::abs(a))
        {
            tau = -a/b;
            s = 1/std::sqrt(1 + tau*tau);
            c = s * tau;
        }
        else
        {
            tau = -b/a;
            c = 1/std::sqrt(1 + tau*tau);
            s = c * tau;
        }
    }
}

givens::givens(double rho)
{
    if(rho == 1)
    {
        c = 0;
        s = 1;
    }
    else if(std::abs(rho) < 1)
    {
        s = 2 * rho;
        c = std::sqrt(1 - s*s);
    }
    else
    {
        c = 2/rho;
        s = std::sqrt(1 - c*c);
    }
}

double givens::flat(void)
{
    double rho;
    if(c == 0)
    {
        rho = 1;
    }
    else if(std::abs(s) < std::abs(c))
    {
        rho = ((c > 0) ? 1 : -1) * s/2;
    }
    else
    {
        rho = ((s > 0) ? 1 : -1) * 2/c;
    }
    
    return rho;
}

matrix<double>& rotate(matrix<double>& A, givens g, size_t r1, size_t c1, size_t r2, size_t c2)
{
    double tau1 = A(r1, c1);
    double tau2 = A(r2, c2);
    
    A(r1, c1) = g.c * tau1 - g.s * tau2;
    A(r2, c2) = g.s * tau1 + g.c * tau2;
    
    return A;
}

matrix<double>& row_update(matrix<double>& A, givens g, size_t i, size_t k)
{
    for(size_t j=0; j < A.cols(); j++)
    {
        A = rotate(A, g, i, j, k, j);
    }
    return A;
}

matrix<double>& col_update(matrix<double>& A, givens g, size_t i, size_t k)
{
    double tau1, tau2;
    for(size_t j=0; j < A.rows(); j++)
    {
        A = rotate(A, g, j, i, j, k);
    }
    return A;
}

matrix<double>& QRfast(matrix<double>& A)
{
    size_t N = A.cols();
    size_t M = A.rows();
    
    givens g;
    
    for(size_t j = 0; j < N; j++)
    {
        for(size_t i = M - 1; i >= j + 1; i--)
        {
            g = givens(A(i-1, j), A(i, j));
            
            std::cout << "s = " << g.s << "\tc = " << g.c << "\n";
            std::cout << "i = " << i << "\tj = " << j << "\n";
            
            A = row_update(A, g, i - 1, i);
            
            A(i, j) = g.flat();
            
            std::cout << "rho = " << A(i, j) << "\n";
            
            std::cout << A << "\n\n";
        }
    }
    
    return A;
}


}

}
