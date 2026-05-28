#pragma once

#include "Exceptions.h"

namespace Griffis
{
    class ElementType;
    //ring pointer
    //encapsulates wraparound behavior of a pointer to a sequence of elements for a given window size
    //exposes prefix increment operator and dereferencing operator
    template <class ElementType>
    class RingPointer 
    {
        ElementType* m_pAnchorElement;
        size_t m_Offset;
        size_t m_NumElements;

    public:
        typedef typename RingPointer::ElementType ElementType;
        RingPointer(ElementType*& p, size_t numElements) 
            : m_pAnchorElement(p)
            , m_Offset(0)
            , m_NumElements(numElements)
        {
            // no fail constructor defers null pointer error handling from construction first access
        }

        ElementType& Dereference() const
        {
            if (NULL == m_pAnchorElement)
            {
                throw new NullPointerException();
            }
            ElementType* ptr(m_pAnchorElement + m_Offset);
            return *ptr;
        }

        RingPointer& Increment()
        {
            if (NULL == m_pAnchorElement)
            {
                throw new NullPointerException();
            }
            if(m_Offset < (m_NumElements-1))
            {
                m_Offset = m_Offset + 1;
            }
            else
            {
                m_Offset = 0;
            }
            return *this;
        }

    private:
        RingPointer& operator=(RingPointer const &);
        RingPointer();
    };



    
//     template<size_t Capacity = 1, class T = int>
// 	class RingPtr 
// 	{
// 	public:
// 		typedef T EltType;
// 		typedef T* PtrType;
// 		typedef T& RefType;
// 		RingPtr(PtrType p) 
// 			: itr(p)
// 			, offset(0)
// 		{}
// 
// 		RefType operator*() const
// 		{
// 			PtrType p(itr + offset);
// 			return *p;
// 		}
// 
// 		RingPtr& operator++()
// 		{
// 			if(offset < (Capacity-1))
// 			{
// 				offset = offset + 1;
// 			}
// 			else
// 			{
// 				offset = 0;
// 			}
// 			return *this;
// 		}
// 
// 	private:
// 		PtrType itr;
// 		unsigned offset;
// 		RingPtr(){}
// 	};
// 
// 
}
