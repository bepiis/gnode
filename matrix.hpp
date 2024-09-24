#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstring>
#include <format>
#include <random>
#include <cmath>
#include <utility>
#include <format>
//#include <Accelerate/Accelerate.h>

#define MEMCPY(ptr, data, size) (std::memcpy((void*)bptr, (const void*)data, size*sizeof(T)))
#define MEMMOV(ptr, data, size) (std::memmove((void*)bptr, (const void*)data, size*sizeof(T)))

#define ROW_OFFS(m) (m * cols())
#define OFFS(m, n) (ROW_OFFS(m) + n)


inline int msgn(double x)
{
    return (x >= 0) ? 1 : -1;
}

template <class T>
class matrix
{
public:
    
    // default constructor
    matrix();
    
    //constructs matrix of size m * n = r * c
    // TODO: read https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    matrix(size_t r, size_t c);
    
    matrix(size_t r, size_t c, const T* data);
    matrix(size_t r, size_t c, std::initializer_list<T> data);
    
    matrix(std::initializer_list<T> data);
    matrix<T>& reshape(size_t new_r, size_t new_c);

    matrix(const matrix<T>& from);
    matrix(matrix<T>&& from) noexcept;
    
    ~matrix();
    
    T* base_ptr(void) const;
    T* top_ptr(void) const;
    
    T get_value(size_t m, size_t n) const;
    T get_value(size_t offs) const;
    T& operator[](size_t offs);
    
    const T operator()(size_t m, size_t n) const;
    T& operator()(size_t m, size_t n);
    
    const T operator()(size_t m) const;
    T& operator()(size_t m);
    
    void set_value(size_t m, size_t n, T value);
    void set_value(size_t offs, T value);
    void fill(T value);
    void zero(void);
    void ones(void);
    void set_identity(void);
    void zero_lower_triangle(void);
    void zero_upper_triangle(void);
    void print(void) const;
    
    static matrix<T> identity(size_t m);
    static matrix<size_t> unit_permutation_matrix(size_t order);
    static matrix<T> random_dense_matrix(size_t m, size_t n, float lowerbound, float upperbound);
    
    bool content_equals(const matrix<T>& other) const;
    bool equals(const matrix<T>& other) const;
    
    matrix<T>& operator=(const matrix<T>& from);
    matrix<T>& operator=(matrix<T>&& from);
    
    matrix<T>& operator+=(const matrix<T>& rhs);
    matrix<T>& operator-=(const matrix<T>& rhs);
    
    matrix<T> row(size_t r) const;
    
    matrix<T>& set_row(const matrix<T>& rvec, size_t r);
    
    //matrix<T> row(size_t rfrom, size_t rto);
    
    matrix<T> col(size_t c) const;
    
    matrix<T>& set_col(const matrix<T>& cvec, size_t c);

    //matrix<T> col(size_t cfrom, cto);
    
    matrix<T> transpose(void);
    matrix<T> diag(void);
    
    matrix<T>& swap_rows(size_t r1, size_t r2);
    matrix<T>& swap_cols(size_t c1, size_t c2);
    
    matrix<T>& sub_rows(size_t r1, size_t r2, T factor);
    
    // A <- PA
    matrix<T>& permute_rows(const matrix<size_t>& permute_mat);
    
    // B <- [A1 | A2]^T
    matrix<T>* split_rows(size_t at_row);
    // B <- [A1 | A2]
    matrix<T>* split_cols(size_t at_col);
    
    matrix<T> sub_matrix(size_t start_row, size_t nrows, size_t start_col, size_t ncols) const;
    matrix<T> sub_matrix(size_t start_row, size_t start_col) const;
    matrix<T> sub_col(size_t start_row, size_t nrows, size_t c) const;

    matrix<T>& set_sub_matrix(const matrix<T>& sub, size_t start_row, size_t start_col);
    matrix<T>& set_sub_col(const matrix<T>& sub, size_t start_row, size_t c);
    
    //matrix<matrix<T>> partition(int x, int y);
    //matrix<matrix<T>> partition(const matrix<std::pair<size_t, size_t>>& shapes);

    //matrix<T>* split(int at_row, int at_col);
    
    size_t rows(void) const { return m_rows; }
    size_t cols(void) const { return m_cols; }
    size_t size(void) const { return m_size; }
    
    std::pair<size_t, size_t> shape(void) const { return m_shape; }
    
    std::pair<size_t, size_t> transpose_shape(void) const
    {
        return std::make_pair(m_shape.second, m_shape.first);
    }
    
    std::pair<size_t, size_t> rvec_shape(void) const
    {
        return std::make_pair(1, m_shape.second);
    }
    
    std::pair<size_t, size_t> cvec_shape(void) const
    {
        return std::make_pair(m_shape.first, 1);
    }
    
    bool is_square(void) const { return (m_rows == m_cols); }
    bool is_symmetric(void) const;
    bool is_row_vector(void) const;
    bool is_col_vector(void) const;
    
    static double col_norm2sq_from(const matrix<T>& mat, size_t c, size_t from);
    static double col_norm2(const matrix<T>& mat, size_t c);
    static matrix<double> cols_norm2sq(const matrix<T>& mat);

private:
    
    size_t m_rows;
    size_t m_cols;
    size_t m_size;
    
    std::pair<size_t, size_t> m_shape;
    
    T* bptr;
    
};


template<class T>
bool operator==(const matrix<T>& lhs, const matrix<T>& rhs)
{
    return lhs.equals(rhs);
}

template<class T>
bool operator!=(const matrix<T>& lhs, const matrix<T>& rhs)
{
    return !lhs.equals(rhs);
}

template<class T>
matrix<T> operator*(T scalar, const matrix<T>& mat)
{
    matrix<T> smat(mat);
    for(size_t i=0; i < mat.size(); i++)
    {
        smat[i] *= scalar;
    }
    return smat;
}

template<class T>
matrix<T> append_horizontal(const matrix<T>* lhs, const matrix<T>* rhs);

template<class T>
matrix<T> append_vertical(const matrix<T>* lhs, const matrix<T>* rhs);

template<class T>
matrix<T> outer_prod_1D(const matrix<T>& cvec, const matrix<T>& rvec)
{
    size_t N = cvec.rows();
    size_t M = rvec.cols();
    matrix<T> ret(N, M);
    
    for(size_t r=0; r < N; r++)
    {
        for(size_t c=0; c < M; c++)
        {
            ret(r, c) = cvec(r, 0) * rvec(0, c);
        }
    }
    
    return ret;
}

template<class T>
T inner_prod_1D(const matrix<T>& rvec, const matrix<T>& cvec, size_t offs)
{
    T* rb = rvec.base_ptr();
    return std::inner_product(rb + offs, rb + rvec.size(), cvec.base_ptr() + offs, static_cast<T>(0.0));
}

template<class T>
T inner_prod_1D(const matrix<T>& rvec, const matrix<T>& cvec)
{
    return inner_prod_1D(rvec, cvec, 0);
}

template<class T>
matrix<T> inner_left_prod(const matrix<T>& rvec, const matrix<T>& cvecs)
{
    matrix<T> rv(rvec);
    if(rv.is_col_vector())
    {
        rv = rv.transpose();
    }
    
    if(rv.cols() != cvecs.rows())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    matrix<T> res(1, cvecs.cols());
    for(size_t c=0; c < cvecs.cols(); c++)
    {
        res(0, c) = inner_prod_1D(rv, cvecs.col(c), 0);
    }
    
    //std::cout << "inner_left_prod = \n";
    //res.print();
    
    return res;
}

template<class T>
matrix<T> inner_right_prod(const matrix<T>& rvecs, const matrix<T>& cvec)
{
    matrix<T> cv(cvec);
    if(cv.is_row_vector())
    {
        cv = cv.transpose();
    }
    
    if(cv.rows() != rvecs.cols())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    matrix<T> res(rvecs.rows(), 1);
    for(size_t r=0; r < rvecs.rows(); r++)
    {
        res(r, 0) = inner_prod_1D(rvecs.row(r), cv, 0);
    }
    
    return res;
}

template<class T>
matrix<T> proj(const matrix<T>& u, const matrix<T>& v)
{
    return (inner_prod_1D(v, u)/inner_prod_1D(u, u)) * u;
}

template<class T>
matrix<matrix<T>> as_rvecs(const matrix<T>& rhs)
{
    matrix<T>* rvecs = new matrix<T>[rhs.rows()];
    for(size_t r=0; r < rhs.rows(); r++)
    {
        rvecs[r] = rhs.row(r);
    }
    
    return matrix<matrix<T>>(rhs.rows(), 1, rvecs);
}

template<class T>
matrix<matrix<T>> as_cvecs(const matrix<T>& rhs)
{
    matrix<T>* cvecs = new matrix<T>[rhs.cols()];
    for(size_t c=0; c < rhs.cols(); c++)
    {
        cvecs[c] = rhs.col(c);
    }
    
    return matrix<matrix<T>>(1, rhs.cols(), cvecs);
}

template<class T>
matrix<T> from_cvecs(const matrix<matrix<T>>& cvecs)
{
    matrix<T> res(cvecs(0, 0).rows(), cvecs.cols());
    for(size_t c=0; c < cvecs.cols(); c++)
    {
        res.set_col(cvecs(0, c), c);
    }
    
    return res;
}


template<class T>
matrix<T> matrix<T>::identity(size_t m)
{
    matrix<T> I(m, m);
    I.set_identity();
    return I;
}

template<> 
matrix<size_t> matrix<size_t>::unit_permutation_matrix(size_t order)
{
    matrix<size_t> perm(order, 1);
    for(size_t i=0; i < order; i++)
    {
        perm(i, 0) = i;
    }
    return perm;
}

template<class T>
matrix<T> matrix<T>::random_dense_matrix(size_t m, size_t n, float lowerbound, float upperbound)
{
    matrix<T> rmat(m, n);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lowerbound, upperbound);
    
    for(size_t i=0; i < rmat.size(); i++)
    {
        rmat[i] = static_cast<T>(dis(gen));
    }
    
    return rmat;
}

template<class T>
matrix<T>::matrix()
{
    m_rows = m_cols = m_size = 0;
    m_shape.first = m_rows;
    m_shape.second = m_cols;
    bptr = nullptr;
}
 
template<class T>
matrix<T>::matrix(size_t r, size_t c)
: m_rows(r), m_cols(c), m_size(r * c), m_shape(r, c)
{
    bptr = new T[(r * c)];
}

template<class T>
matrix<T>::matrix(size_t r, size_t c, const T* data)
: matrix<T>(r, c)
{
    MEMCPY(bptr, data, m_size);
}

template<class T>
matrix<T>::matrix(size_t r, size_t c, std::initializer_list<T> data)
: matrix<T>(r, c)
{
    std::move(data.begin(), data.begin() + std::min(m_size, data.size()), bptr);
}

template<class T>
matrix<T>::matrix(std::initializer_list<T> data)
: matrix<T>(data.size(), 1, data)
{
}

template<class T>
matrix<T>& matrix<T>::reshape(size_t new_r, size_t new_c)
{
    size_t new_s = new_r * new_c;
    
    if(new_s != m_size)
    {
        throw std::range_error("reshape requires that new dimensions fit the existing size. Use resize or realloc to resize and/or reshape.");
    }
    
    m_size = new_s;
    m_rows = new_r;
    m_cols = new_c;
    m_shape.first = new_r;
    m_shape.second = new_c;
}

template<class T>
matrix<T>::matrix(const matrix<T>& from)
: matrix<T>(from.rows(), from.cols(), from.base_ptr()) {}

template<class T>
matrix<T>::matrix(matrix<T>&& from) noexcept
{
    bptr = (T*)std::move(from.base_ptr());
    
    m_rows = from.rows();
    m_cols = from.cols();
    m_size = from.size();
    m_shape = from.shape();
}

template<class T>
matrix<T>::~matrix()
{
    //delete bptr;
}

template<class T>
T* matrix<T>::base_ptr(void) const
{
    return bptr;
}

template<class T>
T* matrix<T>::top_ptr(void) const
{
    return &bptr[m_size];
}

template<class T>
inline T matrix<T>::get_value(size_t m, size_t n) const
{
    return bptr[OFFS(m, n)];
}

template<class T>
inline T matrix<T>::get_value(size_t offs) const
{
    return bptr[offs];
}

template<class T>
inline T& matrix<T>::operator[](size_t offs)
{
    return (T&)bptr[offs];
}

template<class T>
const T matrix<T>::operator()(size_t m, size_t n) const
{
    return bptr[OFFS(m, n)];
}

template<class T>
T& matrix<T>::operator()(size_t m, size_t n)
{
    return (T&)bptr[OFFS(m, n)];
}

template<class T>
inline void matrix<T>::set_value(size_t m, size_t n, T value)
{
    bptr[OFFS(m, n)] = value;
}

template<class T>
inline void matrix<T>::set_value(size_t offs, T value)
{
    bptr[offs] = value;
}

template<class T>
void matrix<T>::fill(T value)
{
    for(size_t k=0; k < m_size; k++)
    {
        bptr[k] = value;
    }
}

template<class T>
void matrix<T>::zero(void)
{
    fill((T)0);
}

template<class T>
void matrix<T>::ones(void)
{
    fill((T)1);
}

template<class T>
void matrix<T>::set_identity(void)
{
    if(rows() != cols())
    {
        throw std::length_error("matrix must be square.");
    }
    
    zero();
    
    for(size_t r=0; r < m_rows; r++)
    {
        (*this)(r, r) = (T)1;
    }
}

template<class T>
inline void matrix<T>::zero_lower_triangle(void)
{
    for(size_t c = 0; c < m_cols; c++)
    {
        for(size_t r = c + 1; r < m_rows; r++)
        {
            (*this)(r, c) = static_cast<T>(0.0);
        }
    }
}

template<class T>
inline void matrix<T>::zero_upper_triangle(void)
{
    for(size_t r = 0; r < m_rows; r++)
    {
        for(size_t c = r + 1; c < m_cols; c++)
        {
            (*this)(r, c) = static_cast<T>(0.0);
        }
    }
}

template<class T>
inline void matrix<T>::print(void) const
{
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c=0; c < m_cols; c++)
        {
            std::cout << (*this)(r, c) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

template<class T>
bool matrix<T>::content_equals(const matrix<T>& other) const
{
    if(size() != other.size())
    {
        return false;
    }
    
    for(size_t k=0; k < m_size; k++)
    {
        if(bptr[k] != other.get_value(k))
        {
            return false;
        }
    }
    
    return true;
}

template<class T>
bool matrix<T>::equals(const matrix<T>& other) const
{
    if(rows() != other.rows() || cols() != other.cols())
    {
        return false;
    }
    
    return content_equals(other);
}

template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& from)
{
    if(this != &from)
    {
        if(m_size != from.size())
        {
            delete bptr;
            
            bptr = new T[from.size()];
            m_rows = from.rows();
            m_cols = from.cols();
            m_size = from.size();
            m_shape = from.shape();
            
        }
        
        MEMCPY(bptr, from.base_ptr(), m_size);
    }
    
    return *this;
}


template<class T>
matrix<T>& matrix<T>::operator=(matrix<T>&& from)
{
    if(this != &from)
    {
        bptr = std::move(from.base_ptr());
        
        m_rows = from.rows();
        m_cols = from.cols();
        m_size = from.size();
        m_shape = from.shape();
    }
    
    return *this;
}

template<class T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& rhs)
{
    if(size() != rhs.size())
    {
        throw std::length_error("sizes must be equal!");
    }
    
    for(size_t k=0; k < m_size; k++)
    {
        bptr[k] = bptr[k] + rhs.get_value(k);
    }
    
    return *this;
}

template<class T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& rhs)
{
    if(size() != rhs.size())
    {
        throw std::length_error("sizes must be equal!");
    }
    
    for(size_t k=0; k < m_size; k++)
    {
        bptr[k] = bptr[k] - rhs.get_value(k);
    }
    
    return *this;
}


template<class T>
matrix<T> matrix<T>::row(size_t r) const
{
    if(r >= rows())
    {
        throw std::range_error("selected row is out of range.");
    }
    
    return matrix<T>(1, cols(), bptr + ROW_OFFS(r));
}

template<class T>
matrix<T>& matrix<T>::set_row(const matrix<T>& rvec, size_t r)
{
    MEMCPY(bptr[ROW_OFFS(r)], rvec.base_ptr(), rvec.cols());
    return *this;
}

template<class T>
matrix<T> matrix<T>::col(size_t c) const
{
    if(c >= cols())
    {
        throw std::range_error("selected column is out of range.");
    }
    
    matrix<T> cvec(rows(), 1);
    for(int r=0; r < m_rows; r++)
    {
        cvec.set_value(r, 0, get_value(r, c));
    }
  
    return cvec;
}

template<class T>
matrix<T>& matrix<T>::set_col(const matrix<T>& cvec, size_t c)
{
    if(c >= cols())
    {
        throw std::range_error("selected column is out of range.");
    }
    
    for(size_t r=0; r < m_rows; r++)
    {
        (*this)(r, c) = cvec(r, 0);
    }
    
    return *this;
}

template<class T>
matrix<T>& matrix<T>::swap_rows(size_t r1, size_t r2)
{
    if(r1 < rows() && r2 < rows())
    {
        size_t c1 = ROW_OFFS(r1);
        size_t c2 = ROW_OFFS(r2);
        size_t i=0;
                
        for(; i < m_cols; c1++, c2++, i++)
        {
            T tmp = bptr[c1];
            set_value(c1, bptr[c2]);
            set_value(c2, tmp);
        }
    }
    
    return *this;
}

template<class T>
matrix<T>& matrix<T>::swap_cols(size_t c1, size_t c2)
{
    if(c1 < cols() && c2 < cols())
    {
        size_t r1, r2;
        T t1, t2;
        for(size_t r=0; r < m_rows; r++)
        {
            r1 = OFFS(r, c1);
            r2 = OFFS(r, c2);
            
            t1 = bptr[r1];
            t2 = bptr[r2];
            
            set_value(r1, t2);
            set_value(r2, t1);
        }
    }
    return *this;
}

template<class T>
matrix<T>& matrix<T>::sub_rows(size_t r1, size_t r2, T factor)
{
    if(r1 < rows() && r2 < rows())
    {
        size_t c1 = ROW_OFFS(r1);
        size_t c2 = ROW_OFFS(r2);
        size_t i = 0;
        
        for(; i < m_cols; c1++, c2++, i++)
        {
            set_value(c1, bptr[c1] - factor * bptr[c2]);
        }
    }
    
    return *this;
}

template<class T>
matrix<T>& matrix<T>::permute_rows(const matrix<size_t>& rpermute)
{
    if(rpermute.rows() != rows())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    matrix<T> to(*this);
    
    for(size_t r=0; r < rpermute.rows(); r++)
    {
        to.set_row(row(rpermute(r, 0)), r);
    }
    
    *this = std::move(to);
    return *this;
}


template<class T>
matrix<T> matrix<T>::transpose(void)
{
    matrix<T> tm(cols(), rows());
    for(size_t r=0; r < m_rows; r++)
    {
        for(size_t c=0; c < m_cols; c++)
        {
            tm(c, r) = get_value(r, c);
        }
    }
    
    return tm;
}

template<class T>
matrix<T> matrix<T>::diag(void)
{
    matrix<T> ret;
    if(is_row_vector())
    {
        ret = matrix<T>(rows(), rows());
        for(size_t i=0; i < rows(); i++)
        {
            ret(i, i) = (*this)(i, 0);
        }
    }
    else if(is_col_vector())
    {
        ret = matrix<T>(cols(), cols());
        for(size_t i=0; i < cols(); i++)
        {
            ret(i, i) = (*this)(0, i);
        }
    }
    else
    {
        throw std::range_error("matrix is not a row or column vector.");
    }
    
    return ret;
}


template<class T>
matrix<T>* matrix<T>::split_rows(size_t at_row)
{
    if(at_row >= rows())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    matrix<T>* split = new matrix<T>[2]
    {
        matrix<T>(at_row + 1, cols(), bptr + ROW_OFFS(0)),
        matrix<T>(rows() - at_row - 1, cols(), bptr + OFFS(at_row, cols()))
    };

    
    return split;
}

template<class T>
matrix<T>* matrix<T>::split_cols(size_t at_col)
{
    if(at_col >= cols())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    matrix<T>* split = new matrix<T>[2]
    {
        matrix<T>(rows(), at_col + 1),
        matrix<T>(rows(), cols() - at_col - 1)
    };
    
    size_t l_idx, r_idx, r;
    l_idx = r_idx = r = 0;
    
    for(; r < m_rows; r++)
    {
        for(size_t c=0; c < m_cols; c++)
        {
            T val = get_value(r, c);
            if(c <= at_col)
            {
                split[0][l_idx++] = val;
            }
            else
            {
                split[1][r_idx++] = val;
            }
        }
    }
    
    return split;
}

template<class T>
matrix<T> matrix<T>::sub_matrix(size_t start_row, size_t nrows, size_t start_col, size_t ncols) const
{
    if(start_row + nrows > rows() || start_col + ncols > cols())
    {
        std::string err = std::format("incompatible dimensions for submatrix operation.\nGot start_row = {}, nrows = {}, rows = {}, start_col = {}, ncols = {}, cols = {}.\n", start_row, nrows, rows(), start_col, ncols, cols());
        throw std::range_error(err);
    }
    
    matrix<T> sub(nrows, ncols);
    T* dst_ptr = sub.base_ptr();
    T* src_ptr = base_ptr() + start_row * cols() + start_col;
    
    for(size_t r=0; r < nrows; r++, src_ptr += cols(), dst_ptr += ncols)
    {
        std::copy(src_ptr, src_ptr + ncols, dst_ptr);
    }
    
    return sub;
}

// TODO: subview object
template<class T>
matrix<T> matrix<T>::sub_matrix(size_t start_row, size_t start_col) const
{
    return sub_matrix(start_row, rows() - start_row, start_col, cols() - start_col);
}

template<class T>
matrix<T> matrix<T>::sub_col(size_t start_row, size_t nrows, size_t c) const
{
    return sub_matrix(start_row, nrows, c, 1);
}

template<class T>
matrix<T>& matrix<T>::set_sub_matrix(const matrix<T>& sub, size_t start_row, size_t start_col)
{
    if(start_row + sub.rows() > rows() || start_col + sub.cols() > cols())
    {
        throw std::range_error("incompatible dimensions for submatrix operation.");
    }
    
    for(size_t r=0; r < sub.rows(); r++)
    {
        for(size_t c=0; c < sub.cols(); c++)
        {
            (*this)(r + start_row, c + start_col) = sub(r, c);
        }
    }
    return *this;
}

template<class T>
matrix<T>& matrix<T>::set_sub_col(const matrix<T>& sub, size_t start_row, size_t c)
{
    return set_sub_matrix(sub, start_row, c);
}



template<class T>
matrix<T> append_horizontal(const matrix<T>* lhs, const matrix<T>* rhs)
{
    if(lhs->rows() != rhs->rows())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    size_t nrows = lhs->rows();
    size_t ncols = lhs->cols() + rhs->cols();
    
    matrix<T> ret(nrows, ncols);
    T* row_ptr = (T*)ret.base_ptr();
    
    for(size_t r = 0; r < nrows; r++)
    {
        std::memcpy
        (
            (void *)row_ptr,
            (const void *)(lhs->base_ptr() + (r * lhs->cols())),
            lhs->cols() * sizeof(T)
        );
        
        std::memcpy
        (
            (void *)(row_ptr + lhs->cols()),
            (const void *)(rhs->base_ptr() + (r * rhs->cols())),
            rhs->cols() * sizeof(T)
        );
        
        row_ptr += ret.cols();
    }
    
    return ret;
}

template<class T>
matrix<T> append_vertical(const matrix<T>* lhs, const matrix<T>* rhs)
{
    if(lhs->cols() != rhs->cols())
    {
        throw std::range_error("incorrect dimensions for operation.");
    }
    
    size_t nrows = lhs->rows() + rhs->rows();
    size_t ncols = lhs->cols();
    
    matrix<T> ret(nrows, ncols);
    const T* row_ptr = ret.base_ptr();
    
    std::memcpy
    (
        (void *)row_ptr,
        (const void *)lhs->base_ptr(),
        lhs->size()*sizeof(T)
    );
    
    std::memcpy
    (
        (void *)(row_ptr + lhs->size()),
        (const void *)rhs->base_ptr(),
        rhs->size()*sizeof(T)
    );
    
    return ret;
}

template<class T>
bool matrix<T>::is_symmetric(void) const
{
    matrix<T> cpy(*this);
    return (*this == cpy.transpose());
}

template<class T>
bool matrix<T>::is_col_vector(void) const
{
    return (cols() == 1) && (rows() > 1);
}

template<class T>
bool matrix<T>::is_row_vector(void) const
{
    return (cols() > 1) && (rows() == 1);
}

/*
 * Norms:
 *
 * if a is type T, and A, B are m x n
 * a norm must satisfy the following:
 *  - ||A|| > 0
 *  - ||A|| = 0 IFF A.zero()
 *  - ||aA|| = |a|(||A||)
 *  - ||A + B|| <= ||A|| + ||B||
 *
 * Also useful if:
 *  - ||AB|| <= ||A||(||B||)
 */

// 1-norm is the column whose absoulte sum is greatest
// inf-norm is the row whose absoulte sum is greatest
// 2-norm is harder:
// TODO: generalized 2-norm -- determine max eigenvalue via power iteration
//       can already be implemented with a jacobi reduction, but we only need the max eigenval
//       which a power iteration can do with less computational work.
/*
 * if A is 1xN or Mx1, then 2-norm is the sqrt(sum of squares)
 * else 2-norm(A) = sqrt(max(lambda)(A^(*)A)
 *
 * in both cases, the else statement is true,
 * but single row/column matries can be handled seperately since
 * the computation is not as intensive
 *
 * 2-norm: find max(lambda) via a power iteration/rayleigh quotient:
 *      https://www.robots.ox.ac.uk/~sjrob/Teaching/EngComp/ecl4.pdf
 */

template<class T>
double matrix<T>::col_norm2sq_from(const matrix<T>& mat, size_t c, size_t from)
{
    if(c >= mat.cols())
    {
        throw std::range_error("column is out of range.");
    }
    
    double square = 0.0;
    for(size_t r=from; r < mat.rows(); r++)
    {
        square += std::pow(static_cast<double>(mat(r, c)), 2);
    }
    
    return square;
}

template<class T>
double matrix<T>::col_norm2(const matrix<T>& mat, size_t c)
{
    return std::sqrt(matrix<T>::col_norm2sq_from(mat, c, 0));
}

template<class T>
matrix<double> matrix<T>::cols_norm2sq(const matrix<T>& mat)
{
    matrix<double> norms(1, mat.cols());
    
    for(size_t c=0; c < mat.cols(); c++)
    {
        double norm = 0.0;
        for(size_t r=0; r < mat.rows(); r++)
        {
            double mrc = static_cast<double>(mat(r, c));
            norm += mrc * mrc;
        }
        norms(0, c) = norm;
    }
    
    return norms;
}


//#include "matrix.cpp"

#endif
