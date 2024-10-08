//
//  test_householder.cpp
//  Created by Ben Westcott on 9/28/24.
//

#include "../householder.h"
#include "../gram_schmidt.h"

using namespace transformation::house;

TEST_CASE("test house")
{
    double zero_tol = 1E-11;
    double reldist = 1E-11;
    
    std::cout << "test transformation::house::house (basic):";
    
    size_t M = S_RAND(100);
    matrix<double> x = matrix<double>::random_dense_matrix(M, 1, -1000, 1000);
    
    auto h = housevec(x, 0);
    
    matrix<double> P = matrix<double>::eye(M) - h.beta * outer_prod_1D(h.vec, h.vec);
    matrix<double> xe1 = inner_right_prod(P, x);

    double x2norm = std::sqrt(col_norm2sq(x, 0));

    double max_mag = std::max(x2norm, xe1(0, 0));
    double absdiff = std::abs(x2norm - xe1(0,0));
    
    std::cout << "\tnorm2 = " << x2norm;
    std::cout << "\terrmax = " << absdiff;
    
    std::cout << "\n";
    
    REQUIRE(absdiff < reldist * max_mag);
    
    for(size_t r=1; r < xe1.rows(); r++)
    {
        REQUIRE(std::abs(xe1(r, 0)) < zero_tol);
    }
    
    REQUIRE(absdiff < zero_tol);
}

TEST_CASE("QRfast basic")
{
    size_t cnt_tol = 0;
    double zero_tol =  1E-14;
    double errmax;
    size_t errcnt;
    
    std::cout << "test transformation::house::QRfast (basic):";
    
    // very basic test first
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    
    auto basic_chk = transformation::GS::QR(basic);
    basic = QRfast(basic);
    
    //matrix<double> basic_chk(3, 3, {6, -3, -8, 1, 5.65685, -3.53553, 1, 2.41421, 0.707107});
    basic.fill_lower_triangle(0.0);
    
    errcnt = matrix<double>::abs_max_excess_err(basic, basic_chk.Y, zero_tol);
    errmax = matrix<double>::abs_max_err(basic, basic_chk.Y);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QRaccumulate basic, square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-14;
    double errmax;
    size_t errcnt;
    
    std::cout << "test transformation::house::QRaccumulate (basic, square):";
    
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    matrix<double> basic_cpy(basic);
    
    basic = QRfast(basic);
    
    //print_matrix(&basic);
    //std::cout << "\n\n";
    
    matrix<double> Qb = QRaccumulate(basic, basic.rows(), 0);
    
    //print_matrix(&Qb);
    //std::cout << "\n\n";
    basic.fill_lower_triangle(0.0);
    matrix<double> chk = mat_mul_alg1(&Qb, &basic);
    
    errcnt = matrix<double>::abs_max_excess_err(chk, basic_cpy, zero_tol);
    errmax = matrix<double>::abs_max_err(chk, basic_cpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
    
    //print_matrix(&chk);
    //std::cout << "\n\n";
}

TEST_CASE("QRaccumulate rand, randsize, square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    size_t M = GENERATE(take(20, random(1, 100)));
    
    std::cout << "test transformation::house::QRaccumulate (rand, M = " << M << ", N = " << M << ", square): ";
    
    matrix<double> randm = matrix<double>::random_dense_matrix(20, 20, -1000, 1000);
    matrix<double> randmcpy(randm);
    
    randm = QRfast(randm);
    
    matrix<double> Qrand = QRaccumulate(randm, randm.rows(), 0);

    randm.fill_lower_triangle(0.0);
    matrix<double> rchk = mat_mul_alg1(&Qrand, &randm);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, randmcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, randmcpy);

    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
    
}

TEST_CASE("QRaccumulate rand, randsize")
{
    
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    //const size_t M = 100;
    //auto N = GENERATE(1, 100, take(20, filter([](size_t i) { return i <= 100; }, random(1, 100))));
    auto S = GENERATE(take(1, rd_randmatsize(1, 100)));
    size_t M = S.M;
    size_t N = S.N;
    
    std::cout << "test transformation::house::QRaccumulate (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> tst = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    
    matrix<double> tstcpy(tst);
    tst = QRfast(tst);
    
    matrix<double> Qrand = QRaccumulate(tst, tst.rows(), 0);
    
    tst.fill_lower_triangle(0.0);
    matrix<double> rchk = mat_mul_alg1(&Qrand, &tst);
    
    //print_matrix(&rchk);
    //std::cout << "\n\n";
    
    //print_matrix(&tstcpy);
    
    //std::cout << "\n\n";
    
    //print_matrix(&Qrand);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, tstcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, tstcpy);
    
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QR")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    //const size_t N = 100;
    //auto M = GENERATE(100, take(10, filter([](size_t i) { return i >= 100; }, random(100, 200))));
    auto S = GENERATE(take(10, rd_randmatsize(1, 100)));
    size_t M = S.M;
    size_t N = S.N;
    
    
    std::cout << "test transformation::house::QR (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> qrtst = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    
    auto qrresult = QR(qrtst);
    
    matrix<double> rchk = mat_mul_alg1(&qrresult.Q, &qrresult.Y);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, qrtst, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, qrtst);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QRHfast")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    // TODO: fix 1x1 case
    
    size_t M = GENERATE(2, take(20, random(2, 100)));
    
    std::cout << "test transformation::house::QRHfast (rand, M = " << M << ", N = " << M<< "): ";
    
    matrix<double> htest = matrix<double>::random_dense_matrix(M, M, -1000, 1000);
    matrix<double> htestcpy(htest);
    //std::cout << "\n\n";
    //print_matrix(&htest);
    
    //std::cout << "\n\n";
    
    htest = QRHfast(htest);
    
    //print_matrix(&htest);
    
    matrix<double> Qh = QRaccumulate(htest, htest.rows(), 1);
    
    //std::cout << "\n\n";
    
    htest.fill_lower_hessenberg(0.0);
    
    //print_matrix(&htest);
    
    //std::cout << "\n\n";
    
    //print_matrix(&Qh);
    
    matrix<double> QhT = Qh.transpose();
    matrix<double> hchk = mat_mul_alg1(&Qh, &htest);
    matrix<double> hchk2 = mat_mul_alg1(&hchk, &QhT);
    
    //matrix<double> Ih = mat_mul_alg1(&QhT, &Qh);
    //std::cout << "\n\n";
    //print_matrix(&Ih);
    
    //std::cout << "\n\n";
    //print_matrix(&hchk2);
    
    //std::cout <<"\n\n";
    
    errcnt = matrix<double>::abs_max_excess_err(hchk2, htestcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(hchk2, htestcpy);
    
    //matrix<double> adiff = matrix<double>::absdiff(hchk2, htestcpy);
    
    //std::cout << "\n\n";
    //print_matrix(&adiff);
    //std::cout << "\n\n";
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
 
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);

}

TEST_CASE("QRH")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    // TODO: fix 1x1 case
    
    size_t M = GENERATE(2, take(10, random(2, 100)));
    
    std::cout << "test transformation::house::QRH (rand, M = " << M << ", N = " << M<< "): ";
    
    matrix<double> qrhtest = matrix<double>::random_dense_matrix(M, M, -1000, 1000);
    
    auto result = QRH(qrhtest);
    
    matrix<double> QT = result.Q.transpose();
    matrix<double> hchk = mat_mul_alg1(&result.Q, &result.H);
    matrix<double> hchk2 = mat_mul_alg1(&hchk, &QT);
    
    errcnt = matrix<double>::abs_max_excess_err(hchk2, qrhtest, zero_tol);
    errmax = matrix<double>::abs_max_err(hchk2, qrhtest);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
 
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("colpiv QRfast basic")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    std::cout << "test transformation::house::colpiv_QRfast (basic): ";
    
    matrix<double> basic(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    matrix<double> basiccpy(basic);
    
    auto result = colpiv_QRfast(basic);
    
    matrix<double> Q = QRaccumulate(result.F, result.F.rows(), 0);
    
    result.F.fill_lower_triangle(0.0);
    matrix<double> bchk = mat_mul_alg1(&Q, &result.F);
    
    bchk.permute_cols(result.P);
    
    errcnt = matrix<double>::abs_max_excess_err(bchk, basiccpy, zero_tol);
    errmax = matrix<double>::abs_max_err(bchk, basiccpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("colpiv householder square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    size_t M = GENERATE(2, take(10, random(2, 100)));
    //size_t M = 10;
    
    std::cout << "test transformation::house::colpiv_QRfast (rand, M = " << M << ", N = " << M<< "): ";
    
    matrix<double> mrand = matrix<double>::random_dense_matrix(M, M, -1000, 1000);
    matrix<double> mrandcpy(mrand);
    
    //std::cout << "\n\n";
    //print_matrix(&mrandcpy);
    //std::cout << "\n\n";
    
    auto result = colpiv_QRfast(mrand);
    
    matrix<double> Q = QRaccumulate(result.F, result.F.rows(), 0);
    
    result.F.fill_lower_triangle(0.0);
    matrix<double> bchk = mat_mul_alg1(&Q, &result.F);

    
    bchk.permute_cols(result.P);
    
    
    //print_matrix(&bchk);
    //std::cout << "\n\n";
    
    //print_matrix(&result.P);
    //std::cout << "\n\n";
    
    errcnt = matrix<double>::abs_max_excess_err(bchk, mrandcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(bchk, mrandcpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("colpiv householder maybe square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    auto S = GENERATE(take(10, rd_randmatsize(1, 100)));
    size_t M = S.M;
    size_t N = S.N;
    
    std::cout << "test transformation::house::colpiv_QRfast (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> mrand = matrix<double>::random_dense_matrix(M, M, -1000, 1000);
    matrix<double> mrandcpy(mrand);
    
    auto result = colpiv_QRfast(mrand);
    
    matrix<double> Q = QRaccumulate(result.F, result.F.rows(), 0);
    
    result.F.fill_lower_triangle(0.0);
    matrix<double> rchk = mat_mul_alg1(&Q, &result.F);
    
    rchk.permute_cols(result.P);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, mrandcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, mrandcpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QL householder square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    auto M = GENERATE(2, 100, take(10, random(2, 100)));
    auto N = M;
    
    std::cout << "test transformation::house::QL (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> b = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    matrix<double> bcpy(b);
    
    auto result = QL(b);
    
    matrix<double> chk = mat_mul_alg1(&result.Q, &result.Y);
    
    errcnt = matrix<double>::abs_max_excess_err(chk, bcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(chk, bcpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QL householder maybe square")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    auto S = GENERATE(take(10, rd_randmatsize(1, 100)));
    size_t M = S.M;
    size_t N = S.N;
    
    std::cout << "test transformation::house::QL (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> b = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    matrix<double> bcpy(b);
    
    auto result = QL(b);
    
    matrix<double> chk = mat_mul_alg1(&result.Q, &result.Y);
    
    errcnt = matrix<double>::abs_max_excess_err(chk, bcpy, zero_tol);
    errmax = matrix<double>::abs_max_err(chk, bcpy);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

TEST_CASE("QLH householder")
{
    size_t cnt_tol = 0;
    double zero_tol = 1E-11;
    double errmax;
    size_t errcnt;
    
    auto S = GENERATE(2, take(10, random(2, 100)));
    size_t M = S;
    size_t N = S;
    
    std::cout << "test transformation::house::QLH (rand, M = " << M << ", N = " << N << "): ";
    
    matrix<double> b = matrix<double>::random_dense_matrix(M, N, -1000, 1000);
    
    auto result = QLH(b);
    
    matrix<double> QT = result.Q.transpose();
    
    matrix<double> chk = mat_mul_alg1(&result.Q, &result.H);
    matrix<double> rchk = mat_mul_alg1(&chk, &QT);
    
    errcnt = matrix<double>::abs_max_excess_err(rchk, b, zero_tol);
    errmax = matrix<double>::abs_max_err(rchk, b);
    
    std::cout << "\terrcnt = " << errcnt;
    std::cout << "\terrmax = " << errmax;
    std::cout << "\n";
    
    REQUIRE(errcnt <= cnt_tol);
    REQUIRE(errmax < zero_tol);
}

/*
TEST_CASE("test")
{
    matrix<double> b(5, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8, 2, -1, 2, 6, 3, -4});
    
    matrix<double> bcpy(b);
    
    std::cout << b << "\n";
    
    b = QLfast(b);
    
    std::cout << b << "\n";
    
    matrix<double> Q = QLaccumulate(b);
    
    for(int64_t c = (int64_t)b.cols() - 1; c >= 0; c--)
    {
        for(int64_t r = c + 1; r >= 0; r--)
        {
            b(r, c) = 0.0;
        }
    }
    
    std::cout << b << "\n";
    
    std::cout << Q << "\n";
    
    matrix<double> chk = mat_mul_alg1(&Q, &b);
    
    std::cout << chk << "\n";
    
    matrix<double> QT = Q.transpose();
    matrix<double> othchk = mat_mul_alg1(&Q, &QT);
    
    std::cout << othchk << "\n";
    
    matrix<double> c(3, 3, {2, -1, -2, -4, 6, 3, -4, -2, 8});
    matrix<double> ccpy(c);
    
    c = QLfast(c);
    
    matrix<double> Qc = QLaccumulate(c);
    
    c.fill_upper_triangle(0.0);
    
    std::cout << Qc << "\n";
    
    std::cout << c << "\n";
    
    matrix<double> cchk = mat_mul_alg1(&Qc, &c);
    
    std::cout << cchk << "\n";
    
    matrix<double> d
    (
     10, 4,
     {
         2, -1, -2, 3,
         -4, 6, 3, 6,
         -4, -2, 8, 9,
         2, -1, -2, 2,
         -4, 6, 3, 5,
         -4, -2, 8, 1,
         2, -1, -2, 9,
         -4, 6, 3, 8,
         -4, -2, 8, 3,
         2, -1, -2, 7
     });
    
    d = QLfast(d);
    
    matrix<double> Qd = QLaccumulate(d);
    
    int64_t exrows = 2;
    for(int64_t c = d.cols() - 1; c >= 0; c--)
    {
        for(int64_t r = d.rows() - exrows; r >= 0; r--)
        {
            d(r, c) = 0.0;
        }
        exrows++;
    }
    
    std::cout << d << "\n";
    
    matrix<double> dchk = mat_mul_alg1(&Qd, &d);
    
    std::cout << dchk << "\n";
    
    
    

    
    
    //bcpy = QRfast(bcpy);
    
    //std::cout << bcpy << "\n";
}*/


