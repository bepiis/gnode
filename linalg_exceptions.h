//
//  exceptions.h
//  mmnn
//
//  Created by Ben Westcott on 9/12/24.
//

#ifndef LINALG_EXCEPTIONS_H
#define LINALG_EXCEPTIONS_H

#include <exception>

class non_square_matrix_error : public std::exception
{
    virtual const char* what() const throw() override
    {
        return "Expected square matrix.";
    }
};

class non_symmetric_matrix_error : public std::exception
{
    virtual const char* what() const throw() override
    {
        return "Expected symmetric matrix.";
    }
};

class non_distinct_indicies_error : public std::exception
{
    virtual const char* what() const throw() override
    {
        return "Expected non distinct indicies.";
    }
};


#endif /* exceptions_h */
