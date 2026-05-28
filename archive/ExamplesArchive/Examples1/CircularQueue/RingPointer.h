#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace Griffis
{
    //Ring Pointer
    //
    //Encapsulates wraparound behavior of a pointer to a simple array of 
    //elements a parameterized type.
    //
    //Simple methods provide the ability to "ratchet" the pointer forward indiscriminately
    //and obtain the object pointed to.
    //For simplicity sake, the class does not do bounds checking. 
    //
    //It is the responsibility of the class user to ensure
    //that the number of elements the ring pointer can point to is not greater than the size
    //of the array that the anchor element points to.
    //
    //It is the responsibility of the class user to ensure that the anchor element
    //passed in during construction time is the zeroeth [0] element of the array.
    template <class ElementType>
    class RingPointer 
    {
        ElementType* m_pAnchorElement;  //pointer to Zeroeth array element
        size_t m_Offset;                //internal state determining position
        size_t m_NumElements;           //size of the simple array

    public:
        typedef typename RingPointer::ElementType ElementType;
        RingPointer(ElementType*& p, size_t numElements) 
            : m_pAnchorElement(p)
            , m_Offset(0)
            , m_NumElements(numElements)
        {
            // no operation
        }

        //Dereference: return access to the current object
        //I elected not to overload operator* so as not to
        //appear as if offering a more robust facility than the method provided
        ElementType& Dereference() const
        {
            if (NULL == m_pAnchorElement)
            {
                //leaving nontrivial exception types out of scope for this exercise
                throw 0;
            }
            ElementType* ptr(m_pAnchorElement + m_Offset);
            return *ptr;
        }

        //Increment: "ratchet" the ring pointer forward the next element in the array
        //without concern for wraparound.
        //I elected not to overload prefix operator++ so as not to
        //appear as if offering a more robust facility than the method provided
        RingPointer& Increment()
        {
            if (NULL == m_pAnchorElement)
            {
                //leaving nontrivial exception types out of scope for this exercise
                throw 0;
            }
            //if the next array index is not the size of the array
            if(m_Offset < (m_NumElements-1))
            {
                ++m_Offset;
            }
            //otherwise, either out of bounds or incrementing will wrap around
            else
            {
                m_Offset = 0;
            }
            return *this;
        }

    private:
        // copy ops are private to prevent copying
        RingPointer(const RingPointer&);
        RingPointer& operator=(const RingPointer &);
        //hiding simple constructor provides rudimentary compile time
        //guard against bad usage
        RingPointer();
    };
}
#undef NULL