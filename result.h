//
//  result.h
//  Created by Ben Westcott on 10/1/24.
//

#pragma once

#include "matrix.h"

namespace result
{

// used to return a solution where:
// Q is orthogonal
// Y is arbitrary
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

template<typename T>
using LQ = QY<T>;

// used to return a solution where:
// Q is orthogonal
// H is hessenberg
template<typename T>
using QH = QY<T>;

// For upper hessenberg solution
template<typename T>
using QRH = QY<T>;

// For lower hessenberg solution
template<typename T>
using QLH = QY<T>;

// used to return a solution where:
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

// used to return a rank-revealing solution where:
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

// used to return a solution where:
// P is a row permutation
// F is arbitrary
template<typename T>
using PFr = FPr<T>;


}
