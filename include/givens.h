//
//  givens.h
//  Created by Ben Westcott on 10/9/24.
//

#pragma once

#include "matrix.h"
#include "result.h"
#include <cmath>

// refs:
// [1] Matrix Computations 4th ed. Golub, Van Loan
// [2]

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
    givens(double a, double b, double& r);
    givens(double rho);
    
    double flat(void);
};

inline void two_mult_fma(double x, double y, double& p, double& psq)
{
    p = x * y;
    psq = std::fma(x, y, -p);
}

givens comp_givens(double a, double b, double& r)
{
    r = std::hypot(a, b);
    double c = a/r;
    double s = b/r;
    
    double c1, c2, s1, s2;
    double p, psq;
    double ep_norm, ep_orth;
    double del_c, del_s;
    
    two_mult_fma(c, c, c1, c2);
    two_mult_fma(s, s, s1, s2);
    
    if(std::abs(c) >= std::abs(s))
    {
        ep_norm = (1 - c1 - s1 - c2 - s2)/2;
    }
    else
    {
        ep_norm = (1 - s1 - c1 - s2 - c2)/2;
    }
    
    two_mult_fma(c, b, p, psq);
    
    ep_orth = (std::fma(-s, a, p) + psq)/r;
    
    del_c = c * ep_norm - s * ep_orth;
    del_s = s * ep_norm + c * ep_orth;
    
    c += del_c;
    s += del_s;
    
    givens res;
    res.c = c;
    res.s = s;
    
    return res;
}


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

givens::givens(double a, double b, double& r)
{
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
        // c = 0 -> s = 1
        rho = 1;
    }
    // if was c != 0 
    else if(std::abs(s) < std::abs(c))
    {
        //std::cout << "less" << "\n";
        rho = ((c > 0) ? 1 : -1) * s/2;
        //std::cout << "copysign = " << std::copysign(s, c)/2 << "\n";
        //rho = std::copysign(s/2, c);
    }
    else
    {
        rho = ((s > 0) ? 1 : -1) * 2/c;
        //std::cout << "copysign = " << 2/std::copysign(c, s) << "\n";
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

matrix<double>& row_step(matrix<double>& A, givens g, size_t i, size_t k, size_t scol)
{
    for(size_t j=scol; j < A.cols(); j++)
    {
        A = rotate(A, g, i, j, k, j);
    }
    return A;
}

matrix<double>& col_step(matrix<double>& A, givens g, size_t i, size_t k, size_t srow)
{
    double tau1, tau2;
    for(size_t j=srow; j < A.rows(); j++)
    {
        A = rotate(A, g, j, i, j, k);
    }
    return A;
}

matrix<double>& nudge_first_rotation(matrix<double>& A, size_t M)
{
    double push = std::nextafter(A(M-2, 0), A(M-2, 0) + 1.0);
    A(M-2, 0) += std::abs(A(M-2, 0)) - std::abs(push);

    return A;
}

matrix<double>& QRfast(matrix<double>& A)
{
    size_t N = A.cols();
    size_t M = A.rows();
    
    givens g;
    
    if(A(M-2, 0) == A(M-1, 0))
    {
        A = nudge_first_rotation(A, M);
    }
    
    for(size_t j = 0; j < N; j++)
    {
        for(size_t i = M - 1; i >= j + 1; i--)
        {
            double r;
            g = givens(A(i-1, j), A(i, j));
            
            //std::cout << "s = " << g.s << "\tc = " << g.c << "\n";
            //std::cout << "i = " << i << "\tj = " << j << "\n";
            //std::cout << "A(i, j) = " << A(i, j) << "\tA(i-1, j) = " << A(i-1, j) << "\n";
            
            //double r;
            //givens ga = comp_givens(A(i - 1, j), A(i, j), r);
            //std::cout << "sa = " << ga.s << "\tca = " << ga.c << "\tra = " << r << "\tflat = " << ga.flat() << "\n";

            
            A = row_step(A, g, i - 1, i, j);
            //A = row_update(A, ga, i -1, i, j);
            
            A(i, j) = g.flat();
            
            //std::cout << "rho = " << A(i, j) << "\n";
            
            //std::cout << A << "\n\n";
        }
    }

    return A;
}

matrix<double>& QRHfast(matrix<double>& A /* must be square! */)
{
    size_t M, N;
    givens g;

    M = A.rows();
    N = A.cols();

    for(size_t j = 0; j < N - 1; j++)
    {
        g = givens(A(j, j), A(j + 1, j));

        A = rotate(A, g, j, j + 1, j, N - 1);
    }

    std::cout << A << "\n";

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
            
            //std::cout << "A(i, j) = " << A(i, j) << "\n";
            //std::cout << "s = " << g.s << "\tc = " << g.c << "\n";
            //std::cout << "i = " << i << "\tj = " << j << "\n";
            //std::cout << "rho= " << A(i, j) << "\n";

            Q = col_step(Q, g, i - 1, i, 0);
            
            //std::cout << "Q = \n";
            //std::cout << Q << "\n";
        }
    }
    
    return Q;
}

result::QR<double> QR(matrix<double> const& A)
{
    result::QR<double> res;
    
    res.Y = matrix<double>(A);
    res.Y = QRfast(res.Y);
    
    res.Q = QRaccumulate(res.Y);
    
    res.Y.fill_lower_triangle(0.0);
    return res;
}



}
}
