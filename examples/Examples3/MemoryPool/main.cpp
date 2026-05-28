//simple example dynamic allocate to static memory pool

//make stl allocator

#include "Mutex.h"
#include <cstdio>
#include "StaticAllocate.h"
#include "Bar.h"
#include "Baz.h"
#include <vector>

extern Pool<StaticAllocate<Bar>,5> staticPool_Bar_5;
class tester
{
public:
    template<class T, unsigned N>
    static T (&exposeArr(Pool<T,N>& p))[N]{return p._pool;}
};

int main()
{
    using namespace std;
    Bar* p;
    for (int i = 0; i < 2 ; i++)
    {
        p = new StaticAllocate<Bar>();
        printf("%x, %x, %d\n",tester::exposeArr(staticPool_Bar_5),p,p-tester::exposeArr(staticPool_Bar_5));
    }

    delete (static_cast<StaticAllocate<Bar>*>(p)-1);
    p = new StaticAllocate<Bar>();
    printf("%x, %x, %d\n",tester::exposeArr(staticPool_Bar_5),p,p-tester::exposeArr(staticPool_Bar_5));
   vector<StaticAllocate<Bar>,StaticAllocator<StaticAllocate<Bar> > > v(1);
    return 0;
}


