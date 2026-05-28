//In place conversion of a Binary Search Tree into a Dynamically Linked List 

#include <queue>
#include <iostream>
#include <cstdlib>
using namespace std;

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


int main()
{
    int vals[] = {8,5,12,3,7,10,15,1,2,4,6,9,11,13,16,14};
    BST tree(vals);
    tree.LevelPrint();
    tree.ToDoublyLinkedListInPlace();
    tree.DoublyLinkedListPrint();
    std::cout << "\n\n";

    srand(1377);
    std::vector<int> vals2;
    for (int i = 0; i < 20 ; i++)
    {
        vals2.push_back(rand()%1000);
    }
    BST tree2(vals2);
    tree2.LevelPrint();
    tree2.ToDoublyLinkedListInPlace();
    tree2.DoublyLinkedListPrint();

    srand(2344);
    std::vector<int> vals3;
    for (int i = 0; i < 20 ; i++)
    {
        vals3.push_back(rand()%1000);
    }
    BST tree3(vals3);
    tree3.LevelPrint();
    tree3.ToDoublyLinkedListInPlace();
    tree3.DoublyLinkedListPrint();

    return 0;
}

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
    flattenSubtree(_root->leftreverse,leftend,rightend);
    Node::DLLConnect(rightend,_root);
    Node* newroot = leftend;
    flattenSubtree(_root->rightforward,leftend,rightend);
    Node::DLLConnect(_root,leftend);
    _root = newroot;

    return *_root;
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
            flattenSubtree(root->leftreverse, leftend, rightend);
            farthestLeftNode = leftend;
            Node::DLLConnect(rightend,root);
            flattenSubtree(root->rightforward,leftend,rightend);
            farthestRightNode = rightend;
            Node::DLLConnect(root,leftend);
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
    using namespace std;
    while(!current.empty())
    {
        Node*& n = current.front();
        current.pop();
        if(NULL != n)
        {
            cout << n->_val << " ";
            other.push(n->leftreverse);
            other.push(n->rightforward);
        }
    }    
    cout << "\n";
    //swap the queues by calling in reverse order
    if(!other.empty()) levelPrint(other,current);
}

void BST::LevelPrint()
{
    using namespace std;
    if(_root)
    {
        //stack allocate some queues
        queue<Node*> current;
        queue<Node*> other;
        //initialize the current queue
        current.push(_root);
        levelPrint(current,other);
        //queues fall off stack; no mem mgmt needed  
    }
}

template<size_t N>
BST::BST( int (&v)[N]):
_root(NULL)
{
    for (unsigned int i = 0; i < N ; i++)
    {
        Insert(v[i]);
    }
}

void BST::DoublyLinkedListPrint()
{
    Node* current = _root;
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
    std::vector<int>::const_iterator val = v.begin();
    while(val != v.end())
    {
        Insert(*val++);
    }
}
