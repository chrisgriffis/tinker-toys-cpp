#pragma once
#include "helper.h"
#include "WrappedBool.h"

template<typename T>
struct And;

template<>
struct And<bool>
{
    typedef bool UnderlyingType;
    bool operator()(const bool& lhs, const bool& rhs){return (lhs && rhs);}
};

template<>
struct And<WrappedBool>
{
    typedef WrappedBool UnderlyingType;
    WrappedBool operator()(const WrappedBool& lhs, const WrappedBool& rhs)
    {
        WrappedBool w;
        w.val = lhs.val && rhs.val;
        return w;
    }
};
