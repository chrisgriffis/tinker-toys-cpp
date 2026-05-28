#include <iostream>
#include <tuple>
#include <queue>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>
#include <windows.h>
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


struct Lock
{
	Lock() { ::InitializeCriticalSection(&m_cs); }
	void enter() { ::EnterCriticalSection(&m_cs); }
	void exit() { ::LeaveCriticalSection(&m_cs); }
	~Lock() { ::DeleteCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

struct ScopedLock
{
	ScopedLock(Lock& l) :m_lock(l) { l.enter(); }
	~ScopedLock() { m_lock.exit(); }
private:
	Lock& m_lock;
};

template<class T>
struct IMyStream
{
	virtual IMyStream& operator>>(T& t) = 0;
	virtual bool isMore() const = 0;
};


template<class R>
struct Stream : public IMyStream<R>
{
	Stream(vector<R>& v) :it(v.begin()), vend(v.end()) {}
private:
	typename vector<R>::iterator it;
	typename vector<R>::iterator vend;
public:
	virtual bool isMore() const override
	{
		return it != vend;
	}

	virtual IMyStream& operator >>(R& t) override
	{
		t = *it++; return *this;
	}
};

template<class Rel1, class Rel2>
class HeterogenousStreamMerge : public IMyStream<pair<Rel1, Rel2>>
{
public:
	void Merge(IMyStream<Rel1>& s1, IMyStream<Rel2>& s2)
	{
		priority_queue<Rel1, vector<Rel1>, std::greater<Rel1>> s1_heap;
		priority_queue<Rel2, vector<Rel2>, std::greater<Rel2>> s2_heap;

		while (
				s1.isMore() || 
				s2.isMore() ||
				(!s1_heap.empty() && !s2_heap.empty())
			)
		{
			Rel1 r1; Rel2 r2; 
			if (s1.isMore()){ s1 >> r1; s1_heap.emplace(move(r1));}
			if (s2.isMore()){ s2 >> r2; s2_heap.emplace(move(r2));}

			if (get<0>(s1_heap.top()) == get<0>(s2_heap.top()))
			{
				auto t1 = move(s1_heap.top()); s1_heap.pop();
				auto t2 = move(s2_heap.top()); s2_heap.pop();
				{
					ScopedLock sl(Lock{});
//					in c#: yield return make_pair(move(t1), move(t2));
					m_buffer.emplace(make_pair(move(t1), move(t2)));
				}
			}
		}
	}

	virtual IMyStream& operator >>(pair<Rel1, Rel2>& t) override
	{
		ScopedLock sl(Lock{});
		if (!m_buffer.empty())
		{
			t = move(m_buffer.front());
			m_buffer.pop();
		}
		return *this;
	}

	virtual bool isMore() const override
	{
		ScopedLock sl(Lock{});
		return !m_buffer.empty();
	}

private:
	queue < pair<Rel1, Rel2>> m_buffer;
};




int main()
{
	vector<Rel1> v1;
	vector<Rel2> v2;
	v1.emplace_back(Rel1{ 4,string{ "chris4" },string{ "home4" } });
	v1.emplace_back(Rel1{ 11,string{ "chris11" },string{ "home11" } });
	v1.emplace_back(Rel1{ 6,string{ "chris6" },string{ "home6" } });
	v1.emplace_back(Rel1{ 7,string{ "chris7" },string{ "home7" } });
	v1.emplace_back(Rel1{ 9,string{ "chris9" },string{ "home9" } });
	v1.emplace_back(Rel1{ 10,string{ "chris10" },string{ "home10" } });
	v1.emplace_back(Rel1{ 8,string{ "chris8" },string{ "home8" } });

	v2.emplace_back(Rel2{ 4, 123412344, 123412344 });
	v2.emplace_back(Rel2{ 6, 123412346, 123412346 });
	v2.emplace_back(Rel2{ 8, 123412348, 123412348 });
	v2.emplace_back(Rel2{ 9, 123412349, 123412349 });
	v2.emplace_back(Rel2{ 10, 1234123410, 1234123410 });
	v2.emplace_back(Rel2{ 11, 1234123411, 1234123411 });
	v2.emplace_back(Rel2{ 7, 123412347, 123412347 });

	auto s1 = Stream<Rel1>(v1);
	auto s2 = Stream<Rel2>(v2);

	HeterogenousStreamMerge<Rel1,Rel2> hsm;
	hsm.Merge(s1, s2);
	auto t1 =  std::thread([&hsm, &s1, &s2]() {hsm.Merge(s1, s2); });
	t1.join();

	while(hsm.isMore())
	{
		pair<Rel1, Rel2> e;
		hsm >> e;
		cout << 
			get<0>(e.first) << ", " << 
			get<1>(e.first) << ", " << 
			get<2>(e.first) << ", " << 
			get<1>(e.second) << ", " << 
			get<2>(e.second) << endl;
	}
	return 0;
}

