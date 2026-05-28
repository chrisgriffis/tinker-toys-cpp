#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <stdlib.h>
// #include <unordered_set>
#include <set>
#include <functional>
#include <limits>
#include <fstream>
using namespace std;

using Node = int;
struct Edge {
	int node;
	int weight;
};
struct test
{
	int nodecount;
	int edgecount;
	int source;
	vector<int> results;
	struct nodeMetadata
	{
		vector<Edge> edges;
		int costToHere;
		bool queued;
	};
	using Graph = map<Node, nodeMetadata>;
	Graph graph;
};

void dijkstra(test::Graph& g, Node source);


int main()
{
#ifndef FOO
#define INPUTSOURCE cin
#define OUTPUTSOURCE cout
#else
	std::ifstream input;
	std::ifstream output;
	input.open("input.txt");
	output.open("output.txt");
#define INPUTSOURCE input
#define OUTPUTSOURCE output
#endif
	int k;
	INPUTSOURCE >> k;
	vector<test> tests(k);
	for (auto& t : tests)
	{
		INPUTSOURCE >> t.nodecount;
		INPUTSOURCE >> t.edgecount;
		for (int i = 0; i < t.edgecount; i++)
		{
			int n1, n2, n3;
			INPUTSOURCE >> n1 >> n2 >> n3;

			t.graph[n1].edges.emplace_back(Edge{ n2,n3 });
			t.graph[n1].costToHere = -1;
			t.graph[n1].queued = false;
			t.graph[n2].edges.emplace_back(Edge{ n1,n3 });
			t.graph[n2].costToHere = -1;
			t.graph[n2].queued = false;
		}
		INPUTSOURCE >> t.source;
		t.graph[t.source].costToHere = 0;
	}

	for (auto t : tests)
	{
		dijkstra(t.graph, t.source);
		for (Node node = 1; node <= t.nodecount; node++)
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
#ifndef FOO
		for (int i : t.results)
			cout << i << " ";
		cout << endl;
#else
		bool pass = true;
		for (int i : t.results)
		{
			int x;
			OUTPUTSOURCE >> x;
			pass = pass && (i == x);
		}
		cout << "pass: " << boolalpha << pass << endl;
#endif
	}
	return 0;
}

void dijkstra(test::Graph& g, Node source)
{
	if (g.find(source) != g.end())
	{
		queue<Node> bfs;
		bfs.emplace(source);
		g[source].queued = true;
		while (!bfs.empty())
		{
			auto currentNode = move(bfs.front());
			bfs.pop();
			//populate bfs queue
			for (auto& edge : g[currentNode].edges)
			{
				//if not seen yet
				if (!g[edge.node].queued)
				{
					//enqueue, mark,and initialize
					bfs.emplace(edge.node);
					g[edge.node].queued = true;
					g[edge.node].costToHere = std::numeric_limits<int>::max();
				}
				//update costs for edges
				if (((g[currentNode].costToHere + edge.weight) < g[edge.node].costToHere))
				{
					g[edge.node].costToHere = g[currentNode].costToHere + edge.weight;
					//cost to a neighbor decreased, need to cascade to its edges
					bfs.emplace(edge.node);
				}
			}
		}
	}

}

