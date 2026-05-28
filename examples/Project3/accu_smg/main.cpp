
#define FOO
#include "ticktock.h"
#include "outcalls.h"
#include <vector>

namespace Insitu
{
	static void trampoline(int argc, const char* argv[])
	{
		outcalls o;
		ticktock test(o);
		printf("trampoline running\n");
		static const std::vector<char> foo
		{
			't','d','t','d','t','d','t',
			'l','d',
			't','d','t','d','t','d','t',
			'l','d',
		};
		while (true)
			for (char c : foo) 
			{
				switch (c)
				{
				case 'd':
					printf("case: d\n");
					test.dispatch(evduring);
					break;
				case 'l':
					printf("case: l\n");
					test.dispatch(evleave);
					break;
				case 't':
					printf("case: t\n");
					test.dispatch(evtran);
					break;
				case 'e':
					return;
				}
#ifndef FOO
				usleep(1000000);
#else
				getc(stdin);
#endif
			}
	}
}

#ifndef FOO
//
extern "C" __EXPORT int Accu_main(int argc, const char *argv[]);
int Accu_main(int argc, const char *argv[])
{
	printf("Accu running\n");
	px4_task_spawn_cmd("Accu",
		SCHED_DEFAULT,
		SCHED_PRIORITY_MAX - 5,
		1200,
		(main_t)&Insitu::trampoline,
		nullptr);

	return OK;
}
#else

int altmain();
int main()
{
// 	return altmain();
	Insitu::trampoline(1, nullptr);
}

int altmain()
{
	outcalls o;
	ticktock test(o);
	for (;;) 
	{
		char c = getc(stdin);
		getc(stdin);
		switch (c)
		{
		case 'd':
			test.dispatch(evduring);
			break;
		case 'l':
			test.dispatch(evleave);
			break;
		case 't':
			test.dispatch(evtran);
			break;
		case 'e':
			return 0;
		}
	}
}
#endif