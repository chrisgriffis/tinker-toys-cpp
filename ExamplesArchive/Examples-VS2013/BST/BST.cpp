//In place conversion of a Binary Search Tree into a Doubly Linked List 

#include "BST.h"
#include <iostream>
#include <future>


void Node::DLLConnect( Node*& left, Node*& right )
{
	if (left == right)
	{
		return;
	} 
	else if (left != NULL && right != NULL)
	{
		left->rightforward = right;
		right->leftreverse = left;
	}
}

Node& BST::ToDoublyLinkedListInPlace()
{
	Node* leftend(NULL);
	Node* rightend(NULL);
	Node* leftend2(NULL);
	Node* rightend2(NULL);
	Node* newroot(NULL);
	auto t1 = std::async([&]{
		flattenSubtree(_root->leftreverse,leftend,rightend); 
		Node::DLLConnect(rightend,_root);
		newroot = leftend;
	}); 
	flattenSubtree(_root->rightforward,leftend2,rightend2);
	Node::DLLConnect(_root,leftend2);
	t1.get();
	return *(_root = newroot);
}

void BST::flattenSubtree( Node* root, Node*& farthestLeftNode, Node*& farthestRightNode )
{
	if (root)
	{
		Node* leftend(root);
		Node* rightend(root);
		if(root->rightforward == NULL && root->leftreverse == NULL)
		{
			farthestLeftNode = root;
			farthestRightNode = root;
			return;
		}
		else if(root->leftreverse != NULL && root->rightforward == NULL)
		{
			flattenSubtree(root->leftreverse, leftend, rightend);
			Node::DLLConnect(rightend,root);
			farthestLeftNode = leftend;
			farthestRightNode = root;
		}
		else if(root->rightforward != NULL && root->leftreverse == NULL)
		{
			flattenSubtree(root->rightforward, leftend, rightend);
			Node::DLLConnect(root,leftend);
			farthestRightNode = rightend;
			farthestLeftNode = root;
		}
		else
		{
			Node* leftend2(root);
			Node* rightend2(root);
			auto t1 = std::async([&]{
				flattenSubtree(root->leftreverse, leftend, rightend);
				farthestLeftNode = leftend;
			});
			flattenSubtree(root->rightforward,leftend2,rightend2);
			farthestRightNode = rightend2;
			t1.get();
			Node::DLLConnect(rightend,root);
			Node::DLLConnect(root,leftend2);
		}

	}
}

void BST::Insert( int val )
{
	insert(_root,val);
}

void BST::insert( Node*& root,int val )
{
	if (root)
	{
		if (val < root->_val)
		{
			insert(root->leftreverse,val);
		} 
		else
		{
			insert(root->rightforward,val);
		}
	} 
	else
	{
		root = new Node(val);
	}
}

void BST::levelPrint( std::queue<Node*>& current, std::queue<Node*>& other )
{
	while(!current.empty())
	{
		Node*& n = current.front();
		current.pop();
		if(NULL != n)
		{
			std::cout << n->_val << " ";
			other.push(n->leftreverse);
			other.push(n->rightforward);
		}
	}    
	std::cout << "\n";
	//swap the queues by calling in reverse order
	if(!other.empty()) levelPrint(other,current);
}

void BST::LevelPrint()
{
	if(_root)
	{
		//stack allocate some queues
		std::queue<Node*> current;
		std::queue<Node*> other;
		//initialize the current queue
		current.push(_root);
		std::cout << "Tree Levels\n";
		levelPrint(current,other);
		//queues fall off stack; no mem mgmt needed  
	}
}


void BST::DoublyLinkedListPrint()
{
	Node* current = _root;
	std::cout << "Doubly Linked List\n";
	while (current)
	{
		std::cout << current->_val << " ";
		current = current->rightforward;
	}
	std::cout << std::endl;

}

BST::BST( const std::vector<int>& v ):
	_root(NULL)
{
	for(auto& val : v)
	{
		Insert(val);
	}
}
