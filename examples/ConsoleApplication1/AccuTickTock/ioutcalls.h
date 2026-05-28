#pragma once


struct ioutcalls
{
	virtual void topduring() = 0;
	virtual void activeduring() = 0;
	virtual void inactiveduring() = 0;
	virtual void tick() = 0;
	virtual void tock() = 0;
	virtual void foo() = 0;
	virtual void bar() = 0;
	virtual ~ioutcalls() {}
};
