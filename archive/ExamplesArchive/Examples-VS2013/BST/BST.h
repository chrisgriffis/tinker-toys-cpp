//In place conversion of a Binary Search Tree into a Doubly Linked List 
#include <queue>

struct Node
{
	static void DLLConnect(Node*& left, Node*& right);
	Node(int val):_val(val),leftreverse(0),rightforward(0){}
	Node(int val,Node* lr,Node* rf):_val(val),leftreverse(lr),rightforward(rf){}

	int _val;
	Node* leftreverse;
	Node* rightforward;
};

class BST
{
	Node* _root;
public:
	BST():_root(0){}
	BST(int val){_root=new Node(val);}
	template<size_t N>
	BST( int (&v)[N]);
	BST(const std::vector<int>& v);
	void Insert(int val);
	void LevelPrint();
	void DoublyLinkedListPrint();
	Node& ToDoublyLinkedListInPlace();
private:
	void insert(Node*& root,int val);
	void levelPrint(std::queue<Node*>& current, std::queue<Node*>& other);
	void flattenSubtree(Node* root, Node*& farthestLeftNode, Node*& farthestRightNode);
};

template<size_t N>
BST::BST( int (&v)[N]):
	_root(NULL)
{
	for (unsigned int i = 0; i < N ; i++)
	{
		Insert(v[i]);
	}
}
