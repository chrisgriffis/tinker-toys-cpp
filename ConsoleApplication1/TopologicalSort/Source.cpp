#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <algorithm>
#include <stack>
#include <unordered_map>
using namespace std;

struct test
{
	int nodecount;
	int edgecount;
	stack<int> results;
	struct nodeMetadata
	{
		vector<int> edges;
		bool tempmark;
		bool permamark;
	};

	//this is the edgelist with metadata i explained
	using Graph = unordered_map<int, nodeMetadata>;
	Graph graph;
};

stack<int> DFSTopoSort(test::Graph& g)
{
	stack<int> retval;
	//recursive lambda, needs itself as a capture and also a fully specified typedecl
	std::function<void (test::Graph&,int)> visit = 
		[&visit,&retval](test::Graph& g, int node)->void
	{
		auto& ndata = g[node];
		if (ndata.tempmark)throw;
		if (!ndata.permamark)
		{
			ndata.tempmark = true;
			for (auto n : ndata.edges)
			{
				visit(g,n);
			}
			ndata.permamark = true;
			ndata.tempmark = false;
			retval.emplace(node); //push, but with move semantics
		}
	};

	//could have been improved with a "do-while" but this is what i had on whiteboard
	//find_if is an STL algorithm, i just created a lambda predicate as its search condition
	auto it = find_if(g.begin(), g.end(), [&g](auto& node) {
		return node.second.permamark == false;
	});
	while (it != g.end())
	{
		//visit gets called here
		visit(g,it->first);
		it = find_if(g.begin(), g.end(), [&g](auto& node) {
			return node.second.permamark == false;
		});
	}
	return retval;
}


int main()
{
#ifndef FOO
#define INPUTSOURCE cin
#define OUTPUT cout
#else
	std::ifstream input;
	std::ofstream output;
	input.open("input.txt");
	output.open("output.txt");
#define INPUTSOURCE input
#define OUTPUT output
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
			int n1, n2;
			INPUTSOURCE >> n1 >> n2;

			t.graph[n1].edges.emplace_back(n2);
			t.graph[n1].tempmark = false;
			t.graph[n1].permamark = false;
		}
	}

	for (auto t : tests)
	{
		t.results = DFSTopoSort(t.graph);
		while (!t.results.empty())
		{
			OUTPUT << t.results.top() << " ";
			t.results.pop();
		}
		OUTPUT << endl;
	}
	return 0;
}

