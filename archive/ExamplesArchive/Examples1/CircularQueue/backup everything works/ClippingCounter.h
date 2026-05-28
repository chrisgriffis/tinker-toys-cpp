#pragma once

#include "Exceptions.h"

namespace Griffis
{

    class ClippingCounter 
    {
        unsigned m_Count;
        unsigned m_Capacity;
        bool m_IsInit;

    public:
        ClippingCounter() 
            : m_Count(0)
            , m_Capacity(0)
            , m_IsInit(false)
        {
            // no fail construction idiom defers object creation to initialize method
        }

        void Init(unsigned capacity)
        {
            m_Count = 0;
            m_Capacity = capacity;
            m_IsInit = true;
        }

        bool IsFull() const
        {
            return m_Count == m_Capacity;
        }

        bool IsEmpty() const
        {

            if(m_IsInit)
            {
                return m_Count < 1;
            }
            else
            {
                throw new UnitializedObjectException();
            }
        }

        void ResetCount()
        {
            if(m_IsInit)
            {
                m_Count = 0;
            }
            else
            {
                throw new UnitializedObjectException();
            }
        }

        ClippingCounter& Increment()
        {
            if(m_IsInit)
            {
                if(m_Count < m_Capacity)
                {
                    m_Count = m_Count + 1;
                }
                else
                {
                    m_Count = m_Capacity;
                }
                return *this;
            }
            else
            {
                throw new UnitializedObjectException();
            }
        }

        ClippingCounter& Decrement()
        {
            if(m_IsInit)
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
                    m_Count = 0;
                }
                return *this;
            }
            else
            {
                throw new UnitializedObjectException();
            }
        }

    };
}
