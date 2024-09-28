//
//  matrix.h
//  Created by Ben Westcott on 9/25/24.
//

#pragma once

#include <iostream>
#include <cstring>
#include <format>
#include <random>
#include <cmath>
#include <utility>
#include <format>
#include <algorithm>

using std::size_t;

template<typename T>
class matrix
{
public:
    matrix(size_t size = 0);
    matrix(size_t size, const T* dat);
        
    matrix<T>& reshape(size_t new_r, size_t new_c);
    matrix(size_t r, size_t c);
    matrix(size_t r, size_t c, const T* dat);
    
    matrix(const matrix<T>& rhs);
    
    matrix(std::initializer_list<T> dat);
    matrix(size_t size, std::initializer_list<T> dat);
    matrix(size_t r, size_t c, std::initializer_list<T> dat);

    ~matrix(void);
    
    friend void swap(matrix<T>& lhs, matrix<T>& rhs) noexcept
    {
        using std::swap;
        
        //std::cout << "\tcalled swap\n";
        
        swap(lhs.m_rows, rhs.m_rows);
        swap(lhs.m_cols, rhs.m_cols);
        swap(lhs.m_size, rhs.m_size);
        swap(lhs.m_data, rhs.m_data);
    }
    
    // TODO: not sure if i want to continue using copy-swap for copy/move assignment...
    // considering matrices can be very large, then passing by value here becomes extremely inefficient...
    matrix<T>& operator=(matrix<T> rhs);
    matrix(matrix<T>&& rhs) noexcept;
    //matrix<T>& operator=(matrix<T>&& rhs);
    
    size_t size(void) const { return m_size; }
    size_t rows(void) const { return m_rows; }
    size_t cols(void) const { return m_cols; }
    
    bool is_square(void) const { return (m_rows == m_cols); }
    bool is_row_vector(void) const;
    bool is_col_vector(void) const;
    bool is_vector(void) const;
    
    T* data(void) const { return m_data.get(); }
    
    T& get_value(size_t offs) const;
    void set_value(size_t offs, T value);
    T& operator[](size_t offs);
	const T operator[](size_t offs) const;
	
    size_t row_offset(size_t m) const;
    size_t offset(size_t m, size_t n) const;
    
    T& get_value(size_t m, size_t n) const;
    void set_value(size_t m, size_t n, T value);
    T& operator()(size_t m, size_t n);
    const T operator()(size_t m, size_t n) const;
    
    void fill(T value);
    void zero(void);
    void ones(void);
    
    void rfill(T value, size_t col_bias);
    void fill_upper_triangle(T value);
    void fill_upper_hessenberg(T value);
    
    void cfill(T value, size_t row_bias);
    void fill_lower_triangle(T value);
    void fill_lower_hessenberg(T value);
    
    void set_identity(void);
    
    bool content_equals(const matrix<T>& rhs) const;
    bool equals(const matrix<T>& rhs) const;
    bool is_symmetric(void) const;

    matrix<T> row(size_t r) const;
    matrix<T>& set_row(const matrix<T>& rvec, size_t r);

    matrix<T> col(size_t c) const;
    matrix<T>& set_col(const matrix<T>& cvec, size_t c);

    matrix<T>& swap_rows(size_t r1, size_t r2);
    matrix<T>& swap_cols(size_t c1, size_t c2);
    matrix<T>& sub_rows(size_t r1, size_t r2, T factor);
    matrix<T>& permute_rows(const matrix<size_t>& rpermute);

    matrix<T> transpose(void) const;
    matrix<T> diag(void) const;

    std::pair<matrix<T>, matrix<T>> split_rows(size_t at_row) const;
    std::pair<matrix<T>, matrix<T>> split_cols(size_t at_col) const;

    matrix<T> sub_matrix(size_t start_row, size_t nrows, size_t start_col, size_t ncols) const;
    matrix<T> sub_matrix(size_t start_row, size_t start_col) const;
    matrix<T>& set_sub_matrix(const matrix<T>& sub, size_t start_row, size_t start_col);

    matrix<T> sub_col(size_t start_row, size_t nrows, size_t c) const;
    matrix<T>& set_sub_col(const matrix<T>& sub, size_t start_row, size_t c);

    template<typename R> 
    matrix<T>& operator+=(const matrix<R>& rhs);

    template<typename R> 
    matrix<T>& operator-=(const matrix<R>& rhs);
    
    template<typename R>
    matrix<T>& operator*=(R scalar);

    static matrix<T> eye(size_t rank);
	static matrix<T> ones(size_t nrows, size_t ncols);
    static matrix<size_t> unit_permutation_matrix(size_t rank);
    
    // TODO: soon we will need to generate random matrix types i.e. symmetric, orthogonal, skew, pos. definite, etc. to test various algos
    static matrix<T> random_dense_matrix(size_t nrows, size_t ncols, float lowerbound, float upperbound);
    
    static matrix<T> abs(const matrix<T>& rhs);
    static matrix<T> absdiff(const matrix<T>& rhs, const matrix<T>& lhs);
    static T abs_max_err(const matrix<T>& result, const matrix<T>& expected);
    static size_t abs_max_excess_err(const matrix<T>& result, const matrix<T>& expected, T tolerance);
    
    static T abs_max_element(const matrix<T>& rhs, size_t from_row);

private:
    
    size_t m_rows;
    size_t m_cols;
    size_t m_size;
    std::shared_ptr<T> m_data;
    
};

template<typename T>
matrix<T>::matrix(size_t size)
: m_size(size), m_rows(size ? size : 1), m_cols(1),
  m_data(size ? new T[size]() : nullptr)
{
      //std::cout << "\tcalled default constructor\n";
}

template<typename T>
matrix<T>::matrix(size_t size, const T* dat)
: matrix<T>(size)
{
    //std::cout << "\tcalled size, data constructor\n";
    std::copy(dat, dat + size, data());
}

template<typename T>
matrix<T>& matrix<T>::reshape(size_t new_r, size_t new_c)
{
    if((new_r * new_c) != m_size)
    {
        throw std::range_error("reshape: requires that new dimensions are compatible with the existing size. Use realloc to resize and/or reshape.");
    }
    
    //std::cout << "\tcalled reshape\n";
    
    m_rows = new_r;
    m_cols = new_c;
    
    return *this;
}

template<typename T>
matrix<T>::matrix(size_t r, size_t c)
: matrix<T>(r * c)
{
    //std::cout << "\tcalled row, col constructor\n";
    reshape(r, c);
}


template<typename T>
matrix<T>::matrix(size_t r, size_t c, const T* dat)
: matrix<T>((r * c), dat)
{
    //std::cout << "\tcalled row, col, data constructor\n";
    reshape(r, c);
}

template<typename T>
matrix<T>::matrix(const matrix<T>& rhs)
: matrix<T>(rhs.rows(), rhs.cols())
{
    //std::cout << "\tcalled copy constructor\n\n";
    std::copy(rhs.data(), rhs.data() + m_size, data());
}

template<typename T>
matrix<T>::matrix(std::initializer_list<T> dat)
: matrix<T>(dat.size())
{
    //std::cout << "\tcalled initializer list constructor\n";
    std::move(dat.begin(), dat.end(), data());
}


template<typename T>
matrix<T>::matrix(size_t size, std::initializer_list<T> dat)
: matrix<T>(size)
{
    //std::cout << "\tcalled size, initializer list constructor\n";
    std::move(dat.begin(), dat.begin() + std::min(m_size, dat.size()), data());
}

template<typename T>
matrix<T>::matrix(size_t r, size_t c, std::initializer_list<T> dat)
: matrix<T>((r * c), dat)
{
    //std::cout << "\tcalled row, col, initializer list constructor\n";
    reshape(r, c);
}

template<typename T>
matrix<T>::~matrix(void)
{
    //std::cout << "\tcalled destroy\n";
    //delete [] m_data;
}

/*
 template<typename T>
 matrix<T>& matrix<T>::operator=(matrix<T>&& rhs)
 {
    std::cout << "\tcalled move assignment\n";
    swap(*this, rhs);
    return *this;
 }*/

template<typename T>
matrix<T>& matrix<T>::operator=(matrix<T> rhs)
{
    //std::cout << "\tcalled copy assignment\n";
    swap(*this, rhs);
    return *this;
}

template<typename T>
matrix<T>::matrix(matrix<T>&& rhs) noexcept
: matrix<T>()
{
    //std::cout << "\tcalled move constructor\n";
    swap(*this, rhs);
}

template<typename T>
inline bool matrix<T>::is_row_vector(void) const
{
    return (m_rows == 1);
}

template<typename T>
inline bool matrix<T>::is_col_vector(void) const
{
    return (m_cols == 1);
}

template<typename T>
inline bool matrix<T>::is_vector(void) const
{
    return is_row_vector() || is_col_vector();
}

template<typename T>
inline T& matrix<T>::get_value(size_t offs) const
{
    return m_data.get()[offs];
}

template<typename T>
inline void matrix<T>::set_value(size_t offs, T value)
{
    m_data.get()[offs] = value;
}

template<typename T>
inline T& matrix<T>::operator[](size_t offs)
{
    return m_data.get()[offs];
}

template<typename T>
const T matrix<T>::operator[](size_t offs) const
{
	return m_data.get()[offs];
}

template<typename T>
inline size_t matrix<T>::row_offset(size_t m) const
{
    return (m * cols());
}

template<typename T>
inline size_t matrix<T>::offset(size_t m, size_t n) const
{
    return (m * cols() + n);
}

template<typename T>
inline T& matrix<T>::get_value(size_t m, size_t n) const
{
    return get_value(offset(m, n));
}

template<typename T>
inline void matrix<T>::set_value(size_t m, size_t n, T value)
{
    set_value(offset(m, n), value);
}

template<typename T>
inline T& matrix<T>::operator()(size_t m, size_t n)
{
    return (*this)[offset(m, n)];
}

template<typename T>
const T matrix<T>::operator()(size_t m, size_t n) const
{
    return (*this)[offset(m, n)];
}


template<typename T>
void matrix<T>::fill(T value)
{
    for(size_t k=0; k < m_size; k++)
    {
        get_value(k) = value;
    }
}

template<typename T>
void matrix<T>::zero(void)
{
    fill(static_cast<T>(0.0));
}

template<typename T>
void matrix<T>::ones(void)
{
    fill(static_cast<T>(1.0));
}

template<typename T>
void matrix<T>::rfill(T value, size_t col_bias)
{
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c = r + col_bias; c < m_cols; c++)
        {
            get_value(r, c) = value;
        }
    }
}

template<typename T>
void matrix<T>::fill_upper_triangle(T value)
{
    rfill(static_cast<T>(value), 1);
}

template<typename T>
void matrix<T>::fill_upper_hessenberg(T value)
{
    rfill(static_cast<T>(value), 2);
}

template<typename T>
void matrix<T>::cfill(T value, size_t row_bias)
{
    for(size_t c = 0; c < m_cols; c++)
    {
        for(size_t r = c + row_bias; r < m_rows; r++)
        {
            get_value(r, c) = value;
        }
    }
}

template<typename T>
void matrix<T>::fill_lower_triangle(T value)
{
    cfill(static_cast<T>(value), 1);
}

template<typename T>
void matrix<T>::fill_lower_hessenberg(T value)
{
    cfill(static_cast<T>(value), 2);
}

template<typename T>
void matrix<T>::set_identity(void)
{
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c=0; c < m_cols; c++)
        {
            // TODO: optimizable since we should only have to check once per row for this
            get_value(r, c) = static_cast<T>((r == c) ? 1.0 : 0.0);
        }
    }
}

// TODO: conditional exit function, maybe when iterators are worked out.
template<typename T>
bool matrix<T>::content_equals(const matrix<T>& rhs) const
{
    if(m_size != rhs.size())
    {
        return false;
    }
    
    for(size_t i=0; i < m_size; i++)
    {
        if(get_value(i) != rhs.get_value(i))
        {
            return false;
        }
    }
    return true;
}

template<typename T>
bool matrix<T>::equals(const matrix<T>& rhs) const
{
    return (m_rows != rhs.rows() || m_cols != rhs.cols()) ? false : content_equals(rhs);
}

template<typename T>
bool matrix<T>::is_symmetric(void) const
{
    if(!is_square())
    {
        return false;
    }
    
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c = r + 1; c < m_cols; c++)
        {
            if(get_value(r, c) != get_value(c, r))
            {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
matrix<T> matrix<T>::row(size_t r) const 
{
    if(r >= m_rows)
    {
        throw std::range_error("row: row index is out of range.");
    }

    return matrix<T>(1, m_cols, data() + row_offset(r));
}

template<typename T>
matrix<T>& matrix<T>::set_row(const matrix<T>& rvec, size_t r)
{
    if(r >= m_rows)
    {
        throw std::range_error("set_row: row index is out of range.");
    }

    std::copy(rvec.data(), rvec.data() + rvec.cols(), data());
    return *this;
}

template<typename T>
matrix<T> matrix<T>::col(size_t c) const
{
    if(c >= m_cols)
    {
        throw std::range_error("col: column index is out of range.");
    }

    matrix<T> cvec(m_rows, 1);
    for(size_t r=0; r < m_rows; r++)
    {
        cvec.set_value(r, 0, get_value(r, c));
    }

    return cvec;
}

template<typename T>
matrix<T>& matrix<T>::set_col(const matrix<T>& cvec, size_t c)
{
    if(c >= m_cols)
    {
        throw std::range_error("set_col: column index is out of range.");
    }

    for(size_t r=0; r < m_rows; r++)
    {
        set_value(r, c, cvec.get_value(r, 0));
    }

    return *this;
}

template<typename T>
matrix<T>& matrix<T>::swap_rows(size_t r1, size_t r2)
{
    if(r1 >= m_rows || r2 >= m_rows)
    {
        throw std::range_error("swap_rows: row index out of range.");
    }

    size_t i1 = row_offset(r1);
    size_t i2 = row_offset(r2);
    size_t i=0;

    for(; i < m_cols; i1++, i2++, i++)
    {
        T tmp = get_value(i1);
        set_value(i1, get_value(i2));
        set_value(i2, tmp);
    }
    
    return *this;
}

template<typename T>
matrix<T>& matrix<T>::swap_cols(size_t c1, size_t c2)
{
    if(c1 >= m_cols || c2 >= m_cols)
    {
        throw std::range_error("swap_cols: column index is out of range.");
    }

    size_t i1, i2, r;
    T t1, t2;

    for(r=0; r < m_rows; r++)
    {
        i1 = offset(r, c1);
        i2 = offset(r, c2);

        t1 = get_value(i1);
        t2 = get_value(i2);

        set_value(i1, t2);
        set_value(i2, t1);
    }

    return *this;
}

template<typename T>
matrix<T>& matrix<T>::sub_rows(size_t r1, size_t r2, T factor)
{
    if(r1 >= m_rows || r2 >= m_rows)
    {
        throw std::range_error("sub_rows: row index is out of range.");
    }

    size_t i1 = row_offset(r1);
    size_t i2 = row_offset(r2);
    size_t i=0;

    for(; i < m_cols; i1++, i2++, i++)
    {
        set_value(i1, get_value(i1) - factor * get_value(i2));
    }

    return *this;
}

template<typename T>
matrix<T>& matrix<T>::permute_rows(const matrix<size_t>& rpermute)
{
    if(rpermute.rows() != m_rows)
    {
        throw std::range_error("permute_rows: incorrect row dimensions.");
    }

    matrix<T> to(*this);
    for(size_t r=0; r < rpermute.rows(); r++)
    {
        to.set_row(row(rpermute.get_value(r, 0)), r);
    }

    // TODO: hmmm
    *this = std::move(to);
    return *this;
}

template<typename T>
matrix<T> matrix<T>::transpose(void) const
{
    matrix<T> tm(m_cols, m_rows);
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c=0; c < m_cols; c++)
        {
            tm.get_value(c, r) = get_value(r, c);
        }
    }

    return tm;
}

template<typename T>
matrix<T> matrix<T>::diag(void) const
{
    bool sel = false;
    size_t i, end_cond;

    if(is_row_vector())
    {
        end_cond = m_cols;
        sel = true;
    }
    else if(is_col_vector())
    {
        end_cond = m_rows;
    }
    else
    {
        throw std::range_error("matrix is not a row or column vector.");
    }

    matrix<T> dm(end_cond, end_cond);
    for(i = 0; i < end_cond; i++)
    {
        dm.get_value(i, i) = get_value(!sel * i, sel * i);
    }
    
    return dm;
}

template<typename T>
std::pair<matrix<T>, matrix<T>> matrix<T>::split_rows(size_t at_row) const
{
    if(at_row >= m_rows)
    {
        throw std::range_error("split_rows: row index is out of range.");
    }

    return std::make_pair
    (
        matrix<T>(at_row + 1, m_cols, data() + row_offset(0)),
        matrix<T>(m_rows - at_row - 1, m_cols, data() + offset(at_row, m_cols))
    );
}

template<typename T>
std::pair<matrix<T>, matrix<T>> matrix<T>::split_cols(size_t at_col) const
{
    if(at_col >= m_cols)
    {
        throw std::range_error("split_cols: column index is out of range.");
    }

    matrix<T> left(m_rows, at_col + 1);
    matrix<T> right(m_rows, m_cols - at_col - 1);

    size_t il, ir, r, c;
    il = ir = r = c = 0;

    for(; r < m_rows; r++)
    {
        c = 0;
        for(; c < m_cols; c++)
        {
            T value = get_value(r, c);
            if(c <= at_col)
            {
                left[il++] = value;
            }
            else right[ir++] = value;
        }
    }

    return std::make_pair(left, right);
}

template<typename T>
matrix<T> matrix<T>::sub_matrix(size_t start_row, size_t nrows, size_t start_col, size_t ncols) const
{
    if(start_row + nrows > m_rows || start_col + ncols > m_cols)
    {
        throw std::range_error("sub_matrix: incompatible dimensions for submatrix.");
    }

    matrix<T> sub(nrows, ncols);
    
    T* dst_ptr = sub.data();
    T* src_ptr = data() + offset(start_row, start_col);

    for(size_t r=0; r < nrows; r++, src_ptr += m_cols, dst_ptr += ncols)
    {
        std::copy(src_ptr, src_ptr + ncols, dst_ptr);
    }

    return sub;
}

 template<typename T>
 matrix<T> matrix<T>::sub_matrix(size_t start_row, size_t start_col) const
 {
    return sub_matrix(start_row, m_rows - start_row, start_col, m_cols - start_col);
 }

// TODO: subview object
template<typename T>
matrix<T>& matrix<T>::set_sub_matrix(const matrix<T>& sub, size_t start_row, size_t start_col) 
{
    if(start_row + sub.rows() > m_rows || start_col + sub.cols() > m_cols)
    {
        throw std::range_error("set_sub_matrix: incompatible dimensions for submatrix.");
    }

    for(size_t r=0; r < sub.rows(); r++)
    {
        for(size_t c=0; c < sub.cols(); c++)
        {
            get_value(r + start_row, c + start_col) = sub.get_value(r, c);
        }
    }

    return *this;
}

template<typename T>
matrix<T> matrix<T>::sub_col(size_t start_row, size_t nrows, size_t c) const 
{
    return sub_matrix(start_row, nrows, c, 1);
}

template<typename T>
matrix<T>& matrix<T>::set_sub_col(const matrix<T>& sub, size_t start_row, size_t c)
{
    return set_sub_matrix(sub, start_row, c);
}

template<typename T>
template<typename R>
matrix<T>& matrix<T>::operator+=(const matrix<R>& rhs)
{
    if(m_size != rhs.size())
    {
        throw std::range_error("operator+=: sizes must be equal.");
    }

    for(size_t i=0; i < m_size; i++)
    {
        get_value(i) += static_cast<T>(rhs.get_value(i));
    }

    return *this;
}

template<typename T>
template<typename R>
matrix<T>& matrix<T>::operator-=(const matrix<R>& rhs)
{
    if(m_size != rhs.size())
    {
        throw std::range_error("operator-=: sizes must be equal.");
    }

    for(size_t i=0; i < m_size; i++)
    {
        get_value(i) -= static_cast<T>(rhs.get_value(i));
    }

    return *this;
}


template<typename T>
template<typename R>
matrix<T>& matrix<T>::operator*=(R scalar)
{
    for(size_t i=0; i < m_size; i++)
    {
        get_value(i) *= static_cast<T>(scalar);
    }
    
    return *this;
}

template<typename T>
matrix<T> matrix<T>::eye(size_t rank)
{
    matrix<T> I(rank, rank);
    I.set_identity();
    return I;
}

template<typename T>
matrix<T> matrix<T>::ones(size_t nrows, size_t ncols)
{
	matrix<T> I(nrows, ncols);
	I.ones();
	return I;
}

template<>
matrix<size_t> matrix<size_t>::unit_permutation_matrix(size_t rank)
{
    matrix<size_t> perm(rank, 1);
    for(size_t i=0; i < rank; i++)
    {
        perm.get_value(i, 0) = i;
    }

    return perm;
}

template<typename T>
matrix<T> matrix<T>::random_dense_matrix(size_t nrows, size_t ncols, float lowerbound, float upperbound)
{
    matrix<T> rmat(nrows, ncols);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lowerbound, upperbound);

    for(size_t i=0; i < rmat.size(); i++)
    {
        rmat[i] = static_cast<T>(dis(gen));
    }

    return rmat;
}

template<typename T>
matrix<T> matrix<T>::abs(const matrix<T>& rhs)
{
    matrix<T> abs_result(rhs.rows(), rhs.cols());
    
    std::transform
    (
        rhs.data(),
        rhs.data() + rhs.size(),
        abs_result.data(),
        [](T val) { return std::abs(val); }
    );
    
    return abs_result;
}

template<typename T>
inline matrix<T> matrix<T>::absdiff(const matrix<T>& rhs, const matrix<T>& lhs)
{
    return matrix<T>::abs(rhs - lhs);
}

template<typename T>
inline T matrix<T>::abs_max_err(const matrix<T>& result, const matrix<T>& expected)
{
    matrix<T> ad = absdiff(result, expected);
    
    return *std::max_element(ad.data(), ad.data() + ad.size());
}

template<typename T>
inline size_t matrix<T>::abs_max_excess_err(const matrix<T>& result, const matrix<T>& expected, T tolerance)
{
    matrix<T> ad = absdiff(result, expected);
    return std::count_if
    (
        ad.data(),
        ad.data() + ad.size(),
        [tolerance](T val) { return val > tolerance; }
    );
    
}

template<typename T>
T matrix<T>::abs_max_element(const matrix<T>& rhs, size_t from_row)
{
    return *std::max_element
    (
        rhs.data() + rhs.row_offset(from_row),
        rhs.data() + rhs.size(),
        [](T v1, T v2) { return std::abs(v1) < std::abs(v2); }
     );
}

template<typename T>
bool operator==(const matrix<T>& lhs, const matrix<T>& rhs)
{
    return lhs.equals(rhs);
}

template<typename T>
bool operator!=(const matrix<T>& lhs, const matrix<T>& rhs)
{
    return !lhs.equals(rhs);
}

template<typename T, typename R>
matrix<T> operator*(R scalar, const matrix<T>& rhs)
{
    matrix<T> res(rhs);
    res *= scalar;
    return res;
}

template<typename T, typename R>
matrix<T> operator+(const matrix<T>& lhs, const matrix<R>& rhs)
{
    matrix<T> sum(lhs);
    sum += rhs;
    return sum;
}

template<typename T, typename R>
matrix<T> operator-(const matrix<T>& lhs, const matrix<R>& rhs)
{
    matrix<T> sum(lhs);
    sum -= rhs;
    return sum;
}



