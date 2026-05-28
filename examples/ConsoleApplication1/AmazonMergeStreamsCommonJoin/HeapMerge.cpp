#include <iostream>
#include <tuple>
#include <queue>
#include <functional>
using namespace std;

using uint = unsigned int;
using SeshId = uint;
using Person = string;
using Address = string;
using Card = unsigned long;
using Ref = unsigned long;
using Rel1 = tuple<SeshId, Person, Address>;
using Rel2 = tuple<SeshId, Card, Ref>;
using Rel3 = tuple<SeshId, Person, Address, Card, Ref>;

// template <class T>
// class Stream
// {
// 	T& operator*() { return T(); }
// 	Stream& operator++(int) { return *this; }
// };

// template< template<class> class Stream>
// vector<Type3> merge(Stream<Type1>& s1, Stream<Type2>& s2)

vector<Rel3> merge(vector<Rel1>& v1, vector<Rel2>& v2);

int main()
{
	vector<Rel1> v1;
	vector<Rel2> v2;
	vector<Rel3> v3;
	v1.emplace_back(Rel1{ 4,string{ "chris4" },string{ "home4" } });
	v1.emplace_back(Rel1{ 11,string{ "chris11" },string{ "home11" } });
	v1.emplace_back(Rel1{ 6,string{ "chris6" },string{ "home6" } });
	v1.emplace_back(Rel1{ 7,string{ "chris7" },string{ "home7" } });
	v1.emplace_back(Rel1{ 9,string{ "chris9" },string{ "home9" } });
	v1.emplace_back(Rel1{ 10,string{ "chris10" },string{ "home10" } });
	v1.emplace_back(Rel1{ 8,string{ "chris8" },string{ "home8" } });

	v2.emplace_back(Rel2{ 4, 123412344, 123412344 });
	v2.emplace_back(Rel2{ 6, 123412346, 123412346});
	v2.emplace_back(Rel2{ 8, 123412348, 123412348 });
	v2.emplace_back(Rel2{ 9, 123412349, 123412349});
	v2.emplace_back(Rel2{ 10, 1234123410, 1234123410});
	v2.emplace_back(Rel2{ 11, 1234123411, 1234123411 });
	v2.emplace_back(Rel2{ 7, 123412347, 123412347 });

	v3 = merge(v1, v2);
	return 0;
}

vector<Rel3> merge(vector<Rel1>& v1, vector<Rel2>& v2)
{
	priority_queue<Rel1, vector<Rel1>, std::greater<Rel1>> s1_heap;
	priority_queue<Rel2, vector<Rel2>, std::greater<Rel2>> s2_heap;
	vector<Rel3> retval;

	auto s1 = v1.begin();
	auto s2 = v2.begin();
	while (s1 != v1.end() || s2 != v2.end() || !s1_heap.empty() || !s2_heap.empty())
	{
		if(s1 != v1.end()) s1_heap.emplace(*s1++);
		if(s2 != v2.end()) s2_heap.emplace(*s2++);

		if (get<0>(s1_heap.top()) == get<0>(s2_heap.top()))
		{
			auto t1 = move(s1_heap.top()); s1_heap.pop();
			auto t2 = move(s2_heap.top()); s2_heap.pop();
			retval.emplace_back(Rel3{
				get<0>(t1),
				get<1>(t1),
				get<2>(t1),
				get<1>(t2),
				get<2>(t2),
			});
		}
	}
	return retval;
}



