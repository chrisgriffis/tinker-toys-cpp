
#include <iostream>
#include <random>
#include <iterator>


namespace foo
{
	template<class It>
	void shuffle(It low, It hi)
	{
		decltype(hi) current = hi;
		auto remaining = hi - low;
		while (remaining)
			iter_swap(--current, low + (std::random_device{}() % remaining--));
	}
}


int main()
{
	using namespace std;
	vector<int> v{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	foo::shuffle(v.begin(), v.begin() + 8);
	cin >> v[0];
}
