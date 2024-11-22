//
//  test_jacobi.cpp
//  Created by Ben Westcott on 9/8/24.
//

TEST_CASE("jacobi")
{
    using namespace transformation;

    matrix<double> A = 
    {
        {2, 5, 9, 11, 7}, 
        {5, 3, 6, -2, 5}, 
        {9, 6, 7, 3, 1}, 
        {11, -2, 3, 1, 3},
        {7, 5, 1, 3, 4}
    };

    /*
            1.67e-01	3.71e+01	2.59e+03	1.48e+00	

            7.93e-01	-1.79e-01	2.92e-02	-5.82e-01	
            4.52e-01	7.42e-01	-3.29e-01	3.71e-01	
            3.22e-01	-1.00e-01	7.91e-01	5.10e-01	
            2.52e-01	-6.38e-01	-5.15e-01	5.14e-01
    */

    matrix<double> B = 
    {
        {4, -30, 60, -35},
        {-30, 300, -675, 420},
        {60, -675, 1620, -1050},
        {-35, 420, -1050, 700}
    };

    eigen_result e = jacobi::eigen(B);

    std::cout << e.values << "\n";
    std::cout << e.vectors << "\n";

}
