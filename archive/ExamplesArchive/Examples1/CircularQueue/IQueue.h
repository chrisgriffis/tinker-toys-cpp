#pragma once

//Implement a circular queue of integers of user-specified size using a simple array. 
//Provide routines to initialize(), Enqueue() and Dequeue() the queue. 
//Make it thread safe. 

namespace Griffis
{
    //The IQueue interface defines a contract for adding and removing 
    //elements of parameterized type to a queue of runtime configurable size. 
    template<class T = void*>
    class IQueue
    {
    public:
        //The IQueue interface exposes the following types
        typedef const int ErrorCodeType;
        typedef T EltType;

        //The IQueue interface exposes the following error codes
        static ErrorCodeType SUCCESS = 0;
        static ErrorCodeType FAIL = 1;
        static ErrorCodeType NOTINITIALIZED = 2;
        static ErrorCodeType ALREADYINITIALIZED = 3;
        static ErrorCodeType EMPTY = 4;
        static ErrorCodeType FULL = 5;

        //The Initialize method creates an empty queue of size queueSize.
        //that is, during normal operation the queue shall have a
        //maximum capacity of queueSize elements. It is the responsibility 
        //of the interface user to guard against initializing the queue
        //with a negative number (which will be coerced into a very large
        //unsigned integer).
        //
        //Upon successful call to the initialize method, the queue shall 
        //be ready for subsequent successful Enqueue/Dequeue operations.
        //Attempts to use an uninitialized queue shall not change the 
        //state of the queue and return the NOTINITIALIZED error code.
        //Attempts to initialize an already initialized queue shall not 
        //change the state of the queue and return the ALREADYINITIALIZED error code.
        //////////////////////////////////////////////////////////////////////////
        virtual int Initialize(size_t queueSize) = 0;

        //The Enqueue method adds an element to the back of the queue.
        //The Enqueue method shall not modify the pass-by-reference parameter
        //If the queue is full, attempts to add more elements shall 
        //not change the state of the queue and return the FULL error code
        //////////////////////////////////////////////////////////////////////////
        virtual int Enqueue(EltType const& inVal) = 0;

        //The Dequeue method removes an element from the front of the queue.
        //The Dequeue method shall conditionally modify the pass-by-reference
        //parameter and assign it the value of that which has been removed from the queue
        //If the queue is empty, attempts to remove an element shall not 
        //change the state of the queue, perform no operation on the 
        //out-param, and return the EMPTY error code.
        //////////////////////////////////////////////////////////////////////////
        virtual int Dequeue(EltType& outVal) = 0;

        //Deleting an empty queue shall not result in memory leak.
        //Deleting the non-empty queue provides no guarantee that all 
        //objects referred to by the queue will also be deleted.
        //////////////////////////////////////////////////////////////////////////
        virtual ~IQueue() = 0;
    };
    //in-line definition needed for pure virtual destructor.
    template<class EltType> inline IQueue<EltType>::~IQueue(){}	
}