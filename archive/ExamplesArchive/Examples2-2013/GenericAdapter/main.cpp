//Local class, make adapter, return type implicit cast
//tons of inference with function pointers

#include <iostream>

template< class RT>
class Interface
{
public:
    virtual RT Fun() = 0;
};

template<
    class T, 
    typename RT, 
    class AT, 
    typename ART, 
    class AG
>
T* MakeAdaptor(RT (T::*func)(), AT& aobj, ART (AT::*afunc)(AG), AG& aarg)
{
    class Local : public T
    {
    public:
        Local (AT& aobj, ART (AT::*afunc)(AG), AG& aarg)
            :_obj(aobj)
            ,_func(afunc)
            ,_arg(aarg)
        {}
        RT Fun(){ return (_obj.*_func)(_arg); }
    private:
        AT& _obj; 
        ART (AT::*_func)(AG); 
        AG& _arg;
    };
    return new Local (aobj,afunc,aarg);
}

struct testint 
{
    char Call (int X) {std::cout << "value is: " << X << std::endl; return 'x';}
};

struct testchar 
{
    int Call (unsigned char X) {std::cout << "value is: " << X << std::endl;return 5;}
};

int main() 
{
    testint ti;
    testchar tc;
    int val = 1;
    unsigned char cval = 'A';

    std::cout << MakeAdaptor(&Interface<int>::Fun,ti,&testint::Call,val)->Fun() << std::endl;
    std::cout << MakeAdaptor(&Interface<char>::Fun,tc,&testchar::Call,cval)->Fun() << std::endl;
    system("pause");
} 