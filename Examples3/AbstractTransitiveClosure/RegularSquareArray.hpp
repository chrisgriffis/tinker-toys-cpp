#pragma once
#include "helper.h"

//first, build concept of square array<T,N>. make it regular.
//create semiring out of element type and two monoid operations

template<Regular T, size_t N>
struct RegularSquareArray
{
    typedef T UnderlyingType;
    RegularSquareArray();
    //copy ctr
    RegularSquareArray(const RegularSquareArray& r);
    RegularSquareArray(T (&inarr)[N][N]);
    //special ctr(int)
    RegularSquareArray(int x); // additive identity and multiplicative identity;
    //copy assign
    RegularSquareArray& operator=(const RegularSquareArray& rhs);

    T arr[N][N];
};

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray( T (&inarr)[N][N] )
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            arr[i][j]=inarr[i][j];
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>& RegularSquareArray<T, N>::operator=( const RegularSquareArray<T, N>& rhs ) 
{
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            arr[i][j]=rhs.arr[i][j];
        }
    }
    return *this;
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray( int x ) // additive identity and multiplicative identity 
{
    //fix this logic does not take size_to account that input arg can be values other than 0 or 1
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            if (x == 1 && i == j)arr[i][j]=T(1);
            else arr[i][j]=T(0);
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray( const RegularSquareArray<T, N>& r ) 
{
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            arr[i][j] = r.arr[i][j];
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray() 
{
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            arr[i][j]=T();
        }
    }
}
