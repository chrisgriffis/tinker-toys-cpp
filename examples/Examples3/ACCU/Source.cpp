
#include "stdio.h"

//////////////////////////////////////////////////////////////////////////
// Framework code
//////////////////////////////////////////////////////////////////////////

template<bool>
struct Bool{};

template<class D, class B>
class IsDerivedFrom 
{
private:
    class Yes { char a[1]; };
    class No { char a[10]; };
    static Yes Test( B* ); // undefined
    static No Test( ... ); // undefined
public:
    enum { Res = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) ? 1 : 0 }; 
};

template<typename H>
struct TopState 
{
    typedef H Host;
    typedef void Base;
    virtual void handler(Host&) const =0;
    virtual unsigned getId() const =0;
};

template<
    typename H, 
    unsigned id,
    typename B> 
struct CompState;

template<
    typename H, 
    unsigned id,
    typename B=CompState<H,0,TopState<H> > > 
struct CompState : B 
{ 
    typedef B Base; 
    typedef CompState<H,id,Base> This; 
    template<typename X> 
    void handle(H& h, const X& x) const { Base::handle(h,x); } 
    static void init(H&){} // no implementation 
    static void entry(H&) {} 
    static void exit(H&) {}
};

template<typename H>
struct CompState<H,0,TopState<H> > : TopState<H> 
{
    typedef TopState<H> Base;
    typedef CompState<H,0,Base> This;
    template<typename X> void handle(H&,
        const X&) const {}
    static void init(H&){} // no implementation 
    static void entry(H&) {} 
    static void exit(H&) {} 
};

template<typename H, unsigned id,
    typename B=CompState<H,0,TopState<H> > > 
struct LeafState : B 
{ 
    typedef B Base; 
    typedef LeafState<H,id,Base> This; 
    template<typename X> 
    void handle(H& h, const X& x) const { Base::handle(h,x); } 
    virtual void handler(H& h) const { handle(h,*this); } 
    virtual unsigned getId() const { return id; } 
    static void init(H& h) { h.next(obj); } // don't specialize this 
    static void entry(H&) {} 
    static void exit(H&) {} 
    static const LeafState obj; 
}; 
template<typename H, unsigned id, typename B> 
const LeafState<H, id, B> LeafState<H, id, B>::obj; 


template<typename C, typename S, typename T> // Current,Source,Target
struct Tran {
    typedef typename C::Host Host;
    typedef typename C::Base CurrentBase;
    typedef typename S::Base SourceBase;
    typedef typename T::Base TargetBase;
    enum { // work out when to terminate
        // template recursion
        eTB_CB = IsDerivedFrom<TargetBase,
        CurrentBase>::Res,
        eS_CB = IsDerivedFrom<S,CurrentBase>::Res,
        eS_C = IsDerivedFrom<S,C>::Res,
        eC_S = IsDerivedFrom<C,S>::Res,
        exitStop = eTB_CB && eS_C,
        entryStop = eS_C || eS_CB && !eC_S
    };
    // We use overloading to stop recursion. The
    // more natural template specialization
    // method would require to specialize the
    // inner template without specializing the
    // outer one, which is forbidden.
    static void exitActions(Host&, Bool<true>) {}
    static void exitActions(Host& h, Bool<false>){
        C::exit(h);
        Tran<CurrentBase,S,T>::exitActions(h,Bool<exitStop>());
    }
    static void entryActions(Host&, Bool<true>) {}
    static void entryActions(Host& h,Bool<false>)
    {
        Tran<CurrentBase,S,T>::entryActions(h,Bool<entryStop>());
        C::entry(h);
    }
    Tran(Host& h) : host_(h)
    { 
        exitActions(host_,Bool<false>());
    }
    ~Tran() 
    { 
        Tran<T,S,T>::entryActions(host_,Bool<false>()); 
        T::init(host_); 
    }
    Host& host_;
};

template<typename T>
struct Init {
    typedef typename T::Host Host;
    Init(Host& h) : host_(h) {}
    ~Init() { T::entry(host_); T::init(host_); }
    Host& host_;
};

// end framework code
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//specific state machine code
//////////////////////////////////////////////////////////////////////////


//create host
// needs next, getsig, dispatch
//plus any other host functions that will be called
enum Signal { A_SIG,B_SIG,C_SIG,D_SIG,
    E_SIG,F_SIG,G_SIG,H_SIG };
class TestHSM {
public:
    TestHSM(){}
    ~TestHSM(){}
    void next(const TopState<TestHSM>& state)
    { state_ = &state; }
    Signal getSig() const { return sig_; }
    void dispatch(Signal sig)
    { sig_ = sig; state_->handler(*this); }
    void foo(int i) { foo_ = i; }
    int foo() const { return foo_; }
private:
    const TopState<TestHSM>* state_;
    Signal sig_;
    int foo_;
};

//compositions/HSM hierarchy
typedef CompState<TestHSM,0> Top;
typedef CompState<TestHSM,1,Top> S0;
typedef CompState<TestHSM,2,S0> S1;
typedef LeafState<TestHSM,3,S1> S11;
typedef CompState<TestHSM,4,S0> S2;
typedef CompState<TestHSM,5,S2> S21;
typedef LeafState<TestHSM,6,S21> S211;


//state transitions
template<> template<typename X> 
inline void S0::handle(TestHSM& h, const X& x) const 
{
    switch(h.getSig()) 
    {
    case E_SIG: 
        { 
            Tran<X,This,S211> t(h);
            printf("s0-E;"); return; 
        }
    default: 
        break;
    }
    return Base::handle(h,x);
}

template<> template<typename X> 
inline void S1::handle(TestHSM& h, const X& x) const 
{
    switch(h.getSig()) 
    {
    case A_SIG: 
        { Tran<X,This,S1> t(h);
        printf("s1-A;"); return; 
        }
    case B_SIG: 
        { Tran<X,This,S11> t(h);
        printf("s1-B;"); return; 
        }
    case C_SIG: 
        { Tran<X,This,S2> t(h);
        printf("s1-C;"); return; 
        }
    case D_SIG: 
        { Tran<X,This,S0> t(h);
        printf("s1-D;"); return; 
        }
    case F_SIG: 
        { Tran<X,This,S211> t(h);
        printf("s1-F;"); return; 
        }
    default: 
        break;
    }
    return Base::handle(h,x);
}

template<> template<typename X> 
inline void S11::handle(TestHSM& h, const X& x) const 
{
    switch(h.getSig()) 
    {
    case G_SIG: 
        { 
            Tran<X,This,S211> t(h);
            printf("s11-G;"); 
            return; 
        }
    case H_SIG: 
        if(h.foo()) 
        {
            printf("s11-H;");
            h.foo(0); 
            return;
        } 
        break;
    default: 
        break;
    }
    return Base::handle(h,x);
}

template<> template<typename X> 
inline void S2::handle(TestHSM& h, const X& x) const 
{
    switch(h.getSig()) 
    {
    case C_SIG: 
        { 
            Tran<X,This,S1> t(h);
            printf("s2-C;"); 
            return; 
        }
    case F_SIG: 
        { 
            Tran<X,This,S11> t(h);
            printf("s2-F;"); 
            return; 
        }
    default: 
        break;
    }
    return Base::handle(h,x);
}

template<> template<typename X> 
inline void S21::handle(TestHSM& h, const X& x) const
{
    switch(h.getSig()) 
    {
    case B_SIG: 
        { 
            Tran<X,This,S211> t(h);
            printf("s21-B;"); 
            return; 
        }
    case H_SIG: 
        if(!h.foo()) 
        {
            Tran<X,This,S21> t(h);
            printf("s21-H;"); 
            h.foo(1);
            return;
        } 
        break;
    default:
        break;
    }
    return Base::handle(h,x);
}

template<> template<typename X> 
inline void S211::handle(TestHSM& h, const X& x) const 
{
    switch(h.getSig()) 
    {
    case D_SIG: 
        { 
            Tran<X,This,S21> t(h);
            printf("s211-D;"); 
            return; 
        }
    case G_SIG: 
        { 
            Tran<X,This,S0> t(h);
            printf("s211-G;"); 
            return; 
        }
    default: 
        break;
    }
    return Base::handle(h,x);
}



// entry actions
template<> inline void S0 ::entry(TestHSM&)
{ printf("s0-ENTRY;"); }
template<> inline void S1 ::entry(TestHSM&)
{ printf("s1-ENTRY;"); }
template<> inline void S11 ::entry(TestHSM&)
{ printf("s11-ENTRY;"); }
template<> inline void S2 ::entry(TestHSM&)
{ printf("s2-ENTRY;"); }
template<> inline void S21 ::entry(TestHSM&)
{ printf("s21-ENTRY;"); }
template<> inline void S211 ::entry(TestHSM&)
{ printf("s11-ENTRY;"); }
// and so on...

// exit actions
template<> inline void S0 ::exit(TestHSM&)
{ printf("s0-EXIT;"); }
template<> inline void S1 ::exit(TestHSM&)
{ printf("s1-EXIT;"); }
template<> inline void S11 ::exit(TestHSM&)
{ printf("s11-EXIT;"); }
template<> inline void S2 ::exit(TestHSM&)
{ printf("s2-EXIT;"); }
template<> inline void S21 ::exit(TestHSM&)
{ printf("s21-EXIT;"); }
template<> inline void S211 ::exit(TestHSM&)
{ printf("s11-EXIT;"); }
// and so on...





int main() {
    S11 s11;
    S211 s211;
    TestHSM test;
    test.next(s11);
    // Init<S0>(test);
    for(;;) {
        printf("\nSignal<-");
        char c = getc(stdin);
        getc(stdin); // discard '\n'
        if(c<'a' || 'h'<c) {
            return 0;
        }
        test.dispatch((Signal)(c-'a'));
    }
}
