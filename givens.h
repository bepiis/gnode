//
//  givens.h
//  Created by Ben Westcott on 10/9/24.
//

#pragma once

#include "matrix.h"
#include "result.h"


inline double signum(double a)
{
    return (a == 0) ? 0.0 : (a > 0) ? 1.0 : -1.0;
}



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

givens alt(double a, double b, double& r)
{
    double s, c;
    if(b == 0)
    {
        c = signum(a);
        if(c == 0)
        {
            c = 1.0;
        }
        s = 0.0;
        r = std::abs(a);
    }
    else if(a == 0)
    {
        c = 0;
        s = -signum(b);
        r = std::abs(b);
    }
    else
    {
        double t, u;
        if(std::abs(a) > std::abs(b))
        {
            t = b/a;
            u = signum(a) * std::sqrt(1 + t*t);
            c = 1/u;
            s = -c * t;
            r = a * u;
        }
        else
        {
            t = a/b;
            u = signum(b) * std::sqrt(1 + t*t);
            s = -1/u;
            c = t/u;
            r = b * u;
        }
    }
    
    givens ret;
    ret.s = s;
    ret.c = c;
    return ret;
}

matrix<double>& rotate(matrix<double>& A, givens g, size_t r1, size_t c1, size_t r2, size_t c2)
{
    double tau1 = A(r1, c1);
    double tau2 = A(r2, c2);
        
    A(r1, c1) = g.c * tau1 - g.s * tau2;
    A(r2, c2) = g.s * tau1 + g.c * tau2;
    
    return A;
}

matrix<double>& row_update(matrix<double>& A, givens g, size_t i, size_t k, size_t scol)
{
    for(size_t j=scol; j < A.cols(); j++)
    {
        A = rotate(A, g, i, j, k, j);
    }
    return A;
}

matrix<double>& col_update(matrix<double>& A, givens g, size_t i, size_t k, size_t srow)
{
    double tau1, tau2;
    for(size_t j=srow; j < A.rows(); j++)
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
            
            double r;
            givens ga = alt(A(i - 1, j), A(i, j), r);
            std::cout << "sa = " << ga.s << "\tca = " << ga.c << "\tra = " << r << "\tflat = " << ga.flat() << "\n";

            
            A = row_update(A, g, i - 1, i, j);
            //A = row_update(A, ga, i -1, i, j);
            
            A(i, j) = ga.flat();
            
            std::cout << "rho = " << A(i, j) << "\n";
            
            std::cout << A << "\n\n";
        }
    }
    
    return A;
}

matrix<double> QRaccumulate(matrix<double> const& A)
{
    size_t M = A.rows();
    size_t N = A.cols();
    
    matrix<double> Q = matrix<double>::eye(M);
    
    givens g;
    
    for(size_t j = 0; j < N; j++)
    {
        for(size_t i = M - 1; i >= j + 1; i--)
        {
            g = givens(A(i, j));
            
            std::cout << "A(i, j) = " << A(i, j) << "\n";
            std::cout << "s = " << g.s << "\tc = " << g.c << "\n";
            std::cout << "i = " << i << "\tj = " << j << "\n";

            
            Q = col_update(Q, g, i-1, i, 0);
            
            std::cout << "Q = \n";
            std::cout << Q << "\n";
        }
    }
    
    return Q;
}


}

}
