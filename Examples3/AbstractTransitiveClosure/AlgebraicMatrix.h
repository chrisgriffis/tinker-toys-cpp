#pragma once
#include "helper.h"
#include "RegularSquareArray.hpp"
#include "Monoid.hpp"
#include "Identity.h"
#include "Mult.h"

template<Regular T, size_t N>
struct AlgebraicMatrix : 
    public RegularSquareArray<T,N>, 
    public Monoid<
        RegularSquareArray<T,N>, 
        Mult<RegularSquareArray<T,N> >
        >,
    public ImbueIdentity<AlgebraicMatrix<T,N> >
{
public:
    typedef T UnderlyingType;

    AlgebraicMatrix(const AlgebraicMatrix&);
    AlgebraicMatrix& operator=(const AlgebraicMatrix&);
    AlgebraicMatrix Identity();

    AlgebraicMatrix(void);
    ~AlgebraicMatrix(void);
};

template<Regular T, size_t N>
AlgebraicMatrix<T,N>& AlgebraicMatrix<T, N>::operator=( const AlgebraicMatrix& a)
{
    RegularSquareArray::operator=(static_cast<RegularSquareArray>(a)); 
    return *this;
}

template<Regular T, size_t N>
AlgebraicMatrix<T, N>::AlgebraicMatrix( const AlgebraicMatrix<T, N>& a)
{
    RegularSquareArray<T, N>::operator =(static_cast<RegularSquareArray<T, N> >(a));
}

template<Regular T, size_t N>
AlgebraicMatrix<T, N>::AlgebraicMatrix()
{

}

template<Regular T, size_t N>
AlgebraicMatrix<T,N> AlgebraicMatrix<T, N>::Identity()
{
    AlgebraicMatrix<T,N> result(0);

    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            if (i == j)result.arr[i][j]=T(1);
            else result.arr[i][j]=T(0);
        }
    }
    return result;
}

