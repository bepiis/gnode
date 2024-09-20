#ifndef MAT_MUL_H
#define MAT_MUL_H

#include "matrix.hpp"
#include "tdpool.hpp"
#include <vector>
#include <iostream>

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
    
    mat_mul_free_pairs(curr_oprod_pairs, N);
    return mresult;
}


#endif
