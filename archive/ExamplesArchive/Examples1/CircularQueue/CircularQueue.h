#pragma once

#include "IQueue.h"
#include "MyCriticalSection.h"

#ifndef OVERRIDE
#define OVERRIDE
#endif

namespace Griffis
{
    //forward declarations
    class ClippingCounter;
    template<class T> class RingPointer;

    //Exercise 2:
    //Implement a circular queue of integers of user-specified size using a simple array. 
    //Provide routines to initialize(), enqueue() and dequeue() the queue. 
    //Make it thread safe. 
    //
    //The following class is a conditionally thread safe implementation of the IQueue interface
    //designed to meet the above criteria.
    //It utilizes only a "simple array" as its basis for storage.
    //The mechanism for thread safety serializes access to heap allocated elements that are themselves not 
    //thread safe but are otherwise inaccessible except through the synchronized interface.
    //Class performs no exception handling, but is guaranteed to never get stuck in the critical section
    //Dynamic memory allocation used to accommodate (runtime) sizing of the queue.
    class CircularQueueOfInts 

        //complies to the contract prescribed by IQueue.h
        : public IQueue<int>
    {
        int* m_pStorage;                    //simple array that stores the queue elements
        RingPointer<int>* m_pFrontOfQueue;  //maintains the array index of the front of the queue
        RingPointer<int>* m_pBackOfQueue;   //maintains the array index of the back of the queue
        ClippingCounter* m_pNumInQueue;     //constrains the displacement of back of queue relative to front
        bool m_IsInitialized;               //flag indicating successful initialization
        MyCriticalSection m_CriticalSection;//CriticalSection used to serialize shared resource access

    public:
        CircularQueueOfInts();
        virtual ~CircularQueueOfInts() OVERRIDE;

        //The Initialize method heap allocates an empty queue of size queueSize.
        int Initialize(size_t queueSize) OVERRIDE;

        //The Enqueue method adds an element to the back of the queue.
        int Enqueue(int const& inVal) OVERRIDE;

        //The dequeue method removes an element from the front of the queue.
        int Dequeue(int& outVal) OVERRIDE;
    };
}
#undef OVERRIDE