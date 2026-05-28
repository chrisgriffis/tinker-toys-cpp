#pragma once
#include "helper.h"

bool odd(int a){return ((a)&1)!=0;}
int half(int a){return a/2;} 


template< SemigroupDomain A> 
A identity_element(A a) 
{
    return a.Identity();
}


//generic algorithm for efficiently computing powers, A^N where A^N = Op(A,A^(N-1)); A^0 = multiplicative identity 
template< SemigroupDomain A> 
A power_accumulate_semigroup(A r, A a, Integer n) 
{
    if (n==0)return r;
    while (true)
    {
        if (odd(n))
        {
            r = a * r; //overloaded to POSSIBLY NONCOMMUTING semigroup mult
            if(n == 1) return r;
        }
        n = half(n);
        a = a * a;
    }
}

template< SemigroupDomain A> 
A power_semigroup(A a, Integer n) 
{
    while (!odd(n))
    {
        a = a * a;
        n = half(n);
    }
    if(n==1) return a;
    return power_accumulate_semigroup(a, a * a, half(n-1)); 
}

template< SemigroupDomain A> 
A power_monoid(A a, Integer n) 
{
    if (n == 0) return identity_element(a);
    return power_semigroup(a, n);
}
