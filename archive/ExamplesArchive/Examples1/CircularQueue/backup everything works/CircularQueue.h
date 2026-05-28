#pragma once

#include "IQueue.h"
#include "RingPointer.h"
#include "ClippingCounter.h"
#include <windows.h>


namespace Griffis
{

    class CircularQueueOfInts : public IQueue<int>
    {
        int* m_pStorage;
        RingPointer<int>* m_pFront;
        RingPointer<int>* m_pBack;
        ClippingCounter m_NumInQueue;
        bool m_IsInitialized;
        CRITICAL_SECTION m_CriticalSection;

    public:
        CircularQueueOfInts();
        virtual ~CircularQueueOfInts();

        int Initialize(size_t queueSize);
        int Enqueue(int inVal);
        int Dequeue(int& outVal);
    };
}
