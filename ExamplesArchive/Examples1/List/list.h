#pragma once

#ifndef NULL
#define NULL 0
#endif
#include <iterator>

template< class T >
struct TNode
{
    TNode() 
        : key (NULL)
        , rightNode(NULL)
        , leftNode(NULL)
    {
        //nothing here
    }
    
    TNode(T t) 
        : key (t)
        , rightNode(NULL)
        , leftNode(NULL)
    {
        //nothing here
    }

    TNode(T t,TNode*& leftn, TNode*& rightn)
        : key (t)
        , rightNode(rightn)
        , leftNode(leftn)
    {
        //nothing here
    }

    T key;
    TNode* rightNode;
    TNode* leftNode;

//private:
//     TNode& operator =(const TNode& rhs)
//     {
//         if (this != &rhs)
//         {
//             key =  rhs.key;
//             delete rightNode;
//             rightNode = rhs.rightNode;
//             delete leftNode;
//             leftNode = rhs.leftNode;
//         }
//         return *this;
//     }
// 
};


//create a doubly linked list
//expose the put, get, and InsertAt
//use the iterator pattern


namespace Griffis
{
    template< class T > 
    class List
    {
    public:
        class iterator : public std::iterator<std::bidirectional_iterator_tag,TNode<T> >
        {
            TNode<T>* m_node;
        public:
            //constructor, copy constructor, increment, decrement, post-and pre-, equals equals and != 
            iterator(TNode<T>* t):m_node(t){}
            iterator ( const iterator& it):m_node(it.m_node){}
            iterator& operator++ (){m_node = m_node->rightNode; return *this;}
            iterator operator++ (int){} //to do
            iterator& operator--(){m_node = m_node->leftNode; return *this;}
            iterator operator--(int) {}//to do
            //iterators are equal if they both point to the same node
            bool operator== (const iterator& rhs){return m_node == rhs.m_node;}
            bool operator!= (const iterator& rhs){return !(m_node == rhs.m_node);}
            TNode<T>& operator*(){return *m_node;}
            TNode<T>* operator->(){return m_node;}
        };


        typedef TNode<T> Node;
        
        Node* m_Front;
        Node* m_Back;

        List()
            :m_Front(NULL)
            ,m_Back(NULL)
        {

        }
//         virtual ~List();
        void put (T t)
        {
            if (m_Front == NULL)
            {
                m_Front = new Node(t);
                m_Back = m_Front;
            } 
            else
            {
                Node* temp = new Node(t);
                temp->leftNode = m_Back;
                m_Back->rightNode = temp;
                m_Back = temp;
            }
        }
        iterator begin(){return iterator(m_Front);}
        iterator end(){return iterator(m_Back->rightNode);}

    };
}