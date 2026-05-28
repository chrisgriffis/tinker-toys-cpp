#include <iostream>
#include <queue>
#include <sstream>
#include <iterator>
#include <utility>
#include <functional>
#include <algorithm>

using namespace std;


template<class T, template<class...> class It, class... Args>
vector<T> merge(vector<It<T, Args...>>& vec)
{
	vector<T> ret;
	std::priority_queue<pair<T, size_t>, vector<pair<T, size_t>>, greater<pair<T, size_t>>> minheap;
	for (size_t index = 0; index < vec.size(); ++index)
		if (vec[index] != It<T, Args...>())
			minheap.push(make_pair(*vec[index]++, index));
	while (!minheap.empty())
	{
		const size_t next = minheap.top().second;
		ret.push_back(minheap.top().first);
		minheap.pop();
		if (vec[next] != It<T, Args...>())
			minheap.push(make_pair(*vec[next]++, next));
	}
	return ret;
}

int main()
{
	istringstream s1("2 5 7 8 10");
	istringstream s2("4 6 9 33");
	istringstream s3("0 1 7 12 15 80");
	istringstream s4("2 4 6 7 9 18 20 23 98");
	cout << s1.str() << endl;
	cout << s2.str() << endl;
	cout << s3.str() << endl;
	cout << s4.str() << endl;

	vector<istream_iterator<int>>
		sources{
		istream_iterator<int>(s1),
		istream_iterator<int>(s2),
		istream_iterator<int>(s3),
		istream_iterator<int>(s4) };

	auto x = merge(sources);
	if (!is_sorted(x.begin(), x.end())) cout << "fail";
	else for (int v : x)
	{
		cout << v << " ";
	}

	cout << endl;
	int debugwait__;
	cin >> debugwait__;
	return 0;
}