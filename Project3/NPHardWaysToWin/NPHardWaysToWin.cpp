#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>


namespace {
	using namespace std;
	using States = vector<int>;
	using Table = vector<vector<long long>>;

	//needed to disambiguate, limit to iterable containers, not just any C
	template< class C, typename = decltype(declval<C>().begin(), declval<C>().end(), void())>
	ostream& operator<<(ostream&, C&);
}

//////////////////////////////////////////////////////////////////////////////////
// the good stuff is in here
//////////////////////////////////////////////////////////////////////////////////

long long waystowin(const States& points, Table& table, int state_index, const int score, const int thresh)
{
	if (state_index < 0) return 0;
	if (table[state_index][score] == -1)
	{
		long long without = waystowin(points, table, state_index - 1, score, thresh);
		long long with =
			waystowin(points, table, state_index - 1, score + points[state_index], thresh)
			+ ((score + points[state_index]) > thresh ? 1 : 0);
		table[state_index][score] = with + without;
	}
	return table[state_index][score];
}

long long waystowin(const States& states)
{
	const int maxscore = std::accumulate(states.begin(), states.end(), 0);
	const int thresh = maxscore / 2;
	cout << "thresh: " << thresh << endl;
	Table t = Table(states.size(), vector<long long>(maxscore + 1, -1));
	long long x = waystowin(states, t, states.size() - 1, 0, thresh);
	return x;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


int main()
{
	//electoral values for Alabama through Wyoming and DC
	States s{
		9, 3, 11, 6, 55, 9, 7, 3, 29, 16, 4, 4, 20, 11, 6, 6, 8,
		8, 4, 10, 11, 16, 10, 6, 10, 3, 5, 6, 4, 14, 5, 29, 15, 3,
		18, 7, 7, 20, 4, 9, 3, 11, 38, 6, 3, 13, 12, 5, 10, 3, 3
	};
	States sx{};
	for_each(s.begin(), s.end(), [&s, &sx](int v)
	{
		sx.emplace_back(v);
		cout << sx;
		cout << "waystowin: " << waystowin(sx) << endl << endl;
	});
	return 0;
}


namespace {

	using namespace std;

	template<class C, typename>
	ostream& operator<<(ostream& s, C& c)
	{
		for_each(c.begin(), c.end(), [&s](auto& e) {
			s << "\t" << e;
		});
		s << endl;
		return s;
	}
}

// vector<long> cache(amount + 1, 0);
// cache[0] = 1L;
// for (int coin : coins)
// {
// 	for (long index = coin; index < cache.size(); index++)
// 		cache[index] += cache[index - coin];
// 	for (auto x : cache) cout << x << " "; cout << endl;
// }
// return cache[amount];
