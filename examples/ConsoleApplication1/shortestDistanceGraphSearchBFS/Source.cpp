#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <stdlib.h>
#include <unordered_set>
using namespace std;

struct test
{
	int nodecount;
	int edgecount;
	int source;
	vector<int> results;
	struct nodeMetadata
	{
		unordered_set<int> neighbors;
		bool queued;
		int costToHere;
	};
	using Graph = map<int, nodeMetadata>;
	Graph graph;
};

void calcShortestDistance(test::Graph& g, int source);


int main()
{
	int k;
	cin >> k;
	vector<test> tests(k);
	for (auto& t : tests)
	{
		cin >> t.nodecount;
		cin >> t.edgecount;
		for (int i = 0; i < t.edgecount; i++)
		{
			int n1, n2;
			cin >> n1 >> n2;

			t.graph[n1].neighbors.emplace(n2);
			t.graph[n1].queued = false;
			t.graph[n1].costToHere = -1;
			t.graph[n2].neighbors.emplace(n1);
			t.graph[n2].queued = false;
			t.graph[n2].costToHere = -1;
		}
		cin >> t.source;
	}

	for (auto t : tests)
	{
		calcShortestDistance(t.graph, t.source);
		for (int node = 1; node <= t.nodecount; node++)
		{
			if (t.graph.find(node) == t.graph.end())
			{
				t.results.emplace_back(-1);
			}
			else if (node != t.source)
			{
				t.results.emplace_back(t.graph[node].costToHere);
			}
		}
		for (int i : t.results)
			cout << i << " ";
		cout << endl;
	}
	return 0;
}

void calcShortestDistance(test::Graph& g, int source)
{
	if (g.find(source) != g.end())
	{
		using ReachedNode = pair<int, int>;
		queue<ReachedNode> bfs;
		bfs.emplace(ReachedNode{ source,0 });
		while (!bfs.empty())
		{
			auto traversalHead = move(bfs.front());
			bfs.pop();
			//determine min cost to get here
			g[traversalHead.first].costToHere = traversalHead.second;
			//add unvisited neighbors to queue
			for (auto adjNode : g[traversalHead.first].neighbors)
			{
				//if unvisited or cheaper route
				if (!g[adjNode].queued || ((traversalHead.second + 6) < g[adjNode].costToHere))
				{
					g[adjNode].queued = true;
					bfs.emplace(ReachedNode{ adjNode , g[traversalHead.first].costToHere + 6 });
				}
			}
		}
	}

}

