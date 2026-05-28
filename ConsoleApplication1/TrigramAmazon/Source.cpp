#include <set>
#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;
using uint = unsigned int;
using SId = uint;
using PathFrag = string;
using ThreeClick = tuple < PathFrag, PathFrag, PathFrag >;
using Data = map < SId, vector<PathFrag> >;
using Result = multiset < ThreeClick >;

//this is the function I wrote up on the white board
void populate(const Data& data, Result& result);
//this is the function I didn't get to implement
void findTopRanked(const Result& r, uint count = 1);

int main()
{
	//simulate data read in from the file,
	//path frags ordered by timestamp, grouped by Session Id
	Data d;
	d[SId(1)] = vector<PathFrag>({ "B", "C", "A", "D" });
	d[SId(2)] = vector<PathFrag>({ "A", "B", "C", "D" });
	d[SId(3)] = vector<PathFrag>({ "A", "B", "C", "D", "E" });
	d[SId(4)] = vector<PathFrag>({ "A", "B", "C", "D", "E" });
	d[SId(5)] = vector<PathFrag>({ "B", "C", "D", "E" });
	d[SId(6)] = vector<PathFrag>({ "B", "C", "D", "E" });
	d[SId(7)] = vector<PathFrag>({ "B", "C", "A" });
	Result r;
	//this is the function I wrote up on the white board
	populate(d, r);
	//this is the function I didn't get to implement
	findTopRanked(r, 5);
	return 0;
}

void populate(const Data& data, Result& result)
{
	for (auto& datum : data)
	{
		const uint windowsize(tuple_size<Result::value_type>::value);
		if (datum.second.size() >= windowsize)
		{
			uint windowStart(0);
			while ((windowStart + windowsize) <= datum.second.size())
			{
				result.insert(make_tuple(
					datum.second[windowStart],
					datum.second[windowStart + 1],
					datum.second[windowStart + 2]
					));
				++windowStart;
			}
		}
	}
}

void findTopRanked(const Result& result, uint count)
{
	//create a map that relates freq to frag triplet, allow dupe frequencies
	multimap<uint, ThreeClick> ranking;
	for (Result::iterator clickTriple(result.begin());
	clickTriple != result.end();
		clickTriple = result.upper_bound(*clickTriple))
	{
		//don't use assignment; want to allow duplicate keys
		ranking.insert(make_pair(result.count(*clickTriple), *clickTriple));
	}
	//map sorts by key in ascending order, so iterate in reverse for high to low
	multimap<uint, ThreeClick>::const_reverse_iterator top_ranked(ranking.crbegin());
	for (uint clickTriple = 0; clickTriple < count; clickTriple++)
	{
		//get the triple, pretty-print path frags for each top ranked click-triple
		const ThreeClick& ranked((*top_ranked++).second);
		cout << get<0>(ranked) << "->"
			<< get<1>(ranked) << "->"
			<< get<2>(ranked) << "\n";
	}
}
