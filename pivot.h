//
//  pivot.h
//  mmnn
//
//  Created by Ben Westcott on 9/10/24.
//

#ifndef PIVOT_H
#define PIVOT_H

#include "matrix.hpp"

/*
 * TODO: Bad idea, completely rework
 *      i was trying to come up with a way to keep track of
 *      left multiply and right multiply operations for
 *      any similarity transformation: A <- Q.transpose() * A * Q
 *      but this is way too overstructured to work nicely with anything.
 */

template<class T>
class operation
{
public:
    operation(const matrix<T>& m)
    : ref(m)
    {}
        
    virtual matrix<T>& swap(size_t, size_t) = 0;
    virtual matrix<T>& multiple(size_t, T) = 0;
    virtual matrix<T>& add_multiple(size_t, size_t, T factor) = 0;
    
private:
    const matrix<T>& ref;
};

template<class T>
class Lop : public operation<T>
{
public:
    Lop(const matrix<T>& m)
    : operation<T>(m), mat(matrix<T>::identity(m.rows()))
    {}
    
    matrix<T>& swap(size_t r1, size_t r2) override
    {
        mat.swap_rows(r1, r2);
        return mat;
    }
    
    matrix<T>& multiple(size_t r, T factor) override
    {
        mat(r, r) = factor;
        return mat;
    }
    
    matrix<T>& add_multiple(size_t r1, size_t r2, T factor) override
    {
        mat(r1, r2) = factor;
        return mat;
    }
    
private:
    matrix<T> mat;
};

template<class T>
class Rop : public operation<T>
{
public:
    Rop(const matrix<T> &m)
    : operation<T>(m), mat(matrix<T>::identity(m.cols()))
    {}
    
    matrix<T>& swap(size_t c1, size_t c2) override
    {
        mat.swap_cols(c1, c2);
        return mat;
    }
    
    matrix<T>& multiple(size_t c, T factor) override
    {
        mat(c, c) = factor;
        return mat;
    }
    matrix<T>& add_multiple(size_t c1, size_t c2, T factor) override
    {
        mat(c1, c2) = factor;
        
        matrix<T> tmp = mat.transpose();
        mat = std::move(tmp);
        
        return mat;
    }
    
private:
    matrix<T> mat;
};


#endif /* PIVOT_H */
