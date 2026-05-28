#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>


namespace {
	using namespace std;
	using States = vector<int>;
	using Table = vector<vector<long long>>;

	//needed to disambiguate, limit to iterable containers, not just any C
	template< class C, typename = decltype(declval<C>().begin(), void()), typename = decltype(declval<C>().end(), void())>
	ostream& operator<<(ostream& s, C& states);
	ostream& operator<<(ostream& s, Table& t);
}

//////////////////////////////////////////////////////////////////////////////////
// the good stuff is in here
//////////////////////////////////////////////////////////////////////////////////

long long waystowin(States& states, Table& table, int numstates, int winthresh)
{
	if (numstates < 0) return 0;
	if (table[numstates][winthresh] == -1)
	{
		long long without = waystowin(states, table, numstates - 1, winthresh);
		long long with = (states[numstates] > winthresh) ? 1 : waystowin(states, table, numstates - 1, winthresh - states[numstates]);
		table[numstates][winthresh] = with + without;
	}
	return table[numstates][winthresh];
}

long long waystowin(States& states, stringstream& s)
{
	int thresh = std::accumulate(states.begin(), states.end(), 0) / 2;
	cout << "thresh: " << thresh << endl;
	Table t = Table(states.size(), vector<long long>(thresh + 1, -1));
	long long x = waystowin(states, t, states.size()-1, thresh);
	s << t;
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
	cout << s;
	stringstream ss;
	cout << "waystowin: " << waystowin(s,ss) << endl;
	cout << ss.str();
	return 0;
}


namespace {

	using namespace std;

	template<class C, typename, typename>
	ostream& operator<<(ostream& s, C& states)
	{
		for_each(states.begin(), states.end(), [&s](auto& state) {
			s << state << "\t";
		});
		s << endl;
		return s;
	}

	ostream& operator<<(ostream& s, Table& t)
	{
		//just print bottom half
		unsigned c=t.rend() - t.rbegin();
		for_each(t.rbegin(), t.rbegin() + ( t.rend() - t.rbegin() ) / 2, [&s,&c](auto& e) {
			s << --c << ":\t";
			s << e;
			s << endl;
		});
		s << endl;
		return s;
	}
}
