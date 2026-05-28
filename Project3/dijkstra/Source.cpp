#include <iostream>
#include <vector>
#include <set>

using namespace std;

struct node
{
	node() :val(0), cost(std::numeric_limits<int>::max()), pred(nullptr) {}
	int row;
	int col;
	int val;
	int cost;
	node* pred;
	vector<node*> edges;
};

vector<node*> dijkstra(node* start, node* end)
{
	vector<node*> ret;
	start->cost = 0;
	// 	start->pred = nullptr; //done by node constructor
	auto cmp = [](const node* l, const node* r) {
		return l->cost < r->cost || (l->cost == r->cost && l < r);
	};
	std::set<node*, decltype(cmp)> unvisited(cmp);
	unvisited.insert(start);
	while (!unvisited.empty())
	{
		node* curr = *unvisited.begin();
		if (curr == end) break;
		unvisited.erase(curr);
		for (auto& e : curr->edges)
		{
			if (curr->cost + e->val < e->cost)
			{
				unvisited.erase(e);
				e->cost = curr->cost + e->val;
				e->pred = curr;
				unvisited.insert(e);
			}
		}
	}
	for (node* n = end; n != start; n = n->pred)
		ret.push_back(n);
	reverse(ret.begin(), ret.end());
	return ret;
}

int main()
{
	int arr[][5] = {
		{ 1, 2, 9, 2, 2 },
		{ 5, 1, 9, 2, 2 },
		{ 5, 2, 1, 7, 1 },
		{ 5, 5, 1, 1, 1 },
		{ 5, 5, 1, 5, 2 },
	};

	//make adj mat
	vector<node> G(25);
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			node& curr = G[row * 5 + col];
			curr.val = arr[row][col];
			curr.row = row;
			curr.col = col;
			if (row + 1 < 5) curr.edges.push_back(&G[(row + 1) * 5 + col]);
			if (row - 1 >= 0) curr.edges.push_back(&G[(row - 1) * 5 + col]);
			if (col + 1 < 5) curr.edges.push_back(&G[row * 5 + col + 1]);
			if (col - 1 >= 0) curr.edges.push_back(&G[row * 5 + col - 1]);
		}
	}

	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			cout << arr[row][col] << " ";
			arr[row][col] = 0;
		}
		cout << endl;
	}
	cout << endl << endl;
	//////////////////////////////////////////////////////////////////////////
	auto s = dijkstra(&G[0], &G[4]);
	//////////////////////////////////////////////////////////////////////////
	int counter = 0;
	for (auto& n : s)
		arr[n->row][n->col] = ++counter;
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			cout << arr[row][col] << " ";
		}
		cout << endl;
	}
	int debugwait__;
	cin >> debugwait__;
	return 0;
}