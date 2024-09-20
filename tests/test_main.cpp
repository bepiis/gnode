#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <iostream>
#include "../matrix.hpp"

#define S_RAND(N) ((size_t)(std::rand() % N))


template<class T>
void print_matrix(matrix<T>* mat)
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

#include "test_mat.cpp"


