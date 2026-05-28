#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


using Table = vector<vector<int>>;
using Items = vector<pair<int, int>>; //xdim,ydim

unsigned knap(const Items& i, Table& t, int remain_gap, int k)
{
	if (k<0) return 0;
	if (t[k][remain_gap] == -1)
	{
// 		int without = knap(i, t, remain_gap, k - 1);
// 		const int area = i[k].first * i[k].second;
// 		int with = (area > remain_gap) ? 0 : (1 + knap(i, t, remain_gap - area, k - 1));
// 		t[k][remain_gap] = max(with, without);
	}
	return t[k][remain_gap];
}

unsigned knap(const Items& i, int capacity)
{
	//table lookup gives you a value for combos and capacity
	Table t(i.size(), vector<int>(capacity + 1, -1));
	return knap(i, t, capacity, i.size() - 1);
}

int main()
{
	Items coll = {
		{ 50,23 },
		{ 20,19 },
		{ 25,13 },
		{ 30,10 },
		{ 11,3 },
		{ 12,3 },
		{ 13,3 },
		{ 14,3 },
	};

	cout << "knap(i, 160): " << knap(coll, 160) << endl;
	cout << "knap(i, 140): " << knap(coll, 140) << endl;
	cout << "knap(i, 120): " << knap(coll, 120) << endl;
	cout << "knap(i, 100): " << knap(coll, 100) << endl;
	cout << "knap(i, 113): " << knap(coll, 113) << endl;
	int debugwait__;
	cin >> debugwait__;
	return 0;
}