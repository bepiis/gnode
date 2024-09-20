//
//  matrix_iterators.h
//  mmnn
//
//  Created by Ben Westcott on 9/16/24.
//

#ifndef MATRIX_ITERATORS_H
#define MATRIX_ITERATORS_H

#include "matrix.hpp"

template<class T>
struct RM_iterator
{
    
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    RM_iterator() = default;
    
    explicit RM_iterator(matrix<T>&ref, T* ptr)
    : m_ref(ref), m_ptr(ptr) {}
    
    explicit RM_iterator(matrix<T>& ref)
    : RM_iterator<T>(ref, ref.base_ptr()) {}
    
    reference operator*() const;
    
    RM_iterator& operator++();
    RM_iterator operator++(int);
    
    RM_iterator& operator--();
    RM_iterator operator--(int);
    
    bool operator==(const RM_iterator& rhs);
    bool operator!=(const RM_iterator& rhs);
    
    static RM_iterator begin(matrix<T>& ref);
    static RM_iterator end(matrix<T>& ref);

private:
    
    matrix<T>& m_ref;
    T* m_ptr;
    
};

template<class T>
RM_iterator<T>::reference RM_iterator<T>::operator*() const
{
    return *m_ptr;
}

template<class T>
RM_iterator<T>& RM_iterator<T>::operator++()
{
    ++m_ptr;
    return *this;
}

template<class T>
RM_iterator<T> RM_iterator<T>::operator++(int)
{
    RM_iterator<T> tmp = *this;
    ++m_ptr;
    return tmp;
}

template<class T>
RM_iterator<T>& RM_iterator<T>::operator--()
{
    m_ptr--;
    return *this;
}

template<class T>
RM_iterator<T> RM_iterator<T>::operator--(int)
{
    T* tmp = m_ptr;
    --m_ptr;
    return *this;
}

template<class T>
bool RM_iterator<T>::operator==(const RM_iterator& rhs)
{
    return m_ptr == rhs.m_ptr;
}

template<class T>
bool RM_iterator<T>::operator!=(const RM_iterator& rhs)
{
    return m_ptr != rhs.m_ptr;
}

template<class T>
RM_iterator<T> RM_iterator<T>::begin(matrix<T>& ref)
{
    return RM_iterator(ref);
}

template<class T>
RM_iterator<T> RM_iterator<T>::end(matrix<T>& ref)
{
    return RM_iterator(ref, ref.top_ptr());
}

template<class T>
struct CM_iterator
{
    
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    CM_iterator() = default;
    
    explicit CM_iterator(matrix<T>&ref, T* ptr)
    : m_ref(ref), m_ptr(ptr)
    {
        
        size_t offs = (size_t)(ptr - ref.base_ptr()) - 1;
        m_col_idx = offs % ref.cols();
        m_row_idx = (size_t)(offs - m_col_idx)/ref.cols();
 
    }
    
    explicit CM_iterator(matrix<T>& ref)
    : CM_iterator<T>(ref, ref.base_ptr()) {}
    
    reference operator*() const;
    
    CM_iterator& operator++();
    CM_iterator operator++(int);
    
    CM_iterator& operator--();
    CM_iterator operator--(int);
    
    bool operator==(const CM_iterator& rhs);
    bool operator!=(const CM_iterator& rhs);
    
    static CM_iterator begin(matrix<T>& ref);
    static CM_iterator end(matrix<T>& ref);
    
private:
    
    matrix<T>& m_ref;
    size_t m_row_idx;
    size_t m_col_idx;
    T* m_ptr;
    
};

template<class T>
CM_iterator<T>::reference CM_iterator<T>::operator*() const
{
    return *m_ptr;
}

template<class T>
CM_iterator<T>& CM_iterator<T>::operator++()
{
    if((m_row_idx + 1) == m_ref.rows())
    {
        m_row_idx = 0;
        m_col_idx++;
    }
    else m_row_idx++;
    
    m_ptr = &m_ref(m_row_idx, m_col_idx);
    
    return *this;
}

template<class T>
CM_iterator<T> CM_iterator<T>::operator++(int)
{
    CM_iterator<T> tmp = *this;
    ++(*this);
    return tmp;
}

template<class T>
CM_iterator<T>& CM_iterator<T>::operator--()
{
    if(m_col_idx != 0 && m_row_idx - 1 == 0)
    {
        m_row_idx = m_ref.rows() - 1;
        m_col_idx--;
    } 
    else m_row_idx--;
    
    m_ptr = &m_ref(m_row_idx, m_col_idx);
    
    return *this;
}

template<class T>
CM_iterator<T> CM_iterator<T>::operator--(int)
{
    T* tmp = m_ptr;
    (*this)--;
    return *this;
}

template<class T>
bool CM_iterator<T>::operator==(const CM_iterator& rhs)
{
    return m_ptr == rhs.m_ptr;
}

template<class T>
bool CM_iterator<T>::operator!=(const CM_iterator& rhs)
{
    return m_ptr != rhs.m_ptr;
}

template<class T>
CM_iterator<T> CM_iterator<T>::begin(matrix<T>& ref)
{
    return CM_iterator(ref);
}

template<class T>
CM_iterator<T> CM_iterator<T>::end(matrix<T>& ref)
{
    return CM_iterator(ref, ref.top_ptr());
}

#endif /* MATRIX_ITERATORS_H */
