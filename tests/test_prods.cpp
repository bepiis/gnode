//
//  test_prods.cpp
//  Created by Ben Westcott on 9/25/24.
//

// TODO: write these tests... maybe just 

TEST_CASE("outer product")
{
	std::cout << "test outer product:\n";
	
	size_t M = S_RAND(1000);
	size_t N = S_RAND(1000);
	
	matrix<int> cv_ones = matrix<int>::ones(M, 1);
	matrix<int> m_ones = matrix<int>::ones(M, M);
	
	matrix<int> rv_ones = matrix<int>::ones(1, N);
	
	matrix<int> oprod;
	
	REQUIRE_THROWS(oprod = outer_prod_1D(m_ones, rv_ones));
	REQUIRE_NOTHROW(oprod = outer_prod_1D(cv_ones, rv_ones));
	
	REQUIRE(oprod == matrix<int>::ones(M, N));
}


TEST_CASE("inner product ")
{
	std::cout << "test inner product:\n";
	
	size_t M = S_RAND(100);
	
	matrix<size_t> m1 = matrix<size_t>::unit_permutation_matrix(M);
	matrix<size_t> m2 = matrix<size_t>::unit_permutation_matrix(M);
	
	matrix<size_t> m1T = m1.transpose();
	matrix<size_t> m2T = m2.transpose();
	
	size_t N = S_RAND(100);
	if(M == N) N++;
	
	matrix<size_t> m3 = matrix<size_t>::unit_permutation_matrix(N);
	matrix<size_t> result;
	
	REQUIRE_THROWS(result = inner_prod_1D(m1, m3));
		
	
	
	
	
	
}

TEST_CASE("inner left product")
{
	std::cout << "test inner left product:\n";
	
}

TEST_CASE("inner right product")
{
	std::cout << "test inner right product:\n";
	
}

TEST_CASE("projection")
{
	std::cout << "test projection:\n";

}

TEST_CASE("column 2 norm")
{
	std::cout << "test column 2 norm:\n";
}

TEST_CASE("columns 2 norm")
{
	std::cout << "test columns 2 norm:\n";
}

TEST_CASE("mat mul alg1")
{
	std::cout << "test mat mul alg1:\n";
}
