#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item { int weight; int val; };
using Items = vector<Item>;
using Table = vector<vector<int>>;
template<class C,typename = decltype(declval<C>().begin(), declval<C>().end(),void())>
ostream& operator<<(ostream& s, C& c)
{
	for_each(c.begin(), c.end(), [&s](auto& e) {
		s << "\t" << e;
	});
	s << endl;
	return s;
}

int knap(Table& t, Items& i, int k, int capacity)
{
	if (k < 0) return 0;
	if (t[k][capacity] == -1)
	{
			int without = knap(t, i, k - 1, capacity);
			int with = (capacity < i[k].weight) ? 0 : i[k].val + knap(t, i, k - 1, capacity - i[k].weight);
			t[k][capacity] = max(with, without);
	}
	return t[k][capacity];
}

int knap(Items& i, int capacity)
{
	Table t(i.size(), vector<int>(capacity + 1, -1));
	int val = knap(t, i, i.size() - 1, capacity);
	cout << t << endl << endl;
	return val;
}

int main()
{
	Items i{ Item{ 5,60 },Item{ 3,50 },Item{ 4,70 },Item{ 2,30 } };
	cout << knap(i, 5);
	int sdfgwert;
	cin >> sdfgwert;
    return 0;
}