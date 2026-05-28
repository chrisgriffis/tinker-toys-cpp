#include <functional>
#include <vector>
#include <iostream>


struct binarytree 
{
	struct node
	{
		int val;
		node* left;
		node* right;
	} *m_root;

	node* insert(int);
	//successor iterator???
	node* lca(int v1, int v2);
};

binarytree::node* binarytree::insert(int v)
{
	std::function<binarytree::node* (binarytree::node*&,int)> 
		inserthelper = [&inserthelper](binarytree::node*& root, int v)->binarytree::node*
	{
		if (root != nullptr)
		{
			return ((v < root->val) ? 
				inserthelper(root->left, v) : 
				inserthelper(root->right, v));
		}
		else
		{
			return root = new node{ v,nullptr,nullptr };
		}
	};
	return inserthelper(m_root, v);
}

binarytree::node* 
binarytree::lca(int v1, int v2)
{
	struct result
	{
		binarytree::node* thelca;
		int count;
	};
	std::function<result (binarytree::node*, int , int)> 
		lcahelper = [&lcahelper](binarytree::node* root, int v1, int v2)-> result
	{
		if (root == nullptr) return result{ nullptr,0 };
		if (root->val == v1 || root->val == v2) return result{root,1};
		result left = lcahelper(root->left, v1, v2);
		result right = lcahelper(root->right, v1, v2);
		if (left.count == 1 && right.count == 1) return result{ root,2 };
		else if (left.count > 0) return left;
		else if (right.count > 0) return right;
		else return result{ nullptr,0 };
	};
	return lcahelper(m_root, v1, v2).thelca;
}

int main()
{
	binarytree b{ nullptr };
	std::vector<int> v{ 4,2,7,1,3,6 };
	for (auto e : v) b.insert(e);
	std::cout << b.lca(1, 7)->val;
}
