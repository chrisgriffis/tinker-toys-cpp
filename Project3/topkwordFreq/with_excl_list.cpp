#include <iostream>

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_set>

using namespace std;


stack<string> topk(string s, int k = 1, vector<string> excl = vector<string>{})
{
    stack<string> topkvals;
    stringstream ss(s);
    unordered_map<string, int> counts;
    unordered_set<string> excl_set{excl.begin(),excl.end()};

    string curr;
    while (ss >> curr)
    {
        if(excl_set.find(curr) == excl_set.end()) 
            ++counts[curr];
    }
    using Entry = pair<string, int>;
    auto comp = [](Entry& l, Entry& r) {return l.second > r.second; };
    priority_queue<Entry, vector<Entry>, decltype(comp)> minheap(comp);
    for (auto& entry : counts)
    {
        minheap.push(entry);
        if (minheap.size() > (k+0U)) minheap.pop();
    }
    while (!minheap.empty())
    {
        topkvals.emplace(minheap.top().first);
        minheap.pop();
    }
    return topkvals;
}

int main()
{
    string s("asdf asdf asdf asdf qwer asdf sdfg wert wert wert dfgh dfgh sdafg qwer asdf wert sdfg dfgh erty dfgh erty sdfg wrety qwer qwer sdfg asdf wert sdfg dfgh fghj tyui ghkj fghj fghj gfhkj cvbnm vbnm cvbn cvbn dfgh gfh j");
    auto v(topk(s, 3, vector<string>{"asdf"}));
    while (!v.empty()) cout << [&v] {auto x(v.top()); v.pop(); return x; }() << endl;
    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}