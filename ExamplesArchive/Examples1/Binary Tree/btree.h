#pragma once

#ifndef NULL
#define NULL 0
#endif

namespace Griffis
{

    class BinaryTree
    {
    public:
    struct TreeNode
    {
        TreeNode();
        virtual ~TreeNode();
        int key;
        TreeNode* leftNode;
        TreeNode* rightNode;
    };

        BinaryTree();
        virtual ~BinaryTree();
        void CreateNode(int keyvalue);
        TreeNode* GetNode(int keyvalue);
    private:
        void DestroyNode(TreeNode* node);
        void CreateNode(int keyvalue, TreeNode* node);
        TreeNode* GetNode(int keyvalue,TreeNode* node);
        TreeNode* m_Root;
    };


    //Create a binary sort tree.
    //make it generic based on value type and enable taking comparison policy as a parameter
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
        T* key;
        TNode* rightNode;
        TNode* leftNode;
    };


    template< class T > 
    class Cmp
    {
    public:
        static bool comparelt(const T& lhs,const T& rhs){return lhs < rhs;}
        static bool compareeq (const T& lhs, const T& rhs){return lhs == rhs;}
    };
    
    template< class ValueType, class ComparisonPolicy = Cmp<ValueType> > 
    class TBinaryTree
    {
    public:
        typedef TNode<ValueType> BTNode;
        TBinaryTree(ValueType*& i){ m_Root = new BTNode(); m_Root->key = i;}
        virtual ~TBinaryTree(){}
        //void CreateNode(ValueType* pKeyvalue);
        //BTNode* GetNode(ValueType* pKeyvalue);
    //private:
        //void DestroyNode(BTNode* pNode);
        void CreateNode(ValueType* pKeyvalue, BTNode* pNode)
        {
            if (pKeyvalue != NULL && pNode != NULL)
            {
                if (ComparisonPolicy::comparelt(*pKeyvalue,*pNode->key))
                {
                    //traverse the left tree
                    if (pNode->leftNode != NULL)
                    {
                    	CreateNode(pKeyvalue,pNode->leftNode);
                    }
                    else //have reached a leaf node
                    {
                        pNode->leftNode = new BTNode();
                        pNode->leftNode->key = pKeyvalue;
                    }
                } 
                else
                {
                    //traverse the right tree
                    if (pNode->rightNode != NULL)
                    {
                    	CreateNode(pKeyvalue,pNode->rightNode);
                    } 
                    else //have reached a leaf node
                    {
                        pNode->rightNode = new BTNode();
                        pNode->rightNode->key = pKeyvalue;
                    }
                }
            } 
            else
            {
            }
        }
        //BTNode* GetNode(ValueType* pKeyvalue,BTNode* pNode);
        BTNode* m_Root;

    };
}
