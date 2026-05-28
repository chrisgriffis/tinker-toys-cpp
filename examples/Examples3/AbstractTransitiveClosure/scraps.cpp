//Concepts
#define Regular class //behaves like a "regular" variable; i.e. assignable, copyable, default constructable
#define Integer int // exactly that, and only that
#define SemiRingAddOp typename //associative commutative operator of the semiring
#define SemiRingMultOp typename //associative distributive operator of the semiring
#define SemigroupOperation typename 
#define MonoidOperation typename 
#define size_t unsigned


struct invalid_type; 

//first, build concept of square array<T,N>. make it regular.
//create semiring out of element type and two monoid operations

template<Regular T, size_t N>
struct RegularSquareArray
{
    RegularSquareArray();
    //copy ctr
    RegularSquareArray(const RegularSquareArray& r);
    //special ctr(int)
    RegularSquareArray(int x) // additive identity and multiplicative identity;
        //copy assign
        RegularSquareArray& operator=(const RegularSquareArray& rhs);
    //comparison
    bool operator==(const RegularSquareArray& lhs,const RegularSquareArray& rhs);

    T arr[N][N];
};


//then build concept of a semiring of underlying type T with monoid add and monoid mult, semiring<T,Add<T>,Mult<T>>
template<class T, template<class> MonoidAdd, template<class> MonoidMult>
struct SemiRing
{
    //a semiring has:
    //additive monoid AND multiplicative monoid TOGETHER WITH distribution (basically a ring without additive inverse)
    ////a monoid is a group without inverse

    //need an additive identity typedef and/or conversion
    //need a multiplicative identity typedef and/or conversion
    //T(1)

    typedef MonoidAdd<T> MonoidAdd;
    typedef MonoidMult<T> MonoidMult;
    typedef T UnderlyingType;
    //     typedef SemiRing<T,MonoidAdd,MonoidMult> SemiRing;

    SemiRing<T,MonoidAdd,MonoidMult>& 
        operator+
        (const SemiRing<T,MonoidAdd,MonoidMult>& rhs)
    {
        //defined
        return MonoidAdd()(*this,rhs); 
    }

    SemiRing<T,MonoidAdd,MonoidMult>& 
        operator*
        (const SemiRing<T,MonoidAdd,MonoidMult>& rhs)
    {
        //defined 
        return MonoidMult()(*this,rhs); 
    }
};

template<typename T>
struct Or;
template<>
struct Or<bool>
{
    bool operator()(bool lhs, bool rhs){return (lhs || rhs);}
    static const bool Identity = false;
};
template<typename T, size_t N>
struct Or<RegularSquareArray<T,N> >
{
    T operator()(T lhs, T rhs)
    {
        typedef Or<T> Or;
        T result[N][N]; //leave unitialized until i figure out what to do about T()
        for (int i = 0; i < N ; i++)
        {
            for (int j = 0; j < N ; j++)
            {
                result[i][j] = Or()(lhs[i][j],rhs[i][j]);
            }
        }
    }
    static const T Identity = T::Identity;
};
template<typename T>
struct And;
template<>
struct And<bool>
{
    bool operator()(bool lhs, bool rhs){return (lhs && rhs);}
    static const bool Identity = true;
};
//TODO refactor to make fully generic
template<size_t N>
struct And<RegularSquareArray<bool,N> >
{
    typedef RegularSquareArray<bool,N> UnderlyingType;
    And()
    {
        for (int i = 0; i < N ; i++)
        {
            for (int j = 0; j < N ; j++)
            {
                if (i==j) Identity.arr[i][j]=true;
                else Identity.arr[i][j]=false;
            }
        }
    }
    RegularSquareArray<bool,N> operator()(RegularSquareArray<bool,N> lhs, RegularSquareArray<bool,N> rhs)
    {
        RegularSquareArray<bool,N> result; //leave unitialized until i figure out what to do about T()
        for (int i = 0; i < N ; i++)
        {
            for (int j = 0; j < N ; j++)
            {
                result.arr[i][j] = false;
                for (int k = 0; k < N ; k++)
                {
                    result.arr[i][j] = result.arr[i][j] || (lhs.arr[i][j] && rhs.arr[i][j]);
                }
            }
        }
    }
    const RegularSquareArray<bool,N> Identity;
};

//////////////////////////////////////////////////////////////////////////
template<size_t N> 
struct BooleanSquareArraySemiRing //star of the show
    : SemiRing< 
    RegularSquareArray<bool,N>, 
    Or<RegularSquareArray<bool,N> >, 
    And<RegularSquareArray<bool,N> > 
    > {};
//////////////////////////////////////////////////////////////////////////

bool odd(int a){return a&1!=0;}
int half(int a){if(odd(a)) throw; return a/2;} //monadic form, hehe

//generic algorithm for efficiently computing powers, A^N where A^N = Op(A,A^(N-1)); A^0 = multiplicative identity
template< Regular A, Integer N, SemigroupOperation Op>
A power_accumulate_semigroup(A r, A a, N n, Op op)
{
    if (n==0)return r;
    while (true)
    {
        if (odd(n))
        {
            r = op(r, a);
            if(n == 1) return r;
        }
        n = half(n);
        a = op(a, a);
    }
}

template< Regular A, Integer N, SemigroupOperation Op>
A power_semigroup(A a, N n, Op op)
{
    while (!odd(n))
    {
        a = op(a,a);
        n = half(n);
    }
    if(n==1) return a;
    return power_accumulate_semigroup(a, op(a,a), half(n-1), op);
}

template< Regular A, Integer N, MonoidOperation Op>
A power_monoid(A a, N n, Op op)
{
    if (n == 0) return identity_element(op);
    return power_semigroup(a, n, op);
}

//in main function, build a class booleansemiring<array<bool,5>,Op1<~>,Op2<~>>
int main()
{
    typedef SemiRing< RegularSquareArray<bool,8>, Or<bool>, And<bool> > BooleanSemiRing; //no dont do this, instead paramterize on N
    BooleanArraySemiRing<8> friendsMatrix;
    //"feels like" i need a while loop for a transitive closure
    {
        power_monoid(friendsMatrix,8,BooleanArraySemiRing::MonoidMult());
    }

    return 0;
}




template<Regular T, size_t N>
RegularSquareArray<T, N>& RegularSquareArray<T, N>::operator=( const RegularSquareArray<T, N>& rhs )
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            arr[i][j]=rhs.arr[i][j]
        }
    }
    return *this;
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray( int x ) // additive identity and multiplicative identity
{
    //fix this logic does not take into account that input arg can be values other than 0 or 1
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            if (x == 1 && i == j)arr[i][j]=T(1);
            else arr[i][j]=T(0);
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray( const RegularSquareArray<T, N>& r )
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            arr[i][j] = r.arr[i][j];
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray()
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            arr[i][j]=T();
        }
    }
}

template<Regular T, size_t N>
bool RegularSquareArray<T, N>::operator==( const RegularSquareArray<T, N>& lhs,const RegularSquareArray<T, N>& rhs )
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            if (lhs.arr[i][j] != rhs.arr[i][j]) return false;
        }
    }
    return true;
}
