//
//  products.h
//  Created by Ben Westcott on 9/25/24.
//

#pragma once

#include "matrix.h"
#include "tdpool.hpp"
#include <vector>
#include <iostream>

template<typename T>
matrix<T> outer_prod_1D(matrix<T> const& cvec, matrix<T> const& rvec)
{
    if(!cvec.is_vector() || !rvec.is_vector())
    {
        throw std::out_of_range("incorrect dimensions for outer product.");
    }

    size_t M = cvec.size();
    size_t N = rvec.size();
    matrix<T> oprod(M, N);

    for(size_t i=0; i < M; i++)
    {
        for(size_t j=0; j < N; j++)
        {
            oprod(i, j) = cvec[i] * rvec[j];
        }
    }
    return oprod;
}

// NOTE: DOES NOT VERIFY SIZE, OR IF THEY ARE VECTORS
template<typename T>
inline T inner_prod_1D(matrix<T> const& rvec, matrix<T> const& cvec, size_t offs)
{
    T* rv_ptr = rvec.data();
    T* cv_ptr = cvec.data();
   /* T res = static_cast<T>(0.0);
    for(size_t r=offs; r < rvec.size(); r++)
    {
        for(size_t c=0; c < cvec.size(); c++)
        {
            res += rvec[r] * cvec[c];
        }
    }*/
    return std::inner_product(rv_ptr + offs, rv_ptr + rvec.size(), cv_ptr + offs, static_cast<T>(0.0));
}

template<typename T>
T inner_prod_1D(matrix<T> const& rvec, matrix<T> const& cvec)
{
    if(rvec.size() != cvec.size())
    {
        throw std::range_error("incorrect dimensions for inner product.");
    }

    return inner_prod_1D(rvec, cvec, 0);
}

template<typename T>
matrix<T> inner_left_prod(const matrix<T>& rvec, const matrix<T>& cvecs)
{
    //std::cout << "rvec dims = " << rvec.rows() << "\t" << rvec.cols() << "\n";
    //std::cout << "cvec dims = " << cvecs.rows() << "\t" << cvecs.cols() << "\n\n";
    if(rvec.size() != cvecs.rows())
    {
        throw std::range_error("incorrect dimensions for inner product.");
    }

    matrix<T> iprod(1, cvecs.cols());
    for(size_t c=0; c < cvecs.cols(); c++)
    {
        iprod.get_value(0, c) = inner_prod_1D(rvec, cvecs.col(c), 0);
    }

    return iprod;
}

template<typename T>
matrix<T> inner_right_prod(const matrix<T>& rvecs, const matrix<T>& cvec)
{
    if(rvecs.cols() != cvec.size())
    {
        throw std::range_error("incorrect dimensions for inner product.");
    }

    matrix<T> iprod(rvecs.rows(), 1);
    for(size_t r=0; r < rvecs.rows(); r++)
    {
        iprod.get_value(r, 0) = inner_prod_1D(rvecs.row(r), cvec, 0);
    }

    return iprod;
}

template<typename T>
matrix<T> projection(matrix<T> const& u, matrix<T> const& v)
{
    return (inner_prod_1D(v, u)/inner_prod_1D(u, u)) * u;
}

template<typename T>
double col_norm2sq_from(matrix<T> const& rhs, size_t c, size_t from_row)
{
    return inner_prod_1D(rhs.col(c), rhs.col(c), from_row);
}

template<typename T>
double col_norm2sq(const matrix<T>& rhs, size_t c)
{
    if(c >= rhs.cols())
    {
        throw std::range_error("column index is out of range.");
    }
    
    return col_norm2sq_from(rhs, c, 0);
} 

template<typename T>
double vec_norm2sq_from(matrix<T> const& rhs, size_t offs)
{
    return inner_prod_1D(rhs, rhs, offs);
}

template<typename T>
matrix<double> cols_norm2sq(matrix<T> const& rhs)
{
    matrix<double> norms(1, rhs.cols());
    
    for(size_t i=0; i < norms.cols(); i++)
    {
        norms.get_value(i) = col_norm2sq(rhs, i);
    }
    
    return norms;
}

template<class T>
matrix<T>** mat_mul_create_pairs(const matrix<T>* lhs, const matrix<T>* rhs)
{
   /* if(lhs->cols() != rhs->rows())
    {
        throw std::domain_error("outer product is undefined for given dimensions!");
    }*/
    
    size_t N = lhs->cols();
    matrix<T>** tmpl = new matrix<T>*[lhs->cols()];

    for(size_t r=0; r < N; r++)
    {
        tmpl[r] = new matrix<T>[2]
        {
            matrix<T>(lhs->col(r)), matrix<T>(rhs->row(r))
        };
    }
    
    return tmpl;
}


template<class T>
matrix<T>* mat_mul_create_pair(const matrix<T>* lhs, const matrix<T>* rhs, size_t n)
{
    matrix<T>* ret = new matrix<T>[2]
    {
        matrix<T>(lhs->col(n)), matrix<T>(rhs->row(n))
    };
    
    return ret;
}

template<class T>
void mat_mul_free_pairs(matrix<T>** tmpl, size_t N)
{
    for(size_t r=0; r < N; r++)
    {
        delete[] tmpl[r];
    }
    
    delete[] tmpl;
}

template<class T>
matrix<T> mat_mul_alg1(const matrix<T>* lhs, const matrix<T>* rhs)
{
    size_t M = lhs->rows();
    size_t N = lhs->cols();
    size_t K = rhs->cols();
    
    tdpool pool(2);
    std::vector<std::future<matrix<T>>> oprod_results;
    
    size_t curr_adds = 0;
    
    matrix<T>** curr_oprod_pairs = mat_mul_create_pairs(lhs, rhs);
    
    matrix<T> mresult(M, K);
    
    for(size_t i = 0; i < N; i++)
    {
        oprod_results.emplace_back
        (
            pool.enqueue(outer_prod_1D<T>, curr_oprod_pairs[i][0], curr_oprod_pairs[i][1])
        );
    }
    
    for(; curr_adds < N;)
    {
        for(auto&& result : oprod_results)
        {
            if(result.valid())
            {
                mresult += result.get();
                curr_adds++;
            }
        }
    }
    
    //mat_mul_free_pairs(curr_oprod_pairs, N);
    return mresult;
}
