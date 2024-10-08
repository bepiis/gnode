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
// R is upper triangular
template<typename T>
struct QR
{
    matrix<T> Q;
    matrix<T> R;
    
    QR(matrix<T> const& q, matrix<T> const& r)
    : Q(q), R(r) {}
};

// used to return a factorization where:
// Q is orthogonal
// H is (upper or lower) hessenberg
template<typename T>
struct QH
{
    matrix<T> Q;
    matrix<T> H;
    
    QH(matrix<T> const& q, matrix<T> const& h)
    : Q(q), H(h) {}
};


// used to return a factorization where:
// F is arbitrary
// P is a column permutation
template<typename T>
struct FP
{
    matrix<T>& F;
    matrix<size_t> P;
    
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
    
    FPr(matrix<T> & f, matrix<size_t> const& p, size_t rnk)
    : F(f), P(p), rank(rnk) {}
};

// used to return a factorization where:
// P is a row permutation
// F is arbitrary
template<typename T>
using PFr = FPr<T>;


}
