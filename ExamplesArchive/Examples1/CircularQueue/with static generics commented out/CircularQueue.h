#pragma once

#include "IQueue.h"
#include "RingPointer.h"
#include "ClippingCounter.h"

namespace Griffis
{
    class CircularQueueOfInts : public IQueue<int>
    {
        int* m_pStorage;
        RingPointer<int>* m_pFront;
        RingPointer<int>* m_pBack;
        ClippingCounter m_NumInQueue;
        bool m_IsInitialized;

    public:
        CircularQueueOfInts()
            : m_pStorage(NULL)
            , m_pFront(NULL)
            , m_pBack(NULL)
            , m_NumInQueue()
            , m_IsInitialized(false)
        {
            // no fail construction idiom defers object creation to initialize method
        }

        virtual ~CircularQueueOfInts()
        {
            delete[] m_pStorage;
            delete m_pFront;
            delete m_pBack;
        }

        int Initialize(size_t queueSize)
        {
            //crit sec
            int retVal = IQueue<int>::NOTINITIALIZED;
            if(!m_IsInitialized && queueSize > 0)
            {
                m_pStorage = new int[queueSize];
                m_pFront = new RingPointer<int>(m_pStorage,queueSize);
                m_pBack = new RingPointer<int>(m_pStorage,queueSize);
                m_NumInQueue.Init(queueSize);
                m_IsInitialized = true;
                retVal = IQueue<int>::SUCCESS;
            }
            return retVal;
            //end crit sec
        }

        int Enqueue(int i)
        {
            int retVal = IQueue<int>::NOTINITIALIZED;
            if(m_IsInitialized)
            {
                (*m_pBack).Dereference() = i;
                (*m_pBack).Increment();
                m_NumInQueue.Increment();
                retVal = IQueue<int>::SUCCESS;
            }
            return retVal;
        }

        int Dequeue(int & out)
        {
            int retVal = IQueue<int>::NOTINITIALIZED;
            if(m_IsInitialized)
            {
                retVal = IQueue<int>::FAIL;
                if(m_NumInQueue > 0)
                {
                    out = (*m_pFront).Dereference(); 
                    (*m_pFront).Increment();
                    m_NumInQueue.Decrement();
                    retVal = IQueue<int>::SUCCESS;
                }
            }
            return retVal;
        }
    };


    
//     template<class EltType = int>
// 	class IQueue
// 	{
// 	public:
// 		typedef EltType EltType;
// 		virtual RESULT Initialize() = 0;
// 		virtual void Reset() = 0;
// 		virtual RESULT Enqueue(EltType) = 0;
// 		virtual RESULT Dequeue(EltType&) = 0;
// 		virtual ~IQueue() = 0;
// 	};
// 	template<class EltType> inline IQueue<EltType>::~IQueue(){}	


// 	template<size_t Capacity = 1, class EltType = int> //RetCode, EltType 
// 	class CircularQueue : public IQueue<EltType>
// 	{
// 		EltType m_Storage[Capacity];
// 		RingPtr<Capacity,EltType> front; //
// 		RingPtr<Capacity,EltType> back; //
// 		ClippingCounter<Capacity> numInQueue;
// 		bool isInitialized;
// 
// 	public:
// 		typedef EltType EltType;
// 		CircularQueue()
// 			: front(&m_Storage[0])
// 			, back(&m_Storage[0])
// 			, numInQueue()
// 			, isInitialized(false)
// 		{
// 			// no fail construction
// 		}
// 		virtual ~CircularQueue(){}
// 		RESULT Initialize()
// 		{
// 			//crit sec
// 			RESULT retVal = FAIL;
// 			if(!isInitialized)
// 			{
// 				Reset();
// 				isInitialized = true;
// 				retVal = SUCCESS;
// 			}
// 			return retVal;
// 			//end crit sec
// 		}
// 		
// 		void Reset()
// 		{
// 			numInQueue = 0;
// 			memset (m_Storage,0,Capacity);
// 		}
// 
// 		RESULT Enqueue(EltType i)
// 		{
// 			RESULT retVal = FAIL;
// 			if(isInitialized)
// 			{
// 				*back = i;
// 				++back;
// 				++numInQueue;
// 				retVal = SUCCESS;
// 			}
// 			return retVal;
// 		}
// 		
// 		RESULT Dequeue(EltType& out)
// 		{
// 			out = 0;
// 			RESULT retVal = FAIL;
// 			if(isInitialized && numInQueue > 0)
// 			{
// 				out = *front; 
// 				++front;
// 				--numInQueue;
// 				retVal = SUCCESS;
// 			}
// 			return retVal;
// 		}
// 	};
// 
// 	template<>
// 	class CircularQueue<0>
// 	{
// 	private:
// 		CircularQueue();
// 	};

}
