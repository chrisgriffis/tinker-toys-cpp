#include "Tree234.h"

int main(){}

Tree234::Tree234(void):_root(nullptr)
{
}


Tree234::~Tree234(void)
{
}


int Tree234::Insert(int val)
{
	if (nullptr == _root)
	{
		_root = new Node();
		_root->values[0].key = val;
		_root->values[0].value = val;
		_root->values[0].isValid = true;
	}
	else
	{
		insert(_root,val);
	}
	return 0;
}


int Tree234::Search(int key, int& val)
{
	return 0;
}


int Tree234::insert(Node*& root, int val)
{
	if (nullptr == root	)
	{
		return -1;
	} 
	else
	{
		if (root->values[0].isValid && val < root->values[0].key)
		{
			auto key = root->values[0].key;
			root->values.push_front(Node::Value(key,key,true));

		} 
		else
		{
			auto key = root->values[0].key;
			root->values.push_back(Node::Value(key,key,true));
		}
	}
	return 0;
}


int Tree234::search(Node*& root, int& val)
{
	return 0;
}


int Tree234::PrintLevels(void)
{
	return 0;
}
