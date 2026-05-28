#pragma once

template<
    typename T, 
    /*template<typename> */class MonoidAdd, 
    /*template<typename> */class MonoidMult
    >
struct SemiRing
{
    //a semiring has:
    //additive monoid AND multiplicative monoid TOGETHER WITH distribution (basically a ring without additive inverse)
    ////a monoid is a group without inverse

    //need an additive identity typedef and/or conversion
    //need a multiplicative identity typedef and/or conversion
    //T(1)

    typedef T DomainType;
 
    SemiRing& 
        operator+
        (const SemiRing& rhs)
    {
        //defined
        return MonoidAdd()(*this,rhs); 
//        return MonoidAdd<T>()(*this,rhs); 
    }

    SemiRing& 
        operator*
        (const SemiRing& rhs)
    {
        //defined 
        return MonoidMult()(*this,rhs); 
//        return MonoidMult<T>()(*this,rhs); 
    }
};
