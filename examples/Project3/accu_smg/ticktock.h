#include "HsmFramework.h"
#include "ioutcalls.h"
//////////////////////////////////////////////////////////////////////////
//specific state machine code
//////////////////////////////////////////////////////////////////////////


//create host
// needs next, getsig, dispatch
//plus any other host functions that will be called
enum Signal 
{
	evduring, evleave, evtran
};

//host HSM. signal and dispatch are artifacts 
//of this state machine, not artifacts of the framework.
class ticktock 
	: public HsmBase<ticktock>
{
public:
	ticktock(ioutcalls& i);
	~ticktock() {}

	Signal getSig() const { return sig_; }
	void dispatch(Signal sig);
	ioutcalls& m_outcalls;

private:
	Signal sig_;
};

//compositions/HSM hierarchy
typedef CompState<ticktock, 0> Top; //special
typedef CompState<ticktock, 1, Top> Running;

//compstate: smg,num,parent
//leafstate:same but parent is comp
typedef CompState<ticktock, 2, Running> active;
	typedef LeafState<ticktock, 3, active> tickstate;
	typedef LeafState<ticktock, 4, active> tockstate;

typedef CompState<ticktock, 5, Running> inactive;
	typedef LeafState<ticktock, 6, inactive> sub1;
	typedef LeafState<ticktock, 7, inactive> sub2;
