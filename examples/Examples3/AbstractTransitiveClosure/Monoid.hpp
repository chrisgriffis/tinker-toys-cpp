#pragma once

template<
    typename T, 
    /*template<typename> */class MonoidOp
    >
struct Monoid
{
    //a weak semiring has:
    //additive monoid AND multiplicative monoid TOGETHER WITH distribution (basically a ring without additive inverse)
    ////a monoid is a group without inverse

    //need an additive identity typedef and/or conversion
    //need a multiplicative identity typedef and/or conversion
    //T(1)

    typedef T DomainType;

    Monoid& 
        operator*
        (const Monoid& rhs)
    {
        //defined 
//         return MonoidOp<T>()(*this,rhs); 
        return MonoidOp()(*this,rhs); 
    }
};
