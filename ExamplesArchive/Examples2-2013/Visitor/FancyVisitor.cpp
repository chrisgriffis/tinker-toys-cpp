// Visitor.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;


class BaseVisitor
{
public:
    virtual ~BaseVisitor(){}
};

template<class T, typename R = void>
class Visitor
{
public:
	typedef R ReturnType;
    virtual ReturnType Visit(T&) = 0;
};

template<typename R = void>
class BaseVisitable
{
public:
    typedef R ReturnType;
    virtual ~BaseVisitable(){}
    virtual ReturnType Accept(BaseVisitor&) = 0;
protected:
    template<class T>
    static ReturnType AcceptImpl(T& visited, BaseVisitor& guest)
    {
    	if (Visitor<T, R>* p = dynamic_cast<Visitor<T, R>* >(&guest))
    	{
            p->Visit(visited);
    	}
        return ReturnType();
    }
};

#define DEFINE_VISITABLE() \
    virtual ReturnType Accept(BaseVisitor& guest) \
    { return AcceptImpl(*this, guest); }

template <typename R =  void>
class ClothingArticle : public BaseVisitable<R>
{
public:
    DEFINE_VISITABLE()
};

template <typename R =  void>
class Pants : public ClothingArticle<R>
{
public:
    DEFINE_VISITABLE()
};

template <typename R =  void>
class Shorts  : public ClothingArticle<R>
{
public:
    DEFINE_VISITABLE()
};

template <typename R =  void>
class Jacket : public ClothingArticle<R>
{
public:
    DEFINE_VISITABLE()
};

template <typename R =  void>
class MyConcreteVisitor : 
    public BaseVisitor,
    public Visitor<Pants<R>, R>,
    public Visitor<Shorts<R>, R>,
    public Visitor<Jacket<R>, R>
{
public:
    R Visit(Pants<R>&)
    {
        cout << "I just visited your pants, " << R(0) << endl;
        return R();
    }
    R Visit(Shorts<R>&)
    {
        cout << "I just visited your shorts, " << R(1) << endl;
        return R();
    }
    R Visit(Jacket<R>&)
    {
        cout << "I just visited your jacket, " << R(2) << endl;
        return R();
    }
};



 
typedef long HRESULT;
int mainf()
{
    Jacket<HRESULT> j;
    Shorts<HRESULT> s;
    Pants<HRESULT> p;
    MyConcreteVisitor<HRESULT> v;
    ClothingArticle<HRESULT>* ca = &j;

    ca->Accept(v);
    ca = &s;
    ca->Accept(v);
    ca = &p;
    ca->Accept(v);
    system ("pause");

	return 0;
}

