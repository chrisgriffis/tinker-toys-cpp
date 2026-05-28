#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <unordered_set>
#include <functional>
#include <set>
#include <algorithm>

using namespace std;

template<class T>
struct node
{
    node() :val(), pred(nullptr), cost(numeric_limits<int>::max()) {}
    T val;
    node* pred;
    int cost;
    vector<pair<int, node*>> edges;
};

using inode = node<int>;
stack<inode*> toposort(inode* root);

int main()
{
    int arr[][5] = {
        { 1, 2, 10, 2, 2 },
        { 5, 2, 10, 2, 2 },
        { 5, 2, 1, 7, 1 },
        { 5, 5, 1, 1, 1 },
        { 5, 5, 1, 5, 2 },
    };

    vector<inode> G(25);
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            inode& curr = G[row * 5 + col];
            curr.val = row * 5 + col;
            if (row + 1 < 5) curr.edges.push_back(make_pair(arr[row + 1][col], &G[(row + 1) * 5 + col]));
            if (col + 1 < 5) curr.edges.push_back(make_pair(arr[row][col + 1], &G[row * 5 + col + 1]));
        }
    }
    G[0].edges.clear();
    G[0].edges.push_back(make_pair(0, &G[5]));
    G[9].edges.clear();
    G[9].edges.push_back(make_pair(0, &G[1]));
    //////////////////////////////////////////////////////////////////////////
    auto s = toposort(&G[0]);
    //////////////////////////////////////////////////////////////////////////
    while (!s.empty())
    {
        cout << "(" << s.top()->val / 5 << "," << s.top()->val % 5 << ")" << endl;
        s.pop();
    }
    return 0;
}

std::stack<inode*> toposort(inode* root)
{
    std::unordered_set<inode*> visited;
    std::stack<inode*> ordering;
    std::function<void(inode*)> DFS = [&](inode* curr) {
        visited.emplace(curr);
        for (auto& e : curr->edges)
        {
            if (visited.find(e.second) == visited.end())
            {
                DFS(e.second);
            }
        }
        ordering.emplace(curr);
    };
    DFS(root);
    return ordering;
}
