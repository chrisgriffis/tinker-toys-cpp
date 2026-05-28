#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <iterator>
#include <string>
#include <set>
#include <map>
#include <fstream>
using namespace std;


string scrub(const string& s)
{
	string out;
	for (auto ch : s)
	{
		if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A'))
			out += ch;
	}
	return out;
}

int main()
{
	ifstream input;
	input.open("input.txt");
	string first, second, third;
	using Trigram = tuple<string, string, string>;
	multiset<Trigram> bag;
	multimap<int, Trigram> histo;

	input >> first; first = scrub(first);
	input >> second; second = scrub(second);
	while (input >> third)
	{
		third = scrub(third);
		bag.emplace(Trigram{ first, second, third });
		first = move(second);
		second = move(third);
	}
	for (auto i = bag.begin(); i != bag.end(); i = bag.upper_bound(*i))
	{
		histo.emplace(make_pair(bag.count(*i), *i));
	}
	Trigram t = histo.rbegin()->second;
	cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t);
	//"around the world"
	return 0;
}
