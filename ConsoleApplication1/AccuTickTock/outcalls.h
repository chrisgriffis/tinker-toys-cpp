#pragma once
#include "ioutcalls.h"

struct outcalls : public ioutcalls
{
	outcalls() = default;
	outcalls(const outcalls&) = delete;
	outcalls(outcalls&&) = default;
	virtual void topduring() override;
	virtual void activeduring() override;
	virtual void inactiveduring() override;
	virtual void tick() override;
	virtual void tock() override;
	virtual void foo() override;
	virtual void bar() override;
};

