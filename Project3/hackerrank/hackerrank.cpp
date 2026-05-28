#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iterator>

using namespace std;

namespace my
{
	template<typename It>
	int findkthlargest(It low, It hi, int k=1,bool filter=true)
	{
		if (filter)
		{
			std::unordered_set<int> us;
			std::copy(low, hi, inserter(us,us.begin()));
			vector<int> v(us.begin(), us.end());
			return findkthlargest(v.begin(), v.end(), k, false);
		}
		if ( distance(low, hi) == 1) return *low;
		auto piv_iter = next(low, distance(low,hi) / 2);
		int piv = *piv_iter;
		iter_swap(piv_iter, prev(hi));
		auto new_piv_iter = partition(low, prev(hi), [piv](int v) {
			return v < piv;
		});
		iter_swap(new_piv_iter, prev(hi));
		if (distance(low,new_piv_iter) < k - 1)
			return findkthlargest(new_piv_iter, hi, k,false);
		if (distance(low, new_piv_iter) > k - 1)
			return findkthlargest(low,new_piv_iter, k,false);
		return *new_piv_iter;
	}
}

int main()
{
	vector<int> v{1,7,6,6,5,4,9,8,0,6,7,3,4,2,7};
	cout << my::findkthlargest(v.begin(),v.end(),6) << endl;
	sort(v.begin(), v.end());
	int sdfgwert;
	cin >> sdfgwert;
    return 0;
}

