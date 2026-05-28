#include "ticktock.h"
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
//specific state machine code
//////////////////////////////////////////////////////////////////////////

namespace //Leafstates
{
	tickstate tis;
	tockstate tos;
	sub1 s1;
	sub2 s2;
}

ticktock::ticktock(ioutcalls& i) 
	: m_outcalls(i)
{
	//set initial leaf state
	//call into framework base class
	setInitState(tis);
}

void 
ticktock::dispatch(Signal sig)
{
	printf("\ndispatch got: %d\n", sig);
	sig_ = sig;
	printf("state: %p, this: %p\n", state_, this);

	//call into framework for action
	if(state_ != nullptr) state_->handler(*this);
}

//////////////////////////////////////////////////////////////////////////
//Running
template<> inline void 
Running::init(ticktock& h) 
{ 
	Init<inactive> i(h); 
	printf("Running-INIT;\n"); 
}

template<> template<typename X> inline void 
Running::handle(ticktock& h, const X& x) const
{
	printf("Running::handle\n");
	switch (h.getSig())
	{
	case evduring:
	case evtran:
		h.m_outcalls.topduring();
		break;
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

//////////////////////////////////////////////////////////////////////////
//active
template<> inline void 
active::init(ticktock& h) 
{ 
	Init<tickstate> i(h); 
	printf("active-INIT;\n");
}

//state machine behavior
template<> template<typename X> inline void
active::handle(ticktock& h, const X& x) const
{
	printf("active::handle\n");
	switch (h.getSig())
	{
	case evduring:
	case evtran:
		h.m_outcalls.activeduring();
		break;
	case evleave:
	{
		Tran<X, This, inactive> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

//////////////////////////////////////////////////////////////////////////
//tickstate
template<> inline void 
tickstate::entry(ticktock& hsm)
{
	//entry actions!
	printf("tickstate::entry\n");
	hsm.m_outcalls.tick();
}

template<> template<typename X> inline void
tickstate::handle(ticktock& h, const X& x) const
{
	printf("tickstate::handle\n");
	//entry and ext handled as separate overloads

	switch (h.getSig())
	{
	case evtran:
	{
		Tran<X, This, tockstate> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

//////////////////////////////////////////////////////////////////////////
//tockstate
template<> inline void 
tockstate::entry(ticktock& hsm)
{
	//entry actions!
	printf("tockstate::entry\n");
	hsm.m_outcalls.tock();
}

//state machine behavior
template<> template<typename X> inline void
tockstate::handle(ticktock& h, const X& x) const
{
	printf("tockstate::handle\n");
	//entry and ext handled as separate overloads

	switch (h.getSig())
	{
	case evtran:
	{
		Tran<X, This, tickstate> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}


//////////////////////////////////////////////////////////////////////////
//inactive
template<> inline void 
inactive::init(ticktock& h) 
{ 
	Init<sub1> i(h); 
	printf("inactive-INIT;\n"); 
}

//state machine behavior
template<> template<typename X> inline void
inactive::handle(ticktock& h, const X& x) const
{
	//entry and ext handled as separate overloads
	printf("inactive::handle\n");
	switch (h.getSig())
	{
	case evduring:
		h.m_outcalls.inactiveduring();
		break;
	case evleave:
	{
		Tran<X, This, active> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

//////////////////////////////////////////////////////////////////////////
//sub1
template<> inline void 
sub1::entry(ticktock& hsm)
{
	//entry actions!
	printf("sub1::entry\n");
	hsm.m_outcalls.foo();
}

template<> template<typename X> inline void
sub1::handle(ticktock& h, const X& x) const
{
	printf("sub1::handle\n");
	switch (h.getSig())
	{
	case evtran:
	{
		Tran<X, This, sub2> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

//////////////////////////////////////////////////////////////////////////
//sub2
template<> inline void 
sub2::entry(ticktock& hsm)
{
	//entry actions!
	printf("sub2::entry\n");
	hsm.m_outcalls.bar();
}

//state machine behavior
template<> template<typename X> inline void
sub2::handle(ticktock& h, const X& x) const
{
	//entry and ext handled as separate overloads
	printf("sub2::handle\n");
	switch (h.getSig())
	{
	case evtran:
	{
		Tran<X, This, sub1> t(h);
		break;
	}
	default:
		break;
	}

	//return parent from framework
	return Base::handle(h, x);
}

