
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
using namespace std;



// 
// int part(vector<int>& v, int lo, int hi, int pivotidx);
// 
// 
// int FindKthLargest(vector<int>& v, int k)
// {
// 	int pivotidx = random_device{}() % v.size();
// 	int lo = 0, hi = v.size() - 1;
// 	pivotidx = part(v, lo, hi, pivotidx);
// 	while (lo <= hi && k <= v.size())
// 	{
// 		if (pivotidx < k - 1)
// 		{
// 			lo = pivotidx + 1;
// 		}
// 		else if (pivotidx > k - 1)
// 		{
// 			hi = pivotidx - 1;
// 		}
// 		else return v[pivotidx];
// 		pivotidx = lo + random_device{}() % (hi - lo + 1);
// 		pivotidx = part(v, lo, hi, pivotidx);
// 	}
// 	return -1;
// }
// 
// int part(vector<int>& v, int lo, int hi, int pivotidx)
// {
// 	int newpiv = lo;
// 	int pivot = v[pivotidx];
// 	swap(v[hi], v[pivotidx]);
// 	for (int curr = lo; curr < hi; ++curr)
// 	{
// 		if (v[curr] > pivot)
// 			swap(v[newpiv++], v[curr]);
// 	}
// 	swap(v[hi], v[newpiv]);
// 	return newpiv;
// }
// 
// 


//////////////////////////////////////////////////////////////////////////
// using std::partition
//////////////////////////////////////////////////////////////////////////

template<class C, class It>
int FindKthLargest(const C& c,It lo, It hi, int k)
{
	if (hi - lo == 1)return *lo;
	auto p = prev(hi);
	iter_swap(lo + (hi - lo) / 2, p);
	auto newpiv =  partition(lo, p, [p](decltype(*p) v) {return v > *p; });
	iter_swap(newpiv, p);
	if (newpiv > c.begin() + k - 1)
	{
		hi = newpiv;
	} 
	else if (newpiv < c.begin() + k - 1)
	{
		lo = newpiv;
	}
	else return *newpiv;
	return FindKthLargest(c, lo, hi, k);
}


int main()
{
	int k(1);
	while (k>0)
	{
		vector<int> v(30);
		iota(v.begin(), v.end(), -10);
		for (auto i : v) cout << i << " "; cout << endl;
		shuffle(v.begin(), v.end(), default_random_engine(random_device{}()));
		for (auto i : v) cout << i << " "; cout << endl;
		cout << k << "th: " << FindKthLargest(v,v.begin(),v.end(), k); cout << endl; cout << endl;
		cin >> k;
	}
// 	int k(1);
// 	while (k>0)
// 	{
// 		vector<int> v(7);
// 		iota(v.begin(), v.end(), 10);
// 		for (auto i : v) cout << i << " "; cout << endl;
// 		shuffle(v.begin(), v.end(), default_random_engine{});
// 		for (auto i : v) cout << i << " "; cout << endl;
// 		auto hi = v.end() - 1;
// 		auto pivotitr = v.begin() + random_device{}() % v.size();
// 		cout << "piv: " << *pivotitr << endl;
// 		iter_swap(pivotitr, hi);
// 		auto pivotitr = std::partition(v.begin()+3, v.end(), [&](int val)->bool {
// 			return false;
// 		});
// 		iter_swap(pivotitr, hi);
// 		cout << boolalpha << ((v.begin() + 3) == pivotitr) << endl;
// 		for (auto i : v) cout << i << " "; cout  << endl<< endl;
// 		cin >> k;
// 	}
}

