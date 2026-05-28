#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


namespace my
{
	template<class T>
	void countsort(vector<T>& v)
	{
		map<T, size_t> histo;
		for (T x : v) ++histo[x];
		map<T, size_t> offsets(histo);
		size_t offset = 0;
		for (const auto& curr : histo)
		{
			offsets[curr.first] = offset;
			offset += curr.second;
		}
		while (!offsets.empty())
		{
			auto from = offsets.begin(); //get an offset
			auto to = offsets.find(v[from->second]); //use that offset as index in container and find the associated entry in offsets
			swap(v[from->second], v[to->second]);
			if (--histo[to->first] > 0)
			{
				++to->second;
			}
			else offsets.erase(to);
		}
	}
}


int main()
{
	vector<int> v{ 5,6,7,6,6,5,4,5,6,6,6,5,3,4,2,3,2,3,8,7,8,6,7,6,5,4,5,3,2,9,8,7,2 };
	for (auto x : v)
		cout << x << " ";
	cout << endl;
	my::countsort(v);
	if (!is_sorted(v.begin(), v.end())) cout << "fail" << endl;
	for (auto x : v)
		cout << x << " ";
	int debugwait__;
	cin >> debugwait__;
	return 0;
}