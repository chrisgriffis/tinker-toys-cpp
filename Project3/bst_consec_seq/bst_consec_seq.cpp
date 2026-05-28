#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct node
{
    int val;
    node* left;
    node* right;
};


struct md
{
    md() :count(1), pred(nullptr) {}
    int count;
    node* pred;
};
void traverse(unordered_map<node*, md>& table, node* root, node*& max_node)
{
    if (root == nullptr) return;
    if (table[root].count > table[max_node].count)
        max_node = root;
    if (root->left && (root->left->val == root->val + 1))
    {
        table[root->left].count = 1 + table[root].count;
        table[root->left].pred = root;
    }
    if (root->right && (root->right->val == root->val + 1))
    {
        table[root->right].count = 1 + table[root].count;
        table[root->right].pred = root;
    }
    traverse(table, root->left, max_node);
    traverse(table, root->right, max_node);
}
vector<int> find_bst_seq(node* root)
{
    vector<int> ret;
    unordered_map<node*, md> table;
    node* max_node(root);
    traverse(table, root, max_node);
    while (max_node)
    {
        ret.emplace_back(max_node->val);
        max_node = table[max_node].pred;
    }
    reverse(ret.begin(),ret.end());
    return ret;
}

int main()
{
    
    node* root = 
        new node{ 3,
        new node{ 4,
        new node{ 5,
        nullptr,nullptr },
        new node{ 2,
        nullptr,nullptr } },
        new node{ 6,
        new node{ 7,
        nullptr,nullptr },nullptr } };
    auto v = find_bst_seq(root);
	int sdfgwert;
	cin >> sdfgwert;
    return 0;
}