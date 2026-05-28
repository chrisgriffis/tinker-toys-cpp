#pragma once
#include "helper.h"
#include "WrappedBool.h"

template<typename T>
struct Or;

template<>
struct Or<bool>
{
    typedef bool UnderlyingType;
    bool operator()(const bool& lhs, const bool& rhs){return (lhs || rhs);}
};

template<>
struct Or<WrappedBool>
{
    typedef WrappedBool UnderlyingType;
    WrappedBool operator()(const WrappedBool& lhs, const WrappedBool& rhs)
    {
        WrappedBool w;
        w.val = lhs.val || rhs.val;
        return w;
    }
};


