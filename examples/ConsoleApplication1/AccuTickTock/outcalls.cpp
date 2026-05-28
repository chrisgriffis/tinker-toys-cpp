#include "outcalls.h"
#include <stdio.h>

void outcalls::tick()
{
	printf("tick\n");
}

void outcalls::tock()
{
	printf("tock\n");
}

void outcalls::foo()
{
	printf("foo\n");
}

void outcalls::bar()
{
	printf("bar\n");
}

void outcalls::inactiveduring()
{
	printf("inactiveduring\n");
}

void outcalls::topduring()
{
	printf("topduring\n");
}

void outcalls::activeduring()
{
	printf("activeduring\n");
}
