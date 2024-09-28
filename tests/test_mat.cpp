//
//  test_matrix.cpp
//  Created by Ben Westcott on 9/8/24.
//

TEST_CASE("default constructor")
{
    std::cout << "test default constructor:\n";

    matrix<int> mat_def;
    
    REQUIRE(mat_def.size() == 0);
    REQUIRE(mat_def.rows() == 1);
    REQUIRE(mat_def.cols() == 1);
    REQUIRE(mat_def.data() == nullptr);
    
}

TEST_CASE("size constructor")
{
    std::cout << "\n\ntest size constructor:\n";

    matrix<int> mat(9);
    
    REQUIRE(mat.size() == 9);
    REQUIRE(mat.rows() == 9);
    REQUIRE(mat.cols() == 1);
    REQUIRE(mat.data() != nullptr);
}

TEST_CASE("size, data constructor")
{
    std::cout << "\n\ntest size, data constructor:\n";
    
    int dat[9] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
    
    matrix<int> mat(9, dat);
    
    int* data_ptr = mat.data();
    for(size_t r=0; r < mat.size(); r++, data_ptr++)
    {
        REQUIRE(*data_ptr == dat[r]);
    }
    
}

TEST_CASE("reshape")
{
    std::cout << "\n\ntest reshape member function:\n";
    
    matrix<int> mat(9);
    
    mat.reshape(3, 3);
    
    REQUIRE(mat.size() == 9);
    REQUIRE(mat.rows() == 3);
    REQUIRE(mat.cols() == 3);
}

TEST_CASE("row, col constructor")
{
    std::cout << "\n\ntest row col constructor:\n";
    
    matrix<int> mat(6, 6);
    
    REQUIRE(mat.size() == 6*6);
    REQUIRE(mat.rows() == 6);
    REQUIRE(mat.cols() == 6);
    REQUIRE(mat.data() != nullptr);
}

TEST_CASE("row, col, data constructor")
{
    std::cout << "\n\ntest row, col, data constructor:\n";
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
        
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N, dat);

    int* mat_ptr = mat.data();
    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(mat_ptr[i] == dat[i]);
    }

    delete [] dat;
}

TEST_CASE("copy constructor")
{
    std::cout << "\n\ntest copy constructor:\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N, dat);
    int *mat_ptr = mat.data();
    
    matrix<int> matcpy(mat);
    int *mat_cpy_ptr = matcpy.data();
    
    REQUIRE(mat.rows() == matcpy.rows());
    REQUIRE(mat.cols() == matcpy.cols());
    REQUIRE(mat.size() == matcpy.size());
    
    for(size_t i = 0; i < mat.size(); i++)
    {
        REQUIRE(mat_ptr[i] == mat_cpy_ptr[i]);
    }

    delete [] dat;
}

TEST_CASE("initializer list constructor")
{
    std::cout << "\n\ntest intializer list constructor:\n";
    
    int lst_dat[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> mat_implicit = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    int *mat_ptr = mat_implicit.data();
    
    for(size_t i = 0; i < mat_implicit.size(); i++)
    {
        REQUIRE(lst_dat[i] == mat_ptr[i]);
    }
}

TEST_CASE("size initializer list constructor")
{
    std::cout << "\n\ntest size intializer list constructor:\n";
    
    int lst_dat[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // takes min(20, 9) = 9
    matrix<int> mat1(20, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    int* mat1_ptr = mat1.data();
    
    for(size_t i=0; i < 9; i++)
    {
        REQUIRE(mat1_ptr[i] == lst_dat[i]);
    }
    
    // takes min(8, 9) = 8
    matrix<int> mat2(8, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    int* mat2_ptr = mat2.data();
    
    for(size_t i=0; i < 8; i++)
    {
        REQUIRE(mat2_ptr[i] == lst_dat[i]);
    }
    
}

TEST_CASE("row col initializer list constructor")
{
    std::cout << "\n\ntest row col initializer list constructor:\n";
    
    int lst_dat[9] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
    
    // takes min(5*5, 9) = 9
    matrix<int> mat1(5, 5, {2, 4, 6, 8, 10, 12, 14, 16, 18});
    int* mat1_ptr = mat1.data();
    
    for(size_t i=0; i < 9; i++)
    {
        REQUIRE(mat1_ptr[i] == lst_dat[i]);
    }
    
    // takes min(2*3, 9) = 6
    matrix<int> mat2(2, 3, {2, 4, 6, 8, 10, 12, 14, 16, 18});
    int* mat2_ptr = mat2.data();
    
    for(size_t i=0; i < 6; i++)
    {
        REQUIRE(mat2_ptr[i] == lst_dat[i]);
    }
}

TEST_CASE("copy assignment")
{
    std::cout << "\n\ntest copy assignment:\n";
    
    size_t M1 = S_RAND(100);
    size_t N1 = S_RAND(100);
    
    int *dat1 = generate_random_ints((M1 * N1), 1);
    matrix<int> m1(M1, N1, dat1);
    
    size_t M2 = S_RAND(100);
    size_t N2 = S_RAND(100);
    
    int *dat2 = generate_random_ints((M2 * N2), 1);
    matrix<int> m2(M2, N2, dat2);
    
    m1 = m2;

    REQUIRE(m1.rows() == M2);
    REQUIRE(m1.cols() == N2);
    REQUIRE(m1.size() == (M2 * N2));
    REQUIRE(m2.rows() == M2);
    REQUIRE(m2.cols() == N2);
    REQUIRE(m2.size() == (M2 * N2));
    
    int *m1_ptr = m1.data();
    int *m2_ptr = m2.data();
    
    for(size_t i=0; i < m1.size(); i++)
    {
        REQUIRE(m1_ptr[i] == dat2[i]);
        REQUIRE(m2_ptr[i] == dat2[i]);
    }

    delete [] dat1;
    delete [] dat2;
}


TEST_CASE("move constructor")
{
    std::cout << "\n\ntest move constructor:\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 1);
    
    matrix<int> mfrom(M, N, dat);
    
    matrix<int> mto = std::move(mfrom);
    
    REQUIRE(mto.rows() == M);
    REQUIRE(mto.cols() == N);
    REQUIRE(mto.size() == (M * N));
    
    int *mto_ptr = mto.data();
    for(size_t i=0; i < mto.size(); i++)
    {
        REQUIRE(mto_ptr[i] == dat[i]);
    }

    delete [] dat;
}

TEST_CASE("move assignment")
{
    std::cout << "\n\ntest move assignment:\n";
    
    size_t M1 = S_RAND(100);
    size_t N1 = S_RAND(100);
    int *dat1 = generate_random_ints((M1 * N1), 1);
    matrix<int> m1(M1, N1, dat1);
    
    size_t M2 = S_RAND(100);
    size_t N2 = S_RAND(100);
    int *dat2 = generate_random_ints((M2 * N2), 1);
    matrix<int> m2(M2, N2, dat2);
    
    m1 = std::move(m2);
    
    REQUIRE(m1.rows() == M2);
    REQUIRE(m1.cols() == N2);
    REQUIRE(m1.size() == (M2 * N2));
    
    int *m1_ptr = m1.data();
    for(size_t i=0; i < m1.size(); i++)
    {
        REQUIRE(m1_ptr[i] == dat2[i]);
    }

    delete [] dat1;
    delete [] dat2;
}

TEST_CASE("is row vector")
{
    std::cout << "\n\ntest is_row_vector:\n";

    
    size_t M = S_RAND(100);
    int *dat = generate_random_ints(M, 1);
    
    matrix<int> m1(1, M, dat);
    REQUIRE(m1.is_row_vector());
    
    matrix<int> m2(M, 1, dat);
    REQUIRE(!m2.is_row_vector());
    
    matrix<int> m3(1, 1, dat);
    REQUIRE(m3.is_row_vector());

    delete [] dat;
}

TEST_CASE("is col vector")
{
    std::cout << "\n\ntest is_col_vector:\n";

    size_t N = S_RAND(100);
    int *dat = generate_random_ints(N, 1);
    
    matrix<int> m1(1, N, dat);
    REQUIRE(!m1.is_col_vector());
    
    matrix<int> m2(N, 1, dat);
    REQUIRE(m2.is_col_vector());
    
    matrix<int> m3(1, 1, dat);
    REQUIRE(m3.is_col_vector());

    delete [] dat;
}

TEST_CASE("get value offs")
{
    std::cout << "\n\ntest get_value(offs):\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 1);
    
    matrix<int> mat(M, N, dat);
    
    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(dat[i] == mat.get_value(i));
    }

    delete [] dat;
}

TEST_CASE("set value offs")
{
    std::cout << "\n\ntest set_value(offs)\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N);
    
    for(size_t i=0; i < mat.size(); i++)
    {
        mat.set_value(i, dat[i]);
        REQUIRE(mat.get_value(i) == dat[i]);
    }
    
    delete [] dat;

}

TEST_CASE("operator [] offs")
{
    std::cout << "\n\ntest operator[]\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N);
    
    for(size_t i=0; i < mat.size(); i++)
    {
        mat[i] = dat[i];
        REQUIRE(mat[i] == dat[i]);
    }

    delete [] dat;

}

TEST_CASE("row offset")
{
    std::cout << "\n\ntest row_offset(m, n):\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N, dat);
    
    size_t i=0;
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++, i++)
        {
            REQUIRE(mat[mat.row_offset(r) + c] == dat[i]);
        }
    }

    delete [] dat;

}

TEST_CASE("offset")
{
    std::cout << "\n\ntest offset(m, n):\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N, dat);

    size_t i=0;
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++, i++)
        {
            REQUIRE(mat[mat.offset(r, c)] == dat[i]);
        }
    }

    delete [] dat;
    
}

TEST_CASE("get value row col")
{
    std::cout << "\n\ntest get_value(m, n):\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N, dat);

    size_t i=0;
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++, i++)
        {
            REQUIRE(mat.get_value(r, c) == dat[i]);
        }
    }

    delete [] dat;
    
}

TEST_CASE("set value row col")
{
    std::cout << "\n\ntest set_value(m, n):\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N);

    size_t i=0;
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++, i++)
        {
            mat.set_value(r, c, dat[i]);
            REQUIRE(mat.get_value(r, c) == dat[i]);
        }
    }

    delete [] dat;

}

TEST_CASE("operator () row col")
{
    std::cout << "\n\ntest operator()(m, n):\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    int *dat = generate_random_ints((M * N), 10);
    
    matrix<int> mat(M, N);

    size_t i=0;
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++, i++)
        {
            mat(r, c) = dat[i];
            REQUIRE(mat(r, c) == dat[i]);
        }
    }

    delete [] dat;
}

TEST_CASE("fill")
{
    std::cout << "\n\ntest fill:\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    size_t rand_value = S_RAND(100);

    matrix<int> mat(M, N);

    mat.fill(rand_value);

    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(mat[i] == rand_value);
    }

}

TEST_CASE("zero")
{
    std::cout << "\n\ntest zero:\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);

    matrix<int> mat(M, N);

    mat.zero();

    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(mat[i] == 0);
    }

}

TEST_CASE("ones")
{
    std::cout << "\n\ntest ones:\n";

    size_t M = S_RAND(100);
    size_t N = S_RAND(100);

    matrix<int> mat(M, N);

    mat.ones();

    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(mat[i] == 1);
    }

}

TEST_CASE("rfill")
{
    /*
    std::cout << "\n\ntest rfill:\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    size_t col_bias = S_RAND(100);
    while(col_bias >= N) col_bias = S_RAND(100);
    
    double val = S_RAND(100);
    */
    // TODO: idk

}

TEST_CASE("fill upper triangle")
{
    std::cout << "\n\ntest fill_upper_triangle:\n";
    
    

}

TEST_CASE("fill upper hessenberg")
{
    std::cout << "\n\nfill_upper_hessenberg:\n";
    
}

TEST_CASE("cfill")
{
    std::cout << "\n\ntest cfill:\n";

}

TEST_CASE("fill lower triangle")
{
    std::cout << "\n\ntest fill_lower_triangle:\n";

}

TEST_CASE("fill lower hessenberg")
{
    std::cout << "\n\ntest fill_lower_hessenberg:\n";

}

TEST_CASE("set identity")
{
    std::cout << "\n\nset_identity:\n";
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    matrix<double> mrand = matrix<double>::random_dense_matrix(M, M, 1000, -1000);
    mrand.set_identity();
    
    for(size_t r=0; r < mrand.rows(); r++)
    {
        for(size_t c=0; c < mrand.cols(); c++)
        {
            double val = mrand.get_value(r, c);
            if(r == c)
            {
                REQUIRE(val == 1.0);
            }
            else REQUIRE(val == 0.0);
        }
    }
}

TEST_CASE("content equals")
{
    std::cout << "\n\ntest content_equals:\n";
    
    int data[6] = {2, 4, 6, 8, 10, 12};
    matrix<int> m1(3, 2, data);
    matrix<int> m2(2, 3, data);
    
    REQUIRE(m1.content_equals(m2));
    
    int data2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> m3(3, 3, data2);
    matrix<int> m4(3, 3, data2);
    
    REQUIRE(m3.content_equals(m4));
    
    matrix<int> m5(3, 2, data);
    matrix<int> m6(3, 3, data2);
    
    REQUIRE(!m5.content_equals(m6));
    
    matrix<int> m7(3, 3, data2);
    matrix<int> m8(3, 3, data);
    
    REQUIRE(!m7.content_equals(m8));

}

TEST_CASE("equals")
{
    std::cout << "\n\ntest equals:\n";
    
    int data[6] = {2, 4, 6, 8, 10, 12};
    matrix<int> m1(3, 2, data);
    matrix<int> m2(2, 3, data);
    
    REQUIRE(!m1.equals(m2));
    REQUIRE(!(m1 == m2));
    REQUIRE(m1 != m2);
    
    int data2[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> m3(3, 3, data2);
    matrix<int> m4(3, 3, data2);
    
    REQUIRE(m3.equals(m4));
    REQUIRE(m3 == m4);
    REQUIRE(!(m3 != m4));
    
    matrix<int> m5(3, 2, data);
    matrix<int> m6(3, 3, data2);
    
    REQUIRE(!m5.equals(m6));
    REQUIRE(!(m5 == m6));
    REQUIRE(m5 != m6);
    
    matrix<int> m7(3, 3, data2);
    matrix<int> m8(3, 3, data);
    
    REQUIRE(!m7.equals(m8));
    REQUIRE(!(m7 == m8));
    REQUIRE(m7 != m8);

}

TEST_CASE("is symmetric")
{
    std::cout << "\n\nis_symmetric:\n";

}

TEST_CASE("operator +=")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int data3[9] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
    int dsum[9] = {4, 8, 12, 16, 20, 24, 28, 32, 36};
    
    matrix<int> m1(3, 3, data);
    matrix<int> m2(3, 3, data3);
    matrix<int> m1pm2(3, 3, dsum);
    
    REQUIRE_NOTHROW(m1 += m2);
    REQUIRE(m1 == m1pm2);
    
    matrix<int> m3(2, 2, data);
    REQUIRE_THROWS(m1 += m3);
    REQUIRE_THROWS(m3 += m1);
}

TEST_CASE("operator -=")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int data3[9] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
    int dsum[9] = {-2,-4, -6, -8, -10, -12, -14, -16, -18};
    
    matrix<int> m1(3, 3, data);
    matrix<int> m2(3, 3, data3);
    matrix<int> m1pm2(3, 3, dsum);
    
    REQUIRE_NOTHROW(m1 -= m2);
    REQUIRE(m1 == m1pm2);
    
    matrix<int> m3(2, 2, data);
    REQUIRE_THROWS(m1 -= m3);
}

TEST_CASE("row")
{
    int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    matrix<int> mat(4, 4, data);
    
    int rdat0[4] = {2, 4, 6, 8};
    int rdat1[4] = {10, 12, 14, 16};
    int rdat2[4] = {18, 20, 22, 24};
    int rdat3[4] = {26, 28, 30, 32};
    
    matrix<int> m0(1, 4, rdat0), m1(1, 4, rdat1), m2(1, 4, rdat2), m3(1, 4, rdat3);
    matrix<int> c0, c1, c2, c3, c4;
    
    REQUIRE_NOTHROW(c0 = mat.row(0));
    REQUIRE(c0 == m0);
    
    REQUIRE_NOTHROW(c1 = mat.row(1));
    REQUIRE(c1 == m1);
    
    REQUIRE_NOTHROW(c2 = mat.row(2));
    REQUIRE(c2 == m2);
    
    REQUIRE_NOTHROW(c3 = mat.row(3));
    REQUIRE(c3 == m3);
    
    REQUIRE_THROWS(c4 = mat.row(4));
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    matrix<double> mrand = matrix<double>::random_dense_matrix(M, N, 1000, -1000);
    
    matrix<double>* mrandrows = new matrix<double>[M]();
    for(size_t r=0; r < M; r++)
    {
        mrandrows[r] = mrand.row(r);
    }
    
    for(size_t r=0; r < mrand.rows(); r++)
    {
        for(size_t c=0; c < mrand.cols(); c++)
        {
            REQUIRE(mrandrows[r](0, c) == mrand(r, c));
        }
    }
    
    delete [] mrandrows;
    
}

TEST_CASE("col")
{
    int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    matrix<int> mat(4, 4, data);
    
    int cdat0[4] = {2, 10, 18, 26};
    int cdat1[4] = {4, 12, 20, 28};
    int cdat2[4] = {6, 14, 22, 30};
    int cdat3[4] = {8, 16, 24, 32};
    
    matrix<int> m0(4, 1, cdat0), m1(4, 1, cdat1), m2(4, 1, cdat2), m3(4, 1, cdat3);
    matrix<int> c0, c1, c2, c3, c4;
    
    REQUIRE_NOTHROW(c0 = mat.col(0));
    REQUIRE(c0 == m0);
    
    REQUIRE_NOTHROW(c1 = mat.col(1));
    REQUIRE(c1 == m1);
    
    REQUIRE_NOTHROW(c2 = mat.col(2));
    REQUIRE(c2 == m2);
    
    REQUIRE_NOTHROW(c3 = mat.col(3));
    REQUIRE(c3 == m3);
    
    REQUIRE_THROWS(c4 = mat.col(4));
    
    size_t M = S_RAND(100);
    size_t N = S_RAND(100);
    
    matrix<double> mrand = matrix<double>::random_dense_matrix(M, N, 1000, -1000);
    
    matrix<double>* mrandcols = new matrix<double>[N]();
    for(size_t c=0; c < N; c++)
    {
        mrandcols[c] = mrand.col(c);
    }
    
    for(size_t c=0; c < mrand.cols(); c++)
    {
        for(size_t r=0; r < mrand.rows(); r++)
        {
            REQUIRE(mrandcols[c](r, 0) == mrand(r, c));
        }
    }
    
    delete [] mrandcols;
}

TEST_CASE("transpose")
{
    int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    int tdata[16] = {2, 10, 18, 26, 4, 12, 20, 28, 6, 14, 22, 30, 8, 16, 24, 32};
    
    matrix<int> mat(4, 4, data);
    matrix<int> tmat(4, 4, tdata);
    
    REQUIRE(mat.transpose() == tmat);
}

TEST_CASE("swap rows")
{
    int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    matrix<int> mat(4, 4, data);
    
    for(size_t i=0; i < mat.rows(); i++)
    {
        for(size_t j=0; j < mat.rows(); j++)
        {
            matrix<int> cmat(mat);
            
            mat.swap_rows(i, j);
            if(i != j)
            {
                REQUIRE(cmat != mat);
            }
            
            mat.swap_rows(j, i);
            REQUIRE(cmat == mat);
        }
    }
    
}

TEST_CASE("swap cols")
{
    int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
    matrix<int> mat(4, 4, data);
    
    for(size_t i=0; i < mat.cols(); i++)
    {
        for(size_t j=0; j < mat.cols(); j++)
        {
            matrix<int> cmat(mat);
            
            mat.swap_cols(i, j);
            if(i != j)
            {
                REQUIRE(cmat != mat);
            }
            
            mat.swap_cols(j, i);
            REQUIRE(cmat == mat);
        }
    }
}

TEST_CASE("sub rows")
{
    int data[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    matrix<int> mat(5, 5, data);
    
    mat.sub_rows(2, 1, 2);
    
    matrix<int> subbed_row1 = mat.row(2);
    int chk_data[5] = {-1, 0, -5, 7, -9};
    
    for(size_t i=0; i < 5; i++)
    {
        REQUIRE(chk_data[i] == subbed_row1[i]);
    }
}

TEST_CASE("split rows")
{
    int data[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    matrix<int> mat(5, 5, data);
    
    REQUIRE_THROWS(mat.split_rows(5));
    REQUIRE_THROWS(mat.split_rows(6));
    
    std::pair<matrix<int>, matrix<int>> mp1;
    std::pair<matrix<int>, matrix<int>> mp2;
    
    int mp1_chk1[5] = {2, 5, 9, 11, 7};
    int mp1_chk2[20] = {5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    
    int mp2_chk1[15] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1};
    int mp2_chk2[10] = {11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    
    REQUIRE_NOTHROW(mp1 = mat.split_rows(0));
    REQUIRE(mp1.first.rows() + mp1.second.rows() == 5);
    REQUIRE(mp1.first.rows() == 1);
    REQUIRE(mp1.second.rows() == 4);
    REQUIRE(mp1.first.cols() == 5);
    REQUIRE(mp1.second.cols() == 5);
    for(size_t i=0; i < 5; i++)
    {
        REQUIRE(mp1.first[i] == mp1_chk1[i]);
    }
    for(size_t i=0; i < 20; i++)
    {
        REQUIRE(mp1.second[i] == mp1_chk2[i]);
    }
    
    
    REQUIRE_NOTHROW(mp2 = mat.split_rows(2));
    REQUIRE(mp2.first.rows() + mp2.second.rows() == 5);
    REQUIRE(mp2.first.rows() == 3);
    REQUIRE(mp2.second.rows() == 2);
    REQUIRE(mp2.first.cols() == 5);
    REQUIRE(mp2.second.cols() == 5);
    for(size_t i=0; i < 15; i++)
    {
        REQUIRE(mp2.first[i] == mp2_chk1[i]);
    }
    for(size_t i=0; i < 10; i++)
    {
        REQUIRE(mp2.second[i] == mp2_chk2[i]);
    }
}

TEST_CASE("split cols")
{
    int data[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    matrix<int> mat(5, 5, data);
    
    REQUIRE_THROWS(mat.split_cols(5));
    REQUIRE_THROWS(mat.split_cols(6));
    
    std::pair<matrix<int>, matrix<int>> mp1;
    std::pair<matrix<int>, matrix<int>> mp2;
    
    int mp1_chk1[10] = {2, 5, 5, 3, 9, 6, 11, -2, 7, 5};
    int mp1_chk2[15] = {9, 11, 7, 6, -2, 5, 7, 3, 1, 3, 1, 3, 1, 3, 4};
    REQUIRE_NOTHROW(mp1 = mat.split_cols(1));
    REQUIRE(mp1.first.cols() + mp1.second.cols() == 5);
    REQUIRE(mp1.first.cols() == 2);
    REQUIRE(mp1.second.cols() == 3);
    REQUIRE(mp1.first.rows() == 5);
    REQUIRE(mp1.second.rows() == 5);
    for(size_t i=0; i < 10; i++)
    {
        REQUIRE(mp1.first[i] == mp1_chk1[i]);
    }
    for(size_t i=0; i < 15; i++)
    {
        REQUIRE(mp1.second[i] == mp1_chk2[i]);
    }
    
    
    int mp2_chk1[20] = {2, 5, 9, 11, 5, 3, 6, -2, 9, 6, 7, 3, 11, -2, 3, 1, 7, 5, 1, 3};
    int mp2_chk2[5] = {7, 5, 1, 3, 4};
    REQUIRE_NOTHROW(mp2 = mat.split_cols(3));
    REQUIRE(mp2.first.cols() + mp2.second.cols() == 5);
    REQUIRE(mp2.first.cols() == 4);
    REQUIRE(mp2.second.cols() == 1);
    REQUIRE(mp2.first.rows() == 5);
    REQUIRE(mp2.second.rows() == 5);
    for(size_t i=0; i < 20; i++)
    {
        REQUIRE(mp2.first[i] == mp2_chk1[i]);
    }
    for(size_t i=0; i < 5; i++)
    {
        REQUIRE(mp2.second[i] == mp2_chk2[i]);
    }
    
}


TEST_CASE("is square")
{
    matrix<int> nonsq(100, 200);
    REQUIRE(!nonsq.is_square());
    
    matrix<int> sq(100, 100);
    REQUIRE(sq.is_square());
}












