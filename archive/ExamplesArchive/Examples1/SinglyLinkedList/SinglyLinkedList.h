#pragma once
#include <iterator>

namespace Griffis
{
    template<class T > 
    struct SimpleList 
    {
        SimpleList()
            : data(NULL)
            , nextNode(NULL)
        {
            //nothing here
        }
        SimpleList(T& t)
            : data(&t)
            , nextNode(NULL)
        {
            //nothing here
        }

		//prepend to an existing list
        SimpleList(T& t, SimpleList<T>* inVal)
            : data(&t)
            , nextNode(inVal)
        {
            //nothing here
        }

        SimpleList(const SimpleList<T>*& inVal)
            : data(inVal->data)
            , nextNode(inVal->nextNode)
        {
            //nothing here
        }

		SimpleList<T>& append(SimpleList<T>& inList)
		{
			SimpleList<T>* temp(this);
			while(temp->nextNode != NULL)
			{
				temp = temp->nextNode;
			}
			temp->nextNode = &inList;
			return *this;
		}
        ~SimpleList()
        {
			delete nextNode;
        }
        T* data;
        SimpleList<T>* nextNode;
    };

#ifdef pants
    template<class NodeType, class ValType> 
	class SinglyLinkedListIterator
	{
	private:
		SinglyLinkedListIterator();
	};

    template<class ValType> 
	class SinglyLinkedListIterator<SimpleList<ValType>,ValType> : public std::iterator<forward_iterator_tag, ValType>
	{
		SimpleList<ValType>* nodePointer
	public:
		SinglyLinkedListIterator(SimpleList<ValType>* x) :nodePointer(x) {}
		SinglyLinkedListIterator(const SinglyLinkedListIterator& mit) : nodePointer(mit.nodePointer) {}
		SinglyLinkedListIterator& operator++() {nodePointer = nodePointer->nextNode;return *this;}
		SinglyLinkedListIterator operator++(int) {myiterator tmp(*this); operator++(); return tmp;}
		bool operator==(const SinglyLinkedListIterator& rhs) {return nodePointer==rhs.nodePointer;}
		bool operator!=(const SinglyLinkedListIterator& rhs) {return nodePointer!=rhs.nodePointer;}
		std::iterator::reference operator*() {return *nodePointer->data;}
	};


    template<class T > 
    class SinglyLinkedList
    {
    //////////////////////////////////////////////////////////////////////////
    // Use STL-style iterator pattern
    //////////////////////////////////////////////////////////////////////////

        typedef T Datatype;
        typedef T* PtrDatatype;
        typedef SimpleList<T>* PtrNode;
        PtrNode m_pFirstNode;
    public:
        SinglyLinkedList()
            : m_pFirstNode(NULL)
        {

        }
        virtual ~SinglyLinkedList()
        {

        }

        void push_back(PtrDatatype d)
        {
            if (m_pFirstNode != NULL)
            {
                PtrNode next = m_pFirstNode->nextNode;
                while (next != NULL)
                {

                }
            } 
            else
            {
            }
        }

        void push_front (PtrDatatype d)
        {
        }

        PtrDatatype pop_back()
        {

        }

        PtrDatatype insert_after (PtrDatatype)
        {

        }
    };
#endif
}