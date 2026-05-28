#include <iostream>

//Concepts
#define Regular class //behaves like a "regular" variable; i.e. assignable, copyable, default constructable 
#define Integer int // exactly that, and only that 
#define SemiRingAddOp typename //associative commutative operator of the semiring 
#define SemiRingMultOp typename //associative distributive operator of the semiring 
#define SemigroupOperation typename 
#define MonoidOperation typename 
#define size_t unsigned
#define min(a,b) (((a) < (b)) ? (a) : (b))
bool odd(int a){return ((a)&1)!=0;}
int half(int a){return a/2;} 


//first, build concept of square array<T,N>. make it regular.
//create semiring out of element type and two monoid operations

template<Regular T, size_t N>
struct RegularSquareArray
{
    typedef T UnderlyingType;
    RegularSquareArray();
    //copy ctr
    RegularSquareArray(const RegularSquareArray& r);
    //special ctr(int)
    RegularSquareArray(int x); // additive identity and multiplicative identity;
    //copy assign
    RegularSquareArray& operator=(const RegularSquareArray& rhs);

    T arr[N][N];
};

struct WrappedBool //act like a bool but know about algebra
{
    WrappedBool();
    WrappedBool(bool);
    WrappedBool(int);
    WrappedBool(const WrappedBool&);
    WrappedBool operator+(const WrappedBool& rhs);
    WrappedBool operator*(const WrappedBool& rhs);
    WrappedBool& operator=(const WrappedBool&);
    WrappedBool& operator=(const bool&);
    operator bool() const;
    bool val;
};

struct TropicalDouble //act like a double but know about tropical algebra
{
    TropicalDouble();
    TropicalDouble(double);
    TropicalDouble operator+(const TropicalDouble& rhs);
    TropicalDouble operator*(const TropicalDouble& rhs);
    operator double() const;
    double val;
};


template<typename T>
struct And
{
    typedef T UnderlyingType;
    T operator()(const T& lhs, const T& rhs){return (lhs * rhs);}
    static const T Identity = T(0);
    static const T Unity = T(1);
};


template<Regular T, size_t N> 
struct And<RegularSquareArray<T,N> > 
{
    typedef RegularSquareArray<T,N> UnderlyingType;
    And():Identity(0),Unity(1){}
    RegularSquareArray<T,N> operator()(const RegularSquareArray<T,N>& lhs, const RegularSquareArray<T,N>& rhs);
    const RegularSquareArray<T,N> Identity; 
    const RegularSquareArray<T,N> Unity; 
};

template<Regular T, template<Regular> class Op> 
T identity_element(Op<T>) {
    //i'm cheating here; i know things about the monoid operation. how do i know i want unity and not zero?
    return typename Op<T>::UnderlyingType(1);
}


template<class T,size_t N>
void printSqArr(T (&)[N][N]);

//generic algorithm for efficiently computing powers, A^N where A^N = Op(A,A^(N-1)); A^0 = multiplicative identity 
template< Regular A, /*Integer K,*/ SemigroupOperation Op> 
A power_accumulate_semigroup(A r, A a, Integer n, Op op) 
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

template< Regular A, /*Integer N,*/ SemigroupOperation Op> 
A power_semigroup(A a, Integer n, Op op) 
{
    while (!odd(n))
    {
        a = op(a,a);
        n = half(n);
    }
    if(n==1) return a;
    return power_accumulate_semigroup(a, op(a,a), half(n-1), op); 
}

template< Regular A, /*Integer N,*/ MonoidOperation Op> 
A power_monoid(A a, Integer n, Op op) 
{
    if (n == 0) return identity_element(op);
    return power_semigroup(a, n, op);
}

// template<typename T, typename Operation, typename AlgStruct>
// struct Unity
// {
//     static const T val;
// };
// 
// template<typename T, typename Operation, typename AlgStruct>
// struct Identity
// {
//     static const T val;
// };

//in main function,
int main()
{
    bool friendsArray[7][7] = 
    {
        {1,1,0,1,0,0,0},
        {1,1,0,0,0,1,0},
        {0,0,1,1,0,0,0},
        {1,0,1,1,0,1,0},
        {0,0,0,0,1,0,1},
        {0,1,0,1,0,1,0},
        {0,0,0,0,1,0,1}
    };
    RegularSquareArray<WrappedBool,7> friends;
    for (int i = 0; i < 7 ; i++)
    {
        for (int j = 0; j < 7 ; j++)
        {
            friends.arr[i][j]=friendsArray[i][j];
        }
    }

    printSqArr(friends.arr);
    RegularSquareArray<WrappedBool,7> X = 
        power_monoid(friends,11,And<RegularSquareArray<WrappedBool,7> >());
    printSqArr(X.arr);
    return 0;
}


template<Regular T, size_t N>
RegularSquareArray<T, N>& RegularSquareArray<T, N>::operator=( const RegularSquareArray<T, N>& rhs ) {
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
RegularSquareArray<T, N>::RegularSquareArray( const RegularSquareArray<T, N>& r ) {
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            arr[i][j] = r.arr[i][j];
        }
    }
}

template<Regular T, size_t N>
RegularSquareArray<T, N>::RegularSquareArray() {
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            arr[i][j]=T();
        }
    }
}

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

template<Regular T, size_t N>
RegularSquareArray<T,N> And<RegularSquareArray<T,N> >::operator()( const RegularSquareArray<T,N>& lhs, const RegularSquareArray<T,N>& rhs )
{
    RegularSquareArray<T,N> result(0);
    for (size_t i = 0; i < N ; i++)
    {
        for (size_t j = 0; j < N ; j++)
        {
            result.arr[i][j] = T(0);
            for (size_t k = 0; k < N ; k++)
            {
                result.arr[i][j] = result.arr[i][j].operator+(lhs.arr[i][k] * rhs.arr[k][j]);
            }
        }
    }
    return result;
}
WrappedBool WrappedBool::operator+( const WrappedBool& rhs )
{
    val = val || rhs.val;
    return *this;
}
WrappedBool WrappedBool::operator*( const WrappedBool& rhs )
{
    val = val && rhs.val;
    return *this;
}
WrappedBool::WrappedBool():val(){}
WrappedBool::WrappedBool( bool b):val(b){}
WrappedBool::WrappedBool( int i)
{
    val = ((i==0) ? false:true);
}

WrappedBool::WrappedBool( const WrappedBool& b):val(b.val){}
WrappedBool::operator bool() const{return val;}
WrappedBool& WrappedBool::operator=( const WrappedBool& sb)
{
    val = sb.val;
    return *this;
}

WrappedBool& WrappedBool::operator=( const bool& b)
{
    val = b;
    return *this;
}


TropicalDouble::TropicalDouble():val(0){}
TropicalDouble::TropicalDouble( double d):val(d){}
TropicalDouble TropicalDouble::operator+( const TropicalDouble& rhs )
{
    val = min(val,rhs.val);
    return *this;
}
TropicalDouble TropicalDouble::operator*( const TropicalDouble& rhs )
{
    val = val+rhs.val;
    return *this;
}
TropicalDouble::operator double() const{return val;}
