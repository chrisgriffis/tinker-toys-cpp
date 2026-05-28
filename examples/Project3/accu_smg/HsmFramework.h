#pragma once

//////////////////////////////////////////////////////////////////////////
// Framework code
//////////////////////////////////////////////////////////////////////////

//pure virtual base class of all states
template<typename Hsm>
struct IState 
{
	typedef Hsm Host;
	typedef void Base;
	virtual void handler(Host&) const = 0;
	virtual unsigned getId() const = 0;
	//virtual ~TopState() {} 
};

//incomplete template, composite state
template
<
	typename Hsm, 
	unsigned id, 
	typename Parent
> 
struct CompState;

//general template, composite state
template
<
	typename Hsm, 
	unsigned id, 
	typename Parent = CompState<Hsm, 0, IState<Hsm> > 
>
struct CompState 
	: Parent 
{
	typedef Parent Base;
	typedef CompState<Hsm, id, Base> This;

	template<typename X> 
	void handle(Hsm& h, const X& x) const 
	{
		Base::handle(h, x);
	}
	static void init(Hsm&); // no implementation
	static void entry(Hsm&) {}
	static void exit(Hsm&) {}
};

//specialization, composite state
template<typename Hsm>
struct CompState<Hsm, 0, IState<Hsm> > 
	: IState<Hsm> 
{
	typedef IState<Hsm> Base;
	typedef CompState<Hsm, 0, Base> This;

	template<typename X> 
	void handle(Hsm&,const X&) const {}

	static void init(Hsm&); // no implementation
	static void entry(Hsm&) {}
	static void exit(Hsm&) {}
};

//general template, leaf state
template
<
	typename Hsm, 
	unsigned id, 
	typename Parent = CompState<Hsm, 0, IState<Hsm> > 
>
struct LeafState 
	: Parent 
{
	typedef Parent Base;
	typedef LeafState<Hsm, id, Base> This;

	template<typename X> 
	void handle(Hsm& h,const X& x) const 
	{
		Base::handle(h, x);
	}
	virtual void handler(Hsm& h) const
	{
		handle(h, *this);
	}
	virtual unsigned getId() const { return id; }
	static void init(Hsm& h) { h.setInitState(obj); }	// don't specialize this
	static void entry(Hsm&) {}
	static void exit(Hsm&) {}

	static const LeafState obj;
};
//implementation tells compiler to instantiate
template<typename Hsm, unsigned id, typename Parent>
const LeafState<Hsm, id, Parent> LeafState<Hsm, id, Parent>::obj;

//initial pseudo state
template<typename T>
struct Init
{
	typedef typename T::Host Host;

	Init(Host& h)
		: host_(h) {}
	~Init()
	{
		T::entry(host_);
		T::init(host_);
	}
	Host& host_;
};

//////////////////////////////////////////////////////////////////////////
// template metaprogramming helpers

//static bool
template<bool>
struct Bool {};

//SFINAE class derivation test
template
<
	class Derived,
	class Ancestor
>
class IsDerivedFrom
{
private:
	class Yes { char a[1]; };
	class No { char a[2]; };
	static Yes Test(Ancestor*); // undefined
	static No Test(...); // undefined
public:
	enum
	{
		Res =
		sizeof(Test(static_cast<Derived*>(0))) ==
		sizeof(Yes)
		? 1 : 0
	};
};

//////////////////////////////////////////////////////////////////////////
//transition logic
template
<
	typename C, // Current
	typename S, // Source
	typename T  // Target
> 
struct Tran 
{
	typedef typename C::Host Host;
	typedef typename C::Base CurrentBase;
	typedef typename S::Base SourceBase;
	typedef typename T::Base TargetBase;
	
	enum 
	{ // work out when to terminate
		   // template recursion
		eTB_CB = IsDerivedFrom<TargetBase,
		CurrentBase>::Res,
		eS_CB = IsDerivedFrom<S, CurrentBase>::Res,
		eS_C = IsDerivedFrom<S, C>::Res,
		eC_S = IsDerivedFrom<C, S>::Res,
		exitStop = eTB_CB && eS_C,
		entryStop = eS_C || (eS_CB && !eC_S)
	};

	// Use overloading to stop recursion. The
	// more natural template specialization
	// method would require to specialize the
	// inner template without specializing the
	// outer one, which is forbidden.
	static void exitActions(Host&, Bool<true>) {}
	static void exitActions(Host& h, Bool<false>) 
	{
		C::exit(h);
		Tran<CurrentBase, S, T>::exitActions(h,Bool<exitStop>());
	}
	static void entryActions(Host&, Bool<true>) {}
	static void entryActions(Host& h, Bool<false>) 
	{
		Tran<CurrentBase, S, T>::entryActions(h,Bool<entryStop>());
		C::entry(h);
	}

	Tran(Host& h) 
		: host_(h)
	{
		exitActions(host_, Bool<false>());
	}
	~Tran() 
	{
		Tran<T, S, T>::entryActions(host_,Bool<false>()); 
		T::init(host_);
	}
	Host& host_;
};

//base class for any host state machine
template<typename Hsm>
class HsmBase
{
public:
	HsmBase() : state_(nullptr) {}
	/*not virtual*/ void setInitState(const IState<Hsm>& state) { state_ = &state; }
protected:
	const IState<Hsm>* state_;
};

// end framework code
//////////////////////////////////////////////////////////////////////////
