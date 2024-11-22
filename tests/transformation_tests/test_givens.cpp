//
//  test_givens.cpp
//  Created by Ben Westcott on 10/9/24.
//

#ifdef TEST_FULL_VERBOSE_OUTPUT
    #define TEST_GIVENS_VERBOSE_OUTPUT
#endif

TEST_CASE("QR householder")
{
    using namespace transformation::givens;

    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    auto S = GENERATE(take(10, rd_randmatsize(1, 100)));
    size_t M = S.M;
    size_t N = S.N;

#ifdef TEST_GIVENS_VERBOSE_OUTPUT
    std::cout << "test transformation::givens::QR (rand, M = " << M << ", N = " << N << "): ";
#endif
    
    matrix<double> b = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    matrix<double> bcpy(b);
    matrix<double> Q;

    uint64_t tR, tQ;
    
    //auto result = QR(b);
    {
        b = time_exec(tR, QRfast, b);
        Q = time_exec(tQ, QRaccumulate, b);
        b.fill_lower_triangle(0.0);

    }
    
    matrix<double> chk = mat_mul_alg1(&Q, &b, mult_pool);
    
    errcnt = matrix<double>::abs_max_excess_err(chk, bcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(chk, bcpy);
    
#ifdef TEST_GIVENS_VERBOSE_OUTPUT
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\ttR (ns) = " << tR;
    std::cout << "\ttQ (ns) = " << tQ;
    std::cout << "\n";
#endif
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}


/*
TEST_CASE("bruh")
{
    
    //matrix<double> basic(5, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8, 2, -1, -2, -4, 6, 3});
    //matrix<double> basic = {{2, -1, 2, 2}, {-4, 6, 3, 3}, {-4, -2, 8, 8}, {1, 2, 3, 4}};
    //matrix<double> basic = {{2, -1, 2}, {5, 6, 3}, {5, -1, 8}};
    //matrix<double> basic = {{2, 5, 2}, {-1, 5, 3}, {6, -1, 8}};
    
    matrix<double> basic = {{2, -1, 2, 2}, {-4, 6, 3, 3}, {-4, -2, 8, 8}, {-4, 2, 3, 4}};
    //matrix<double> basic = matrix<double>::random_dense_matrix(3, 3, -1000, 1000);
    std::cout << basic << "\n";
    //matrix<double> basic = {{-2, -1}, {-4, 6}};
    matrix<double> bcpy(basic);
    
    basic = QRfast(basic);
    
    auto out = transformation::GS::QR(bcpy);
    
    
    matrix<double> Q = QRaccumulate(basic);
    
    std::cout << out.Q << "\n";
    std::cout << Q << "\n";
    

    
    
    std::cout << basic << "\n";
    
    basic.fill_lower_triangle(0.0);
    
    matrix<double> QT = Q.transpose();
    auto res = mat_mul_alg1(&Q, &basic);
    
    std::cout << res << "\n";

}*/
