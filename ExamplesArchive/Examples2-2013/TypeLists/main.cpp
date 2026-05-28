//Playing around with TypeLists 

#include <iostream>

template<bool, class X, class Y>
struct Selector
{
    typedef X Result;
};

template<class X, class Y>
struct Selector<false,X,Y>
{
    typedef Y Result;
};
struct NullType 
{
};

template<class H, class T>
struct TypeList 
{
    typedef H Head;
    typedef T Tail;
};

// list size
template<class T> struct ListSize ;
template<> struct ListSize<NullType>
{
    enum {Value =  0};
};
template<class H,class T>
struct ListSize <TypeList<H,T> >
{
    enum {Value =  1+ ListSize<T>::Value};
};

// TypeAt
template<class Tlist, int index> struct TypeAt;

template<class Head, class Tail, int index> 
struct TypeAt<TypeList<Head,Tail>,index>
{
    typedef typename TypeAt<Tail, index-1>::value value;
};
template<class Head, class Tail> 
struct TypeAt<TypeList<Head, Tail>, 0>
{
    typedef Head value;
};


// loop
template<unsigned N, class TList, void (*Func)(unsigned)>
struct Loop;

template<unsigned N, class H, class T,void (*Func)(unsigned)>
struct Loop<N,TypeList<H,T>,Func>
{
    typedef H Result;
    static inline void Execute()
    {
        (*Func)(N); //just for fun
        std::cout << " Result: " << Result(97) << std::endl; 
        Loop<N-1,T,Func>::Execute();
    }
};

template<class H, class T,void (*Func)(unsigned)>
struct Loop<0,TypeList<H,T>,Func>
{
    typedef H Result;
    static inline void Execute() { Result();}
};

template<unsigned N,void (*Func)(unsigned)>
struct Loop<N,NullType,Func>
{
    typedef NullType Result;
    static inline void Execute() {}
};


template<bool Pred,template<class,class> class Host, class T1, class T2>
static inline typename Selector<Pred,T1,T2>::Result foo(Host<T1,T2>& val) 
{ 
    return typename Selector<Pred,T1,T2>::Result(val);
}

template<class H, class T>
class  Host
{
    H h;
    T t;
public:
    Host(int a): h(a), t(a) {}
    operator H()  {return h;}
    operator T() {return t;}
};

void bar(unsigned N){std::cout << "Loop" << N;}
int main()
{
    Host<int,char> v(97);
    std::cout << foo<true>(v) << std::endl;
    std::cout << foo<false>(v) << std::endl;

    typedef TypeList<int ,TypeList<char, TypeList<double, NullType> > > MyList;
    Loop<ListSize<MyList>::Value,MyList,bar>::Execute();

    std::cout << TypeAt<MyList,1>::value (91) << std::endl;
    system("pause");
}