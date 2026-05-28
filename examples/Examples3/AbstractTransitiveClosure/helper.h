#pragma once
#include <iostream>

//Concepts
#define Regular class //behaves like a "regular" variable; i.e. assignable, copyable, default constructable 
#define Integer int // exactly that, and only that 
#define SemiRingAddOp typename //associative commutative operator of the semiring 
#define SemiRingMultOp typename //associative distributive operator of the semiring 
#define SemigroupDomain typename 
#define MonoidOperation typename 
#define size_t unsigned
#define min(a,b) (((a) < (b)) ? (a) : (b))

template<class T,size_t N>
void printSqArr( T (&arr)[N][N] )
{
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            std::cout << (arr[i][j] ? "1" : "0") << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}
