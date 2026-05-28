#pragma once
#include "helper.h"

template<template <typename,unsigned> class T> struct AlgebraicMatrix;

template<typename T>
struct Mult;

template<Regular T, size_t N>
struct Mult<AlgebraicMatrix<T,N> >
{
    AlgebraicMatrix<T,N> operator()( const AlgebraicMatrix<T,N>& lhs, const AlgebraicMatrix<T,N>& rhs )
    {
        AlgebraicMatrix<T,N> result(0);
        for (size_t i = 0; i < N ; i++)
        {
            for (size_t j = 0; j < N ; j++)
            {
                result.arr[i][j] = T(0);
                for (size_t k = 0; k < N ; k++)
                {
                    result.arr[i][j] = result.arr[i][j] + //T add 
                        (lhs.arr[i][k] * //T mult
                        rhs.arr[k][j]);
                }
            }
        }
        return result;
    }
};

// template<>
// struct Mult<AlgebraicMatrix<AlgebraicBool,7> >
// {
//     AlgebraicMatrix<AlgebraicBool,7> operator()( const AlgebraicMatrix<AlgebraicBool,7>& lhs, const AlgebraicMatrix<AlgebraicBool,7>& rhs )
//     {
//         AlgebraicMatrix<AlgebraicBool,7> result(0);
//         for (size_t i = 0; i < 7 ; i++)
//         {
//             for (size_t j = 0; j < 7 ; j++)
//             {
//                 result.arr[i][j] =AlgebraicBool(0);
//                 for (size_t k = 0; k < 7 ; k++)
//                 {
//                     result.arr[i][j] = result.arr[i][j] + //AlgebraicBool add 
//                         (lhs.arr[i][k] * //AlgebraicBool mult
//                         rhs.arr[k][j]);
//                 }
//             }
//         }
//         return result;
//     }
// };
