//
//  test_matrix.cpp
//  mmnn
//
//  Created by Ben Westcott on 9/8/24.
//


TEST_CASE("default constructor")
{
    matrix<int> mat = matrix<int>();
    
    REQUIRE(mat.rows() == 0);
    REQUIRE(mat.cols() == 0);
    REQUIRE(mat.size() == 0);
    REQUIRE(mat.base_ptr() == nullptr);
}

TEST_CASE("row, col constructor")
{
    size_t m = S_RAND(20);
    size_t n = S_RAND(20);
    
    matrix<int> mat = matrix<int>(m, n);
    
    REQUIRE(mat.rows() == m);
    REQUIRE(mat.cols() == n);
    REQUIRE(mat.size() == (m*n));
}

TEST_CASE("rows, cols, size")
{
	size_t m = S_RAND(20);
	size_t n = S_RAND(20);
	
	matrix<int> mat(m, n);
	
	REQUIRE(mat.rows() == m);
	REQUIRE(mat.cols() == n);
	REQUIRE(mat.size() == (m * n));
}

TEST_CASE("base ptr")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> mat(3, 3, data);
    int* mat_ptr = mat.base_ptr();
    
    for(size_t i=0; i < 9; i++)
    {
        REQUIRE(data[i] == mat_ptr[i]);
    }
    
}

TEST_CASE("rows, cols, data constructor")
{
    size_t m = 3;
    size_t n = 3;
    
    int *data = new int[(m * n)];
    
    for(int i=0; i < (m * n); i++)
    {
        data[i] = i;
    }
    
    matrix<int> mat = matrix<int>(m, n, data);
    int* mat_ptr = (int*)mat.base_ptr();
    
    for(int i=0; i < (m * n); i++)
    {
        CHECK(data[i] == mat_ptr[i]);
    }
}

TEST_CASE("copy constructor")
{
    matrix<int> original = matrix<int>::random_matrix(5, 5, -1000.0, 1000.0);
    
    matrix<int> cpy(original);
    
    REQUIRE(original.rows() == cpy.rows());
    REQUIRE(original.cols() == cpy.cols());
    REQUIRE(original.size() == cpy.size());
    
    int* optr = (int*)original.base_ptr();
    int* cptr = (int*)cpy.base_ptr();
    
    for(size_t i=0; i < original.size(); i++)
    {
        REQUIRE(optr[i] == cptr[i]);
    }
}

TEST_CASE("move constructor")
{
    
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> original(3, 3, data);
    
    size_t orows = original.rows();
    size_t ocols = original.cols();
    size_t osize = original.size();
    
    matrix<int> mv = std::move(original);
    
    REQUIRE(mv.rows() == orows);
    REQUIRE(mv.cols() == ocols);
    REQUIRE(mv.size() == osize);
    
    int* mv_ptr = (int*)mv.base_ptr();
    for(size_t i=0; i < mv.size(); i++)
    {
        REQUIRE(data[i] == mv_ptr[i]);
    }
    
}

TEST_CASE("get_value, rows, cols")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> mat(3, 3, data);
    
    for(size_t r=0; r < mat.rows(); r++)
    {
        for(size_t c=0; c < mat.cols(); c++)
        {
            int chk = data[r*3 + c];
            REQUIRE(chk == mat.get_value(r, c));
        }
    }
}

TEST_CASE("get_value, offs")
{
    
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> mat(3, 3, data);
    
    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(data[i] == mat.get_value(i));
    }
}

TEST_CASE("operator []")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    matrix<int> mat(3, 3, data);
    
    for(size_t i=0; i < mat.size(); i++)
    {
        REQUIRE(data[i] == mat[i]);
    }
}

TEST_CASE("set_value, row, col")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int set_data[9] = {2, 4, 6, 8, 10, 12, 14, 18, 20};
    
    matrix<int> mat(3, 3, data);
	
	for(size_t r=0; r < mat.rows(); r++)
	{
		for(size_t c=0; c < mat.cols(); c++)
		{
			mat.set_value(r, c, set_data[r*3+c]);
			REQUIRE(set_data[r*3+c] == mat.get_value(r, c));
		}
	}    
}

TEST_CASE("set_value, offs")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int set_data[9] = {2, 4, 6, 8, 10, 12, 14, 18, 20};
	
    matrix<int> mat(3, 3, data);
	
	for(size_t i=0; i < mat.size(); i++)
	{
		mat.set_value(i, set_data[i]);
		REQUIRE(set_data[i] == mat.get_value(i));
	}  
}

TEST_CASE("fill")
{
	matrix<int> mat(10, 10);
	mat.fill(100);
	
	for(size_t i=0; i < mat.size(); i++)
	{
		REQUIRE(mat[i] == 100);
	}
    
}

TEST_CASE("zero")
{
	matrix<int> mat(10, 10);
	mat.zero();
	
	for(size_t i=0; i < mat.size(); i++)
	{
		REQUIRE(mat[i] == 0);
	}
    
}

TEST_CASE("ones")
{
	matrix<int> mat(10, 10);
	mat.ones();
	
	for(size_t i=0; i < mat.size(); i++)
	{
		REQUIRE(mat[i] == 1);
	}
    
}

TEST_CASE("set_identity, identity")
{
	matrix<int> mat(5, 5);
	mat.set_identity();
	
	matrix<int> mat2 = matrix<int>::identity(5);
	
	for(size_t r=0; r < mat.rows(); r++)
	{
		for(size_t c=0; c < mat.cols(); c++)
		{
			if(r == c)
			{
				REQUIRE(mat.get_value(r, c) == 1);
				REQUIRE(mat2.get_value(r, c) == 1);
			}
			else
			{
				REQUIRE(mat.get_value(r, c) == 0);
				REQUIRE(mat2.get_value(r, c) == 0);
			}
		}
	}
    
}

TEST_CASE("unit permutation matrix")
{
	matrix<size_t> upm = matrix<size_t>::unit_permutation_matrix(10);
	
	for(size_t i=0; i < upm.size(); i++)
	{
		REQUIRE(upm[i] == i);
	}
    
}

TEST_CASE("content equals")
{
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


TEST_CASE("equals, operator==, operator!=")
{
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

TEST_CASE("operator= copy")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int data2[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int data3[9] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
	
	matrix<int> m1(3, 3, data);
	matrix<int> m2(4, 4, data2);
	matrix<int> m3(3, 2, data3);
	matrix<int> m4(3, 3);
	matrix<int> m5(2, 3);
	
	m4 = m1;
	REQUIRE(m4 == m1);
	
	m4 = m2;
	REQUIRE(m4 == m2);
	
	m5 = m3;
	REQUIRE(m5 != m3);
	REQUIRE(m5.content_equals(m3));
}

TEST_CASE("operator= move")
{
    int data[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int data2[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int data3[9] = {3, 6, 9, 12, 15, 18, 21, 24, 27};
	
	matrix<int> m1(3, 3, data);
	matrix<int> m1cpy(m1);
	
	matrix<int> m2(4, 4, data2);
	matrix<int> m2cpy(m2);
	
	matrix<int> m3(3, 2, data3);
	matrix<int> m3cpy(m3);
	
	matrix<int> m4(3, 3);
	matrix<int> m5(2, 3);
	
	m4 = std::move(m1);
	REQUIRE(m4 == m1cpy);
	
	m4 = std::move(m2);
	REQUIRE(m4 == m2cpy);
	
	m5 = std::move(m3);
	REQUIRE(m5 == m3cpy);    
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
	
	matrix<int>* mp1;
	matrix<int>* mp2;
	int mp1_chk1[5] = {2, 5, 9, 11, 7};
	int mp1_chk2[20] = {5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
	
	int mp2_chk1[15] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1};
	int mp2_chk2[10] = {11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
	
	REQUIRE_NOTHROW(mp1 = mat.split_rows(0));
	REQUIRE(mp1[0].rows() + mp1[1].rows() == 5);
	REQUIRE(mp1[0].rows() == 1);
	REQUIRE(mp1[1].rows() == 4);
	REQUIRE(mp1[0].cols() == 5);
	REQUIRE(mp1[1].cols() == 5);
	for(size_t i=0; i < 5; i++)
	{
		REQUIRE(mp1[0][i] == mp1_chk1[i]);
	}
	for(size_t i=0; i < 20; i++)
	{
		REQUIRE(mp1[1][i] == mp1_chk2[i]);
	}
	
	
	REQUIRE_NOTHROW(mp2 = mat.split_rows(2));
	REQUIRE(mp2[0].rows() + mp2[1].rows() == 5);
	REQUIRE(mp2[0].rows() == 3);
	REQUIRE(mp2[1].rows() == 2);
	REQUIRE(mp2[0].cols() == 5);
	REQUIRE(mp2[1].cols() == 5);
	for(size_t i=0; i < 15; i++)
	{
		REQUIRE(mp2[0][i] == mp2_chk1[i]);
	}
	for(size_t i=0; i < 10; i++)
	{
		REQUIRE(mp2[1][i] == mp2_chk2[i]);
	}
}

TEST_CASE("split cols")
{
    int data[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
	matrix<int> mat(5, 5, data);
	
    REQUIRE_THROWS(mat.split_cols(5));
	REQUIRE_THROWS(mat.split_cols(6));
	
	matrix<int>* mp1;
	matrix<int>* mp2;
	
	int mp1_chk1[10] = {2, 5, 5, 3, 9, 6, 11, -2, 7, 5};
	int mp1_chk2[15] = {9, 11, 7, 6, -2, 5, 7, 3, 1, 3, 1, 3, 1, 3, 4};
	REQUIRE_NOTHROW(mp1 = mat.split_cols(1));
	REQUIRE(mp1[0].cols() + mp1[1].cols() == 5);
	REQUIRE(mp1[0].cols() == 2);
	REQUIRE(mp1[1].cols() == 3);
	REQUIRE(mp1[0].rows() == 5);
	REQUIRE(mp1[1].rows() == 5);
	for(size_t i=0; i < 10; i++)
	{
		REQUIRE(mp1[0][i] == mp1_chk1[i]);
	}
	for(size_t i=0; i < 15; i++)
	{
		REQUIRE(mp1[1][i] == mp1_chk2[i]);
	}
	
	
	int mp2_chk1[20] = {2, 5, 9, 11, 5, 3, 6, -2, 9, 6, 7, 3, 11, -2, 3, 1, 7, 5, 1, 3};
	int mp2_chk2[5] = {7, 5, 1, 3, 4};
	REQUIRE_NOTHROW(mp2 = mat.split_cols(3));
	REQUIRE(mp2[0].cols() + mp2[1].cols() == 5);
	REQUIRE(mp2[0].cols() == 4);
	REQUIRE(mp2[1].cols() == 1);
	REQUIRE(mp2[0].rows() == 5);
	REQUIRE(mp2[1].rows() == 5);
	for(size_t i=0; i < 20; i++)
	{
		REQUIRE(mp2[0][i] == mp2_chk1[i]);
	}
	for(size_t i=0; i < 5; i++)
	{
		REQUIRE(mp2[1][i] == mp2_chk2[i]);
	}
	
}


TEST_CASE("is square")
{
    matrix<int> nonsq(100, 200);
	REQUIRE(!nonsq.is_square());
	
	matrix<int> sq(100, 100);
	REQUIRE(sq.is_square());
}

TEST_CASE("append vertical")
{
	int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int data2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	
    matrix<int> m1(4, 4, data);
	matrix<int> m2(4, 4, data2);
	matrix<int> m3(4, 3, data);
	matrix<int> m4(3, 4, data2);
	
	REQUIRE_THROWS(append_vertical(&m1, &m3));
	REQUIRE_THROWS(append_vertical(&m2, &m3));
	REQUIRE_THROWS(append_vertical(&m3, &m4));
	
	matrix<int> m1m2, m1m4, m2m4;
	
	REQUIRE_NOTHROW(m1m2 = append_vertical(&m1, &m2));
	REQUIRE(m1m2.cols() == m1.cols());
	REQUIRE(m1m2.cols() == m2.cols());
	REQUIRE(m1m2.rows() == m1.rows() + m2.rows());
	int dm1m2[32] = 
	{
		2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
	};
	for(size_t i=0; i < m1m2.size(); i++)
	{
		REQUIRE(dm1m2[i] == m1m2[i]);
	}
	
	REQUIRE_NOTHROW(m1m4 = append_vertical(&m1, &m4));
	REQUIRE(m1m4.cols() == m1.cols());
	REQUIRE(m1m4.cols() == m4.cols());
	REQUIRE(m1m4.rows() == m1.rows() + m4.rows());
	int dm1m4[28] = 
	{
		2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
	};
	for(size_t i=0; i < m1m4.size(); i++)
	{
		REQUIRE(dm1m4[i] == m1m4[i]);
	}
	
	
	REQUIRE_NOTHROW(m2m4 = append_vertical(&m2, &m4));
	REQUIRE(m2m4.cols() == m2.cols());
	REQUIRE(m2m4.cols() == m4.cols());
	REQUIRE(m2m4.rows() == m2.rows() + m4.rows());
	int dm2m4[28] =
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
	};
	for(size_t i=0; i < m2m4.size(); i++)
	{
		REQUIRE(dm2m4[i] == m2m4[i]);
	}

}

TEST_CASE("append horizontal")
{
	int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int data2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	
    matrix<int> m1(4, 4, data);
	matrix<int> m2(4, 4, data2);
	matrix<int> m3(4, 3, data);
	matrix<int> m4(3, 4, data2);
	
	REQUIRE_THROWS(append_horizontal(&m3, &m4));
	REQUIRE_THROWS(append_horizontal(&m2, &m4));
	REQUIRE_THROWS(append_horizontal(&m1, &m4));
	
	matrix<int> m1m2, m2m3, m1m3;
	
	REQUIRE_NOTHROW(m1m2 = append_horizontal(&m1, &m2));
	REQUIRE(m1m2.rows() == m1.rows());
	REQUIRE(m1m2.rows() == m2.rows());
	REQUIRE(m1m2.cols() == m1.cols() + m2.cols());
	int dm1m2[32] = 
	{
		2, 4, 6, 8, 1, 2, 3, 4, 10, 12, 14, 16, 5, 6, 7, 8, 
		18, 20, 22, 24, 9, 10, 11, 12, 26, 28, 30, 32, 13, 14, 15, 16 
	};
	
	for(size_t i=0; i < m1m2.size(); i++)
	{
		REQUIRE(dm1m2[i] == m1m2[i]);
	}
	
	REQUIRE_NOTHROW(m2m3 = append_horizontal(&m2, &m3));
	REQUIRE(m2m3.rows() == m2.rows());
	REQUIRE(m2m3.rows() == m3.rows());
	REQUIRE(m2m3.cols() == m2.cols() + m3.cols());
	int dm2m3[28] = 
	{
		1, 2, 3, 4, 2, 4, 6, 5, 6, 7, 8, 8, 10, 12, 
		9, 10, 11, 12, 14, 16, 18, 13, 14, 15, 16, 20, 22, 24
	};
	
	for(size_t i=0; i < m2m3.size(); i++)
	{
		REQUIRE(dm2m3[i] == m2m3[i]);
	}
	
	REQUIRE_NOTHROW(m1m3 = append_horizontal(&m1, &m3));
	REQUIRE(m1m3.rows() == m1.rows());
	REQUIRE(m1m3.rows() == m3.rows());
	REQUIRE(m1m3.cols() == m1.cols() + m3.cols());
	int dm1m3[28] =
	{
		2, 4, 6, 8, 2, 4, 6, 10, 12, 14, 16, 8, 10, 12, 18, 20, 22, 24, 
		14, 16, 18, 26, 28, 30, 32, 20, 22, 24
	};
	
	for(size_t i=0; i < m1m3.size(); i++)
	{
		REQUIRE(dm1m3[i] == m1m3[i]);
	}
}

TEST_CASE("outer product")
{
	int data[16] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32};
	int data2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	
	matrix<int> rvec1(1, 16, data);
	matrix<int> rvec2(1, 12, data);
	
	matrix<int> cvec1(16, 1, data2);
	matrix<int> cvec2(14, 1, data2);
	
	matrix<int> oprod1 = outer_prod_1D(&cvec1, &rvec1);
	REQUIRE(oprod1.rows() == cvec1.rows());
	REQUIRE(oprod1.cols() == rvec1.cols());
	REQUIRE(oprod1.size() == cvec1.rows() * rvec1.cols());
	
	matrix<int> oprod2 = outer_prod_1D(&cvec2, &rvec2);
	REQUIRE(oprod2.rows() == cvec2.rows());
	REQUIRE(oprod2.cols() == rvec2.cols());
	REQUIRE(oprod2.size() == cvec2.rows() * rvec2.cols());
}
