#include <cstdio>
#include "pool.h"
#include <memory>

template<class BASE>
struct StaticAllocate : public BASE
{

     typedef unsigned size_type;
//     StaticAllocate();
//     StaticAllocate(const_reference other);
    void* operator new(unsigned n) throw();
    void operator delete(void* p) throw();
};

template<class T>
struct StaticAllocator
{

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef T* const const_pointer;
    typedef const T& const_reference;
    typedef unsigned size_type;
    typedef ptrdiff_t difference_type;
    template <class U> struct rebind { typedef StaticAllocator<U> other; };

    StaticAllocator();
    StaticAllocator(const StaticAllocator& other);
    void* operator new(unsigned n) throw();
    void operator delete(void* p) throw();
    pointer address ( reference x ) const;
    const_pointer address ( const_reference x ) const;
    pointer allocate (size_type n, std::allocator<void>::const_pointer hint=0);
    void deallocate (pointer p, size_type n);
    size_type max_size() const throw();
    void construct ( pointer p, const_reference val );
    void destroy (pointer p);
};


#ifndef MAKE_STATIC_ALLOCATABLE
#define MAKE_STATIC_ALLOCATABLE(CName,slotCount) \
/*static*/ Pool<StaticAllocate<CName>,slotCount> staticPool_##CName##_##slotCount##; \
\
template<>\
void* StaticAllocate<CName>::operator new(StaticAllocate<CName>::size_type n)\
{\
    return staticPool_##CName##_##slotCount##.allocate();\
}\
template<>\
void StaticAllocate<CName>::operator delete(void* p)\
{\
    staticPool_##CName##_##slotCount##.deallocate(static_cast<StaticAllocate<CName>*>(p));\
}\
\
template<>\
    StaticAllocator<StaticAllocate<CName> >::StaticAllocator(){}\
template<>\
StaticAllocator<StaticAllocate<CName> >::StaticAllocator(const StaticAllocator<StaticAllocate<CName> >&){}\
template<>\
void* StaticAllocator<StaticAllocate<CName> >::operator new(StaticAllocator<StaticAllocate<CName> >::size_type n)\
{\
    return StaticAllocate<CName>::operator new(n);\
}\
template<>\
void StaticAllocator<StaticAllocate<CName> >::operator delete(void* p)\
{\
    StaticAllocate<CName>::operator delete(static_cast<StaticAllocate<CName>*>(p));\
}\
\
template<>\
void StaticAllocator<StaticAllocate<CName> >::destroy( StaticAllocator<StaticAllocate<CName> >::pointer p )\
{\
    if(NULL!=p)static_cast<CName*>(p)->~CName();\
}\
\
template<>\
void StaticAllocator<StaticAllocate<CName> >::construct( StaticAllocator<StaticAllocate<CName> >::pointer p, StaticAllocator<StaticAllocate<CName> >::const_reference val )\
{\
/*don't know what to put here; cant call constructor directly*/\
}\
\
template<>\
StaticAllocator<StaticAllocate<CName> >::size_type StaticAllocator<StaticAllocate<CName> >::max_size() const throw()\
{\
    return staticPool_##CName##_##slotCount##.remainingSlots();\
}\
\
template<>\
void StaticAllocator<StaticAllocate<CName> >::deallocate( StaticAllocator<StaticAllocate<CName> >::pointer p, StaticAllocator<StaticAllocate<CName> >::size_type n )\
{\
    if(sizeof(StaticAllocate<CName>) != n)return;\
    staticPool_##CName##_##slotCount##.deallocate(static_cast<StaticAllocate<CName>*>(p));\
}\
\
template<>\
StaticAllocator<StaticAllocate<CName> >::pointer StaticAllocator<StaticAllocate<CName> >::allocate( StaticAllocator<StaticAllocate<CName> >::size_type n, std::allocator<void>::const_pointer hint )\
{\
    if(sizeof(StaticAllocate<CName>) != n)return NULL;\
    if(hint != 0)return NULL;\
    return staticPool_##CName##_##slotCount##.allocate();\
}\
\
template<>\
StaticAllocator<StaticAllocate<CName> >::const_pointer StaticAllocator<StaticAllocate<CName> >::address( StaticAllocator<StaticAllocate<CName> >::const_reference x ) const\
{\
    return const_cast<StaticAllocator<StaticAllocate<CName> >::const_pointer>(&x);\
}\
\
template<>\
StaticAllocator<StaticAllocate<CName> >::pointer StaticAllocator<StaticAllocate<CName> >::address( StaticAllocator<StaticAllocate<CName> >::reference x ) const\
{\
    return &x;\
}
#endif