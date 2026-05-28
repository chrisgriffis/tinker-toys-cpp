#pragma once

namespace Griffis
{
    //Clipping counter keeps count and frees the user from 
    //tracking exceedances of an upper or lower bound.
    //It maintains an internal value that incremented and 
    //decremented but cannot be decremented below zero and 
    //cannot be incremented above a configured threshold.
    class ClippingCounter 
    {
        unsigned m_Count;
        unsigned m_Capacity;

    public:
        ClippingCounter(unsigned capacity) 
            : m_Count(0)
            , m_Capacity(capacity)
        {
            //no operation
        }

        virtual ~ClippingCounter(){}

        bool IsFull() const
        {
            return m_Count == m_Capacity;
        }

        bool IsEmpty() const
        {
            return m_Count < 1;
        }

        //Increment method ratchets the counter one notch higher
        ClippingCounter& Increment()
        {
            if(m_Count < m_Capacity)
            {
                m_Count = m_Count + 1;
            }
            else
            {
                //explicitly set count to the maximum
                //even if it has exceeded the maximum
                m_Count = m_Capacity;
            }
            return *this;
        }

        //Decrement method ratchets the counter one notch lower
        ClippingCounter& Decrement()
        {
            if(m_Count >= m_Capacity)
            {
                m_Count = m_Capacity - 1;
            }
            else if (m_Count > 0)
            {
                --m_Count;
            }
            else
            {
                //explicitly set everything else zero
                //even in cases where the count is already zero
                m_Count = 0;
            }
            return *this;
        }

    private:
        //hiding simple constructor provides rudimentary compile time
        //guard against bad usage
        ClippingCounter();

    };
}
