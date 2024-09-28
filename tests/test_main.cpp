#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <iostream>
#include "../matrix.h"

//#define PY_SSIZE_T_CLEAN
//#include <Python.h>

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

int* generate_random_ints(size_t nbr, size_t modulus)
{
    int *res = new int[nbr]();
    
    for(size_t i=0; i < nbr; i++)
    {
        res[i] = S_RAND(modulus);
    }
    
    return res;
}

/*
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

class pyrunlistener : public Catch::EventListenerBase {
public:
    using Catch::EventListenerBase::EventListenerBase;
    
    
    void testRunStarting(const Catch::TestRunInfo &info) override
    {
        Py_Initialize();
        PyRun_SimpleString("from time import time,ctime\nprint('Today is', ctime(time()))\n");
    }
    
    void testRunEnded(const Catch::TestRunStats &info) override
    {
        Py_FinalizeEx();
    }
};
 */

//CATCH_REGISTER_LISTENER(pyrunlistener)

#include "test_mat.cpp"
#include "test_householder.cpp"
//#include "test_prods.cpp"
//#include "test_gram_schmidt.cpp"


