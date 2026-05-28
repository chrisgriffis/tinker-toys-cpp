#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <array>
#include <vector>
#include <memory>
#include <type_traits>


using namespace std;

template<int... Tables>
class reservation_day
{

public:
	array<set<int>, 24 * 4> m_unreserved;
	reservation_day() :
		m_unreserved() 
	{
		for (auto& timeslot : m_unreserved)
			timeslot = move(set<int>{Tables...});
	}

	int reserve(int startindex, int duration = 4)
	{
		int endindex = startindex + duration;
		auto avail(all_available(startindex, duration));
		if (avail.size() < 1) return -1;
		int tableid = avail[0];
		for_each(
			next(m_unreserved.begin(), startindex),
			next(m_unreserved.begin(), endindex),
			[=](std::set<int>& curr)
		{
			curr.erase(tableid);
		});
		return tableid;
	}

	int unreserve(int tableid, int startindex, int duration = 4)
	{
		int endindex = startindex + duration;
		if (any_of(
			next(m_unreserved.cbegin(), startindex),
			next(m_unreserved.cbegin(), endindex),
			[=](const std::set<int>& curr)
			{
				return curr.count(tableid) > 0;
			})
		) 
			return -1;

		for_each(
			next(m_unreserved.begin(), startindex),
			next(m_unreserved.begin(), endindex),
			[=](std::set<int>& curr)
		{
			curr.insert(tableid);
		});
		return tableid;
	}

	//tricky tricky
	//need to *find* any table that is free whole duration
	//the below just *checks* if a given table is free for the span
	int is_available(int tableid, int startindex, int duration = 4) const
	{
		int endindex = startindex + duration;
		if( all_of(
			next(m_unreserved.cbegin(), startindex),
			next(m_unreserved.cbegin(), endindex),
			[=](const std::set<int>& curr)
			{
				return curr.count(tableid) > 0; 
		}))
			return tableid;
		return -1;
	}

	vector<int> all_available(int startindex, int duration = 4)
	{
		if (duration < 1)return vector<int>{};
		int endindex = startindex + duration;
		unique_ptr<vector<int>> 
			v_intersect(
				new vector<int>(
					next(m_unreserved.begin(), startindex)->begin(), 
					next(m_unreserved.begin(), startindex)->end()));
		for_each(
			next(m_unreserved.begin(), startindex+1),
			next(m_unreserved.begin(), endindex),
			[&v_intersect](std::set<int>& curr)
		{
			unique_ptr<vector<int>> new_intersect(new vector<int>());
			set_intersection(
				v_intersect->begin(), v_intersect->end(),
				curr.begin(), curr.end(),
				back_inserter(*new_intersect)
				);
			v_intersect = move(new_intersect);
		});
		return *v_intersect;
	}

};

ostream& operator<<(ostream& s, vector<int> v)
{
	for(auto e : v) s << e << " "; 
	s << endl;
	return s;
}

int main() 
{ 
	reservation_day<1,2,3,4,5,6,7,8,9,10> rj;
	rj.m_unreserved[50].erase(0);
	rj.m_unreserved[52].erase(1);
	rj.m_unreserved[51].erase(7);
	rj.m_unreserved[53].erase(3);

	int tid(-1);
	cout << "all_avail: " << rj.all_available(50, 4) << endl;
	cout << "rj.reserve(50, 4): " << (tid = rj.reserve(50, 4)) << endl;
	cout << "all_avail: " << rj.all_available(50, 4) << endl;
	cout << boolalpha << "rj.unreserve(" << tid << ", 50, 4): " << rj.unreserve(tid, 50, 4) << endl;
	cout << "all_avail: " << rj.all_available(50, 4) << endl;

	return 0; 
}