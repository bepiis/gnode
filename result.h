//
//  results.h
//  Created by Ben Westcott on 10/1/24.
//

#pragma once

#include "matrix.h"

namespace result
{

// used to return a factorization where:
// Q is orthogonal
// Y is triangular
template<typename T>
struct QY
{
    matrix<T> Q;
    matrix<T> Y;
    
    QY(void) = default;
    
    QY(matrix<T> const& q, matrix<T> const& y)
    : Q(q), Y(y) {}
    
};


template<typename T>
using QR = QY<T>;

template<typename T>
using QL = QY<T>;

// used to return a factorization where:
// Q is orthogonal
// H is hessenberg
template<typename T>
struct QH
{
    matrix<T> Q;
    matrix<T> H;
    
    QH(void) = default;
    
    QH(matrix<T> const& q, matrix<T> const& h)
    : Q(q), H(h) {}
};

// For upper hessenberg factorization
template<typename T>
using QRH = QH<T>;

// For lower hessenberg factorization
template<typename T>
using QLH = QH<T>;


// used to return a factorization where:
// F is arbitrary
// P is a column permutation
template<typename T>
struct FP
{
    matrix<T>& F;
    matrix<size_t> P;
    
    FP(void) = default;
    
    FP(matrix<T> & f, matrix<size_t> const& p)
    : F(f), P(p) {}
};

// used to return a rank-revealing factorization where:
// F is arbitrary
// P is a column permutation
// r is the rank
template<typename T>
struct FPr
{
    matrix<T>& F;
    matrix<size_t> P;
    size_t rank;
    
    FPr(void) = default;
    
    FPr(matrix<T> & f, matrix<size_t> const& p, size_t rnk)
    : F(f), P(p), rank(rnk) {}
};

// used to return a factorization where:
// P is a row permutation
// F is arbitrary
template<typename T>
using PFr = FPr<T>;


}
