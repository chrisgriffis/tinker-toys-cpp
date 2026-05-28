#include <unordered_map>
#include <iostream>
#include <string>
using namespace std;


struct Trie
{
	Trie() :_root(new Node(Node{ '.',0,0,nullptr,Node::Children{} })) {}
	struct Node
	{
		char c;
		int occurenceCount;
		int childcount;
		Node* parent;
		using Children = unordered_map<char, Node*>;
		Children children;
	};
	void insert(string s);
	int lookup(string s);
private:
	Node* _root;
};

int main()
{
	Trie t;
	int k;
	cin >> k;
	for (int i = 0; i < k; i++)
	{
		string s;
		cin >> s;
		if (s == "add")
		{
			cin >> s;
			t.insert(s);
		}
		else if (s == "find")
		{
			cin >> s;
			cout << t.lookup(s) << endl;
		}
		else throw;
	}
	// 	for (int i = 0; i < k; i++)
	// 	{
	// 		string s;
	// 		cin >> s;
	// 		t.insert(s);
	// 	}
	// 	string s("x");
	// 	while (s != string("exit"))
	// 	{
	// 		cin >> s;
	// 		cout << s << ": " << t.lookup(s) << endl;
	// 	}
}

void Trie::insert(string s)
{
	Node* current = _root;
	for (char c : s)
	{
		auto f = current->children.find(c);
		if (f == current->children.end())
		{
			current->children[c] = new Node(Node{ c,0,0,current,Node::Children{} });
			current = current->children[c];
		}
		else
		{
			current = f->second;
		}
	}
	++current->occurenceCount;
	current = current->parent;
	while (current)
	{
		++current->childcount;
		current = current->parent;
	}
}

int Trie::lookup(string s)
{
	Node* current = _root;
	for (char c : s)
	{
		auto f = current->children.find(c);
		if (f == current->children.end())
		{
			return 0;
		}
		else
		{
			current = f->second;
		}
	}
	return current->occurenceCount + current->childcount;
}

