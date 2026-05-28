#include "CircularQueue.h"


Griffis::CircularQueueOfInts::CircularQueueOfInts()
: m_pStorage(NULL)
, m_pFront(NULL)
, m_pBack(NULL)
, m_NumInQueue()
, m_IsInitialized(false)
{
    InitializeCriticalSection(&m_CriticalSection);
}


Griffis::CircularQueueOfInts::~CircularQueueOfInts()
{
    EnterCriticalSection(&m_CriticalSection);
    delete[] m_pStorage;
    delete m_pFront;
    delete m_pBack;
    LeaveCriticalSection(&m_CriticalSection);
    DeleteCriticalSection(&m_CriticalSection);
}

int Griffis::CircularQueueOfInts::Initialize(size_t queueSize)
{
    EnterCriticalSection(&m_CriticalSection);
    int retVal = IQueue<int>::ALREADYINITIALIZED;
    if(!m_IsInitialized)
    {
        retVal = IQueue<int>::FAIL;
        if(queueSize > 0)
        {
            m_pStorage = new int[queueSize];
            m_pFront = new RingPointer<int>(m_pStorage,queueSize);
            m_pBack = new RingPointer<int>(m_pStorage,queueSize);
            m_NumInQueue.Init(queueSize);
            m_IsInitialized = true;
            retVal = IQueue<int>::SUCCESS;
        }
    }
    LeaveCriticalSection(&m_CriticalSection);
    return retVal;
}

int Griffis::CircularQueueOfInts::Enqueue(int inVal)
{
    EnterCriticalSection(&m_CriticalSection);
    int retVal = IQueue<int>::NOTINITIALIZED;
    if(m_IsInitialized)
    {
        retVal = IQueue<int>::FULL;
        if (!m_NumInQueue.IsFull())
        {
            (*m_pBack).Dereference() = inVal;
            (*m_pBack).Increment();
            m_NumInQueue.Increment();
            retVal = IQueue<int>::SUCCESS;
        }
    }
    LeaveCriticalSection(&m_CriticalSection);
    return retVal;
}

int Griffis::CircularQueueOfInts::Dequeue(int& outVal)
{
    EnterCriticalSection(&m_CriticalSection);
    int retVal = IQueue<int>::NOTINITIALIZED;
    if(m_IsInitialized)
    {
        retVal = IQueue<int>::EMPTY;
        if(!m_NumInQueue.IsEmpty())
        {
            outVal = (*m_pFront).Dereference(); 
            (*m_pFront).Increment();
            m_NumInQueue.Decrement();
            retVal = IQueue<int>::SUCCESS;
        }
    }
    LeaveCriticalSection(&m_CriticalSection);
    return retVal;
}
