#include <cstring>
#include <cstdio>
#include "Mutex.h"
#define LOCK_SCOPE() cg::Lock l(_mutex);

template<class T, unsigned N=1>
class Pool
{
protected:
    //registry managed separately from pool to preserve contiguous memory semantics
    bool _registry[N];
    T _pool[N];
    mutable cg::Mutex _mutex; //mutable allows cast to ref in const functions
    unsigned _slotCount;
public:
    friend class tester;

    Pool()
    {
        LOCK_SCOPE();
        _slotCount=N;
        printf("Pool()\n");
        memset(_registry,0,sizeof _registry);
        memset(_pool,0,sizeof _pool);
    }
    T* allocate() throw() //poor man's noexcept
    {
        LOCK_SCOPE();
//         if(0==_slotCount || _slotCount>N) return NULL;
        for (int i = 0; i < N ; i++)
        {
            if(false == _registry[i])
            {
                _registry[i] = true;
                printf("allocate %x\n",&_pool[i]);
                return &_pool[i];
                --_slotCount;
            }
        }
        return NULL;
    }

    void deallocate(T* p) throw()
    {
        LOCK_SCOPE();
        if(_slotCount>N) return;
        unsigned diff;
        if(_pool > p) return;
        diff = p - _pool;
        if(diff >= N) return;
        printf("deallocate %x\n",p);
        _registry[diff]=false;
        ++_slotCount;
    }

    unsigned remainingSlots() const throw()
    {
        //slotcount could be invalid right after its read
        LOCK_SCOPE(); //to prevent race on (de)allocate
        return _slotCount;
    }
};
