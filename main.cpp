#include "matrix.hpp"
#include "tdpool.hpp"
#include "mat_mul.hpp"
#include "lu_decomp.h"
#include "householder.h"
#include <iostream>
#include <vector>
#include "pivot.h"
#include "jacobi.h"
#include "matrix_iterators.h"
#include "gram_schmidt.h"

//#define NS_PRIVATE_IMPLEMENTATION
//#define MTL_PRIVATE_IMPLEMENTATION

//#include "Metal/Metal.hpp"

/*
 * 1.0 2.0          5.0 6.0             1(5) 1(6)       2(7) 2(8)       19  22
 * 3.0 4.0    *     7.0 8.0    =        3(5) 3(6)   +   4(7) 4(8)  =    43  50
 *
 */

// https://math.nist.gov/MatrixMarket
// TODO: HASH???

template<class T>
void pprint_matrix(matrix<T>* mat)
{
    for(int i=0; i < mat->rows(); i++)
    {
        for(int j=0; j < mat->cols(); j++)
        {
            std::cout << mat->get_value(i, j) << "\t";
        }
        std::cout << "\n";
    }
}

int main()
{
    double jdata[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    
    double hh_data[9] = {2, -1, -2, -4, 6, 3, -4, -2, 8};
    
    matrix<double> HH_test(3, 3, hh_data);
    
    matrix<double> Aj(5, 5, jdata);
    pprint_matrix(&HH_test);
    
    matrix<double> hv = transformation::house::householder_vector(0, HH_test);
    std::cout<< "\n\n";
    
    //matrix<double> scol = Aj.sub_col(1, 4, 1);
    //pprint_matrix(&scol);
    
    matrix<double> HH_cpy(HH_test);
    auto res = transformation::house::householder(HH_test);
    
    pprint_matrix(&res.first);
    std::cout<< "\n\n";
    
    pprint_matrix(&res.second);
    
    std::cout << "\n\n";
    matrix<double> Q = transformation::house::Qaccumulate(res.first, res.second, res.first.cols());
    
    pprint_matrix(&Q);
    std::cout << "\n\n";
    
    matrix<double> randM = matrix<double>::random_matrix(10,10, 1000, -1000);
    
    auto hess = transformation::house::hessenberg(HH_cpy);
    
    pprint_matrix(&hess.first);
    
    transformation::house::Qaccumulate(hess.first, hess.second, hess.first.cols());
    
   // transformation::house::forward_accumulate(res.first, res.second);
    
    //res.first(1, 0) = 0;
    //res.first(2, 0) = 0;
    //res.first(2, 1) = 0;
    
    //pprint_matrix(&Q);
    std::cout << "\n\n";
    
    //matrix<double> chk = mat_mul_alg1(&Q, &res.first);
    
    //pprint_matrix(&chk);
    
    matrix<double> gs_test(3, 3, hh_data);
    
    std::pair<matrix<double>, matrix<double>> outgs = transformation::GS::QR(gs_test);
    
    pprint_matrix(&outgs.first);
    std::cout << "\n\n";
    pprint_matrix(&outgs.second);
    std::cout << "\n\n";
    //pprint_matrix(&hv);
}
    
/*
int main()
{
    double dat1[16];
    double dat2[16];
    
    //std::cout << std::thread::hardware_concurrency() << "\n";

    //NS::SharedPtr< MTL::Device > pDevice = NS::TransferPtr( MTL::CreateSystemDefaultDevice() );
    
    for(size_t i=0; i < 16; i++)
    {
        dat1[i] = 2*i;
        dat2[i] = 2*i + 1;
    }
    
    matrix<double> m1(4, 4, dat1);
    matrix<double> m2(4, 4, dat2);
    
    //pprint_matrix(&m1);
    //std::cout << "\n\n";
    //pprint_matrix(&m2);
    //std::cout << "\n\n";

    
        
    //matrix<double> m12 = mat_mul_alg1(&m1, &m2);
    
    //pprint_matrix(&m12);
    
    //std::cout << "\n\n";
    
    //m1.swap_rows(1, 3);
    
    double sdata[9] = {1.0, 4.0, 5.0, 6.0, 8.0, 22.0, 32.0, 5.0, 5.0};
    double bv[3] = {1.0, 2.0, 3.0};
    
    int ddata[9] = {1, 1, 1, 2, 2, 2, 3, 3, 3};
    matrix<int> ddm(3, 3, ddata);
    
    matrix<double> b(3, 1, bv);
    
    
    matrix<double> sm(3, 3, sdata);
    
    
    //std::cout << "\n" << find_row_swap(0, m1) << "\n";
    matrix<double> res = LU_solve(sm, b);
    
    double hdata[16] = {4, 1, -2, 2, 1, 2, 0, 1, -2, 0, 3, -2, 2, 1, -2, -1};
    matrix<double> htest(4, 4, hdata);
    
    
    //matrix<double> hv = householder_vector(0, htest);
    
    //pprint_matrix(&hv);
    //std::cout << "\n\n";
    
    //matrix<double> hm = householder_matrix(hv);
    
    //pprint_matrix(&hm);
    
    matrix<double> A0 = transformation::householder_iteration(0, htest);
    
    pprint_matrix(&A0);
    
    matrix<double> A1 = transformation::householder_iteration(1, A0);
    
    std::cout << "\n\n";
    for(size_t j=0; j < A1.size(); j++)
    {
        if(std::abs(A1[j]) < 0.001)
        {
            A1[j] = 0.0;
        }
    }
    
    pprint_matrix(&A1);
    
    
    double jdata[25] = {2, 5, 9, 11, 7, 5, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    //double jdatap[25] = {2, 4, 9, 10, 7, 8, 3, 6, -2, 5, 9, 6, 7, 3, 1, 11, -2, 3, 1, 3, 7, 5, 1, 3, 4};
    //double jdata2[16] = {4, -30, 60, -35, -30, 300, -675, 420, 60, -675, 1620, -1050, -35, 420, -1050, 700};
    matrix<double> Aj(5, 1, jdata);
    
    matrix<double> t(5, 1, jdata);
    
    pprint_matrix(&Aj);
    
    std::cout << "\n" << inner_prod_1D(Aj, t) << "\n";
    
    double imdata[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double ivdata[3] = {1, 2, 3};
    
    matrix<double> im(3, 3, imdata);
    matrix<double> iv(1, 3, ivdata);
    matrix<double> ivT = iv.transpose();
    
    matrix<double> iPleft = inner_left_prod(iv, im);
    matrix<double> iPright = inner_right_prod(im, ivT);
    
    pprint_matrix(&iPleft);
    std::cout << "\n";
    pprint_matrix(&iPright);
    
    std::cout << "\n\n";
    
    double sd[4] = {3, 2, 1, 2};
    matrix<double> s(2, 2, sd);
    //matrix<double> s = matrix<double>::random_matrix(5, 5, -10.0, 10.0);
    matrix<double> out = transformation::GS::MGS(s);
    
    pprint_matrix(&out);
    
    std::cout << "\n\n";
    
    std::pair<matrix<double>, matrix<double>> QR = transformation::GS::QR(sm);
    
    pprint_matrix(&QR.first);
    
    std::cout << "\n\n";
    
    pprint_matrix(&QR.second);
    //std::cout << inner_prod_1D(out.col(0), out.col(1));
    
    //transformation::eigen_result res2 = transformation::jacobi::eigen(Aj);
    
    //pprint_matrix(&res2.values);
    
    //transformation::jacobi::tridiagonalize(Aj, 0.001);
    
    //std::cout << "finished";
    
    //matrix<double> Qj = transformation::jacobi_rotation_matrix(Aj, 0, 1);
    
    //pprint_matrix(&Qj);
    //std::cout << "\n\n";
    
    //matrix<double> QjT = Qj.transpose();
    //matrix<double> QTA = mat_mul_alg1(&QjT, &Aj);
    //matrix<double> Ap = mat_mul_alg1(&QTA, &Qj);
    
    //pprint_matrix(&Ap);
    //Lop<int> L = Lop<int>(ddm);
    //Rop<int> R = Rop<int>(ddm);
    
    
    //std::vector<std::unique_ptr<operation<int>>> v;
    //v.emplace_back(std::make_unique<Lop<int>>(L));
    //v.emplace_back(std::make_unique<Rop<int>>(R));
    
   // L.add_multiple(1, 0, 4);
    //matrix<int>& perm = L.swap(0, 1);
    
    //matrix<int> res2 = mat_mul_alg1(&perm, &ddm);
    
    
    //pprint_matrix(&res2);
    
    //pprint_matrix(&P);

    
}*/

