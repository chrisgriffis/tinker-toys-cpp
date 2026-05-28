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
		bool processed;
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

			t.graph[n1].edges.emplace_back(Edge{ n2,n3});
			t.graph[n1].costToHere = -1;
			t.graph[n1].processed = false;
			t.graph[n2].edges.emplace_back(Edge{ n1,n3});
			t.graph[n2].costToHere = -1;
			t.graph[n2].processed = false;
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
		int processed(0);
		using Snapshot = pair<int, int>;
		priority_queue<Snapshot, vector<Snapshot>, std::greater<Snapshot>> min_heap;
		min_heap.emplace(Snapshot{ 0,source });
		while (!min_heap.empty())
		{
			auto currentNode = move(min_heap.top().second);
			min_heap.pop();
			if (g[currentNode].processed) continue;
			//populate bfs queue
			for (auto& edge : g[currentNode].edges)
			{
				//update costs for edges
				if (
					(g[edge.node].costToHere < 0) || 
					(
						(g[currentNode].costToHere + edge.weight) < 
						g[edge.node].costToHere)
					)
				{
					g[edge.node].costToHere = 
						g[currentNode].costToHere + edge.weight;
				}
				if (!g[edge.node].processed) 
					min_heap.emplace(
						Snapshot{ g[edge.node].costToHere,edge.node }
				);
			}
			g[currentNode].processed = true;
			if (++processed == g.size())return;
		}
	}

}

