//
//  matrix.h
//  Created by Ben Westcott on 11/7/24.
//

#pragma once

#include "matrix.h"
#include <cstdint>

using std::size_t;

struct permutation
{
    static matrix<size_t> unit(size_t rank);
    static matrix<size_t> flip(size_t rank);     // also called exchange permutation

    template<typename T>
    static matrix<T>& onrows(matrix<T> & A, matrix<size_t> const& perm);

    template<typename T>
    static matrix<T>& swap_rows(matrix<T> & A, size_t r1, size_t r2);

    template<typename T>
    static matrix<T>& oncols(matrix<T> & A, matrix<size_t> const& perm);

    template<typename T>
    static matrix<T>& swap_cols(matrix<T> & A, size_t c1, size_t c2);

};

matrix<size_t> permutation::unit(size_t rank)
{
    matrix<size_t> perm(rank, 1);
    for(size_t i=0; i < perm.size(); i++)
    {
        perm[i] = i;
    }
    return perm;
}

matrix<size_t> permutation::flip(size_t rank)
{
    matrix<size_t> perm(rank, 1);
    for(int64_t i=perm.size() - 1; i >= 0; i--)
    {
        perm[rank - i - 1] = (size_t)i;
    }
    return perm;
}

// TODO: once realloc is implemented, we can be more space efficient by rellocing an extra row/column for permute rows/cols instead of creating an entirely new matrix
template<typename T>
matrix<T>& permutation::onrows(matrix<T> & A, matrix<size_t> const& perm)
{
    if(perm.size() != A.rows())
    {
        throw std::range_error("permutation::onrows: incorrect dimensions.");
    }

    matrix<T> to(A);
    for(size_t r=0; r < perm.size(); r++)
    {
        to.set_sub_row(A.row(r), perm[r], 0);
    }

    A = std::move(to);
    return A;
}

template<typename T>
matrix<T>& permutation::swap_rows(matrix<T> & A, size_t r1, size_t r2)
{
    if(r1 >= A.rows() || r2 >= A.rows())
    {
        throw std::range_error("swap_rows: row index out of range.");
    }

    size_t i1 = A.offset(r1, 0);
    size_t i2 = A.offset(r2, 0);
    size_t i = 0;

    for(; i < A.cols(); i++, i1++, i2++)
    {
        T tmp = A[i1];
        A[i1] = A[i2];
        A[i2] = tmp;
    }
    
    return A;
}

template<typename T>
matrix<T>& permutation::oncols(matrix<T> & A, matrix<size_t> const& perm)
{
    if(perm.size() != A.cols())
    {
        throw std::range_error("permutation::oncols: incorrect dimensions.");
    }

    matrix<T> to(A);
    for(size_t c = 0; c < perm.size(); c++)
    {
        to.set_sub_col(A.col(c), 0, perm[c]);
    }

    A = std::move(to);
    return A;
}

template<typename T>
matrix<T>& permutation::swap_cols(matrix<T> & A, size_t c1, size_t c2)
{
    if(c1 >= A.cols() || c2 >= A.cols())
    {
        throw std::range_error("swap_cols: column index is out of range.");
    }

    size_t i1, i2, r;
    T t1, t2;

    for(r=0; r < A.rows(); r++)
    {
        i1 = A.offset(r, c1);
        i2 = A.offset(r, c2);

        t1 = A[i1];
        t2 = A[i2];

        A[i1] = t2;
        A[i2] = t1;
    }
    
    return A;
}

