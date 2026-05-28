#pragma once

#include <deque>

template<class X,class Y,class Z>
struct triple
{
	triple():key(),value(),isValid(){}
	triple(X x,Y y,Z z):key(x),value(y),isValid(z){}
	triple(X&& x,Y&& y,Z&& z):key(x),value(y),isValid(z){}
	X key;
	Y value;
	Z isValid;
};

template<class X,class Y,class Z>
triple<X,Y,Z> make_triple(X&& x,Y&& y,Z&& z){return triple<X,Y,Z>(x,y,z);}

class Node
{
public:
	typedef triple<int,int,bool> Value;
	typedef std::pair<Node*,bool> Child;
	Node()
	{
		values[0] = make_triple(static_cast<int>(0),static_cast<int>(0),bool(false));
		children[0] = std::make_pair(static_cast<Node*>(nullptr),bool(false));
		children[1] = std::make_pair(static_cast<Node*>(nullptr),bool(false));
	}
	std::deque<Value> values;
	std::deque<Child> children;
};

class Tree234
{
public:
	Tree234(void);
	~Tree234(void);
	int Insert(int val);
	int Search(int key, int& val);
private:
	int insert(Node*& root, int val);
	int search(Node*& root, int& val);
	int PrintLevels(void);
	Node* _root;
};

