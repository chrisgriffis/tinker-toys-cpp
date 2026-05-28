#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <queue>
#include <functional>
using namespace std;

struct livemedian
{
	livemedian()
		//         :
		//         min_heap([](int l, int r){
		//         return l > r;
		//     }),
		//         max_heap([](int l, int r) {
		//         return l < r;
		//     })
	{}

	int add(int val)
	{
		order.emplace_back(val);
		if (max_heap.empty())
		{
			max_heap.push(val);
			return val;
		}
		else if (val > max_heap.top())
		{
			min_heap.push(val);
		}
		else
		{
			max_heap.push(val);
		}
		return rebalance();
	}

	vector<int> order;
private:
	int rebalance()
	{
		while (min_heap.size() > max_heap.size())
		{
			max_heap.push(min_heap.top());
			min_heap.pop();
		}
		while (max_heap.size() > (min_heap.size() + 1))
		{
			min_heap.push(max_heap.top());
			max_heap.pop();
		}
		return max_heap.top();
	}

	//     std::priority_queue<int, std::vector<int>, std::function<bool(int, int)>> min_heap;
	//     std::priority_queue<int, std::vector<int>, std::function<bool(int, int)>> max_heap;
	std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
	std::priority_queue<int> max_heap;
};

int main()
{
	livemedian lm;
	for (int i = 0; i < 30; i++)
	{
		int val = (random_device{}() % 100) - 50;
		cout << val << "; new median: ";
		cout << lm.add(val);
		cout << endl;
		sort(lm.order.begin(), lm.order.end());
		for (auto i : lm.order)cout << i << " ";
		cout << endl;
	}
}