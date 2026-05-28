#include "btree.h"

using namespace Griffis;

Griffis::BinaryTree::TreeNode::TreeNode()
: key (-1)
, leftNode(NULL)
, rightNode(NULL)
{

}

Griffis::BinaryTree::TreeNode::~TreeNode()
{

}

Griffis::BinaryTree::BinaryTree()
: m_Root(NULL)
{

}

Griffis::BinaryTree::~BinaryTree()
{
    if (m_Root != NULL)
    {
        DestroyNode(m_Root);
        m_Root = NULL;
    }
}

void Griffis::BinaryTree::CreateNode( int keyvalue )
{
    if (m_Root == NULL)
    {
        m_Root = new BinaryTree::TreeNode;
        m_Root->key = keyvalue;
    } 
    else
    {
        return CreateNode(keyvalue,m_Root);
    }
}

void Griffis::BinaryTree::CreateNode( int keyvalue, BinaryTree::TreeNode* node )
{
    if (keyvalue < node->key)
    {
        if (node->leftNode == NULL)
        {
            node->leftNode = new BinaryTree::TreeNode;
            node->leftNode->key = keyvalue;
        } 
        else
        {
            CreateNode(keyvalue,node->leftNode);
        }
    } 
    else
    {
        if (node->rightNode == NULL)
        {
            node->rightNode = new BinaryTree::TreeNode;
            node->rightNode->key = keyvalue;
        } 
        else
        {
            CreateNode(keyvalue,node->rightNode);
        }
    }
}

BinaryTree::TreeNode* Griffis::BinaryTree::GetNode( int keyvalue )
{
    if (m_Root == NULL)
    {
        return NULL;
    } 
    else
    {
        return GetNode(keyvalue,m_Root);
    }
}

BinaryTree::TreeNode* Griffis::BinaryTree::GetNode( int keyvalue,BinaryTree::TreeNode* node )
{
    if (keyvalue == node->key)
    {
        return node;
    }
    else if (keyvalue < node->key)
    {
        if (node->leftNode == NULL)
        {
            return NULL;
        }
        else
        {
            return GetNode(keyvalue, node->leftNode);
        }
    } 
    else
    {
        if (node->rightNode == NULL)
        {
            return NULL;
        }
        else
        {
            return GetNode(keyvalue, node->rightNode);
        }
    }
}

void Griffis::BinaryTree::DestroyNode( BinaryTree::TreeNode* node )
{
    if (node->leftNode != NULL)
    {
        DestroyNode(node->leftNode);
    }
    if (node->rightNode != NULL)
    {
        DestroyNode(node->rightNode);
    }
    if (node != NULL)
    {
        delete node;
    }
}


