//Implementation of CircularQueueOfInts

#include "RingPointer.h"
#include "ClippingCounter.h"
#include "CircularQueue.h"

#ifndef NULL
#define NULL 0
#endif

//no-fail constructor
Griffis::CircularQueueOfInts::CircularQueueOfInts()
    : m_pStorage(NULL)
    , m_pFrontOfQueue(NULL)
    , m_pBackOfQueue(NULL)
    , m_pNumInQueue(NULL)
    , m_IsInitialized(false)
    , m_CriticalSection()
{
    //no operation
}

//destructor
Griffis::CircularQueueOfInts::~CircularQueueOfInts()
{
    MyCriticalSectionLock locallyScopedLock(m_CriticalSection);
    delete[] m_pStorage;
    delete m_pFrontOfQueue;
    delete m_pBackOfQueue;
    delete m_pNumInQueue;
}

//Initialize method
//Accepts the runtime parameter that determines the queue size
//and initializes the object accordingly
//
//Returns a return code according to the IQueue contract.
//Uses a locally scoped CriticalSection to ensure serialized access.
//Method guaranteed to leave the critical section even in the case of an unhandled exception
int Griffis::CircularQueueOfInts::Initialize(size_t queueSize)
{
    //Lock the critical section
    MyCriticalSectionLock locallyScopedLock(m_CriticalSection);
    //Assume the object is already initialized
    int retVal = IQueue<int>::ALREADYINITIALIZED;
    if(!m_IsInitialized)
    {
        //otherwise, assume that the operation will fail
        //because the argument is invalid
        retVal = IQueue<int>::FAIL;
        if(queueSize > 0)
        {
            //otherwise, heap allocate the objects that depend on runtime values
            //and then complete initialization successfully
            m_pStorage = new int[queueSize];
            m_pFrontOfQueue = new RingPointer<int>(m_pStorage,queueSize);
            m_pBackOfQueue = new RingPointer<int>(m_pStorage,queueSize);
            m_pNumInQueue = new ClippingCounter(queueSize);
            m_IsInitialized = true;
            retVal = IQueue<int>::SUCCESS;
        }
    }
    return retVal;
    //The critical section Lock falls out of scope and exits the critical section
} 

//Enqueue method
//When successful, the Enqueue method assigns the back of the queue 
//to the value passed in and revises the queue structure accordingly
//
//Returns a return code according to the IQueue contract.
//Uses a locally scoped CriticalSection to ensure serialized access.
//Method guaranteed to leave the critical section even in the case of an unhandled exception.
int Griffis::CircularQueueOfInts::Enqueue(int const& inVal)
{
    //Lock the critical section
    MyCriticalSectionLock locallyScopedLock(m_CriticalSection);
    //Assume the object is not yet initialized
    int retVal = IQueue<int>::NOTINITIALIZED;
    if(m_IsInitialized)
    {
        //otherwise assume that the queue is full and will not accept new values
        retVal = IQueue<int>::FULL;
        if (!m_pNumInQueue->IsFull())
        {
            //otherwise assign the back of the queue to the value passed in
            m_pBackOfQueue->Dereference() = inVal;
            //ratchet the back of the queue one notch further away from the front of the queue
            m_pBackOfQueue->Increment();
            //keep track of the increase in size
            m_pNumInQueue->Increment();
            retVal = IQueue<int>::SUCCESS;
        }
    }
    return retVal;
    //The critical section Lock falls out of scope and exits the critical section
} 

//Dequeue method
//When successful, the Dequeue method assigns the the out value
//to that which is at the front of the queue and revises the 
//queue structure accordingly
//
//Returns a return code according to the IQueue contract.
//Uses a locally scoped CriticalSection to ensure serialized access.
//Method guaranteed to leave the critical section even in the case of an unhandled exception.
int Griffis::CircularQueueOfInts::Dequeue(int& outVal)
{
    //Lock the critical section
    MyCriticalSectionLock locallyScopedLock(m_CriticalSection);
    //Assume the object is not yet initialized
    int retVal = IQueue<int>::NOTINITIALIZED;
    if(m_IsInitialized)
    {
        //otherwise assume that the queue is empty
        retVal = IQueue<int>::EMPTY;
        if(!m_pNumInQueue->IsEmpty())
        {
            //otherwise assign the out value to what's at the front of the queue
            outVal = m_pFrontOfQueue->Dereference(); 
            //ratchet the front of the queue one notch closer to the back of the queue
            m_pFrontOfQueue->Increment();
            //keep track of the decrease in size
            m_pNumInQueue->Decrement();
            retVal = IQueue<int>::SUCCESS;
        }
    }
    return retVal;
    //The critical section Lock falls out of scope and exits the critical section
}

#undef NULL