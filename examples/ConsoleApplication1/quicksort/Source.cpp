//quicksort

#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>
using namespace std;



template<class It>
void quicks(It lo, It hi)
{
	if (hi - lo < 2) return;
	auto p = prev(hi);
	iter_swap(lo + (hi - lo) / 2, p);
	auto newpiv = partition(lo, p, [p](decltype(*p) v) {return v < *p; });
	iter_swap(newpiv, p);
	quicks(lo, newpiv);
	quicks(newpiv + 1, hi);
}



int main()
{
	vector<int> v(20);
	iota(v.begin(), v.end(), -5);
	shuffle(v.begin(), v.end(), default_random_engine{});
	for (auto i : v) cout << i << " "; cout << endl;
	quicks(v.begin(), v.end());
	for (auto i : v) cout << i << " "; cout << endl;
	cin >> v[0];
	return 0;
}

// template<class T>
// int partition(T* A, int lo, int hi)
// {
// 	int pivot = A[hi];
// 	int i = lo;
// 	for (int j = lo; j < hi; j++)
// 	{
// 		if (A[j] <= pivot)
// 		{
// 			swap(A[i], A[j]);
// 			i++;
// 		}
// 	}
// 	swap(A[i], A[hi]);
// 	return i;
// }
// 
// template<class T>
// void quicksort(T* A, int lo, int hi)
// {
// 	if (lo < hi)
// 	{
// 		int p = partition(A, lo, hi);
// 		quicksort(A, lo, p - 1);
// 		quicksort(A, p + 1, hi);
// 	}
// }

// template<class BiIter>
// BiIter partition(BiIter lo, BiIter hi)
// {
// 	auto pivot = *hi;
// 	BiIter current = lo;
// 	for (BiIter j = lo; j < hi; j++)
// 	{
// 		if (*j <= pivot)
// 			iter_swap(current++, j);
// 	}
// 	iter_swap(current, hi);
// 	return current;
// }
// 
// template<class BiIter>
// void quicksort(BiIter lo, BiIter hi)
// {
// 	if (lo < hi)
// 	{
// 		BiIter p = partition(lo, hi);
// 		quicksort(lo, p - 1);
// 		quicksort(p + 1, hi);
// 	}
// }
// 
// template<class Container>
// inline void quicksort(Container& c)
// {
// 	quicksort(c.begin(), c.end() - 1);
// }
// 
// int main()
// {
// 	//     char arr[] = "asdsfbryiultlrwqarety3dslfghkanfvsbvg erngoievgkvhsdscghweirgbnaslidgnmalieghmosirvjnaejgmrtugol3ew9485ymo9wrtgjmsieurtg9034hjgm9w48hmgjserhygmmrfhglsdjgfsoerhtoiwrhufgmsdigh";
// 	//     shuffle(arr, arr + (sizeof(arr) / sizeof(arr[0]) - 2), default_random_engine());
// 	//     cout << arr << "\n\nquicksort\n";
// 	//     quicksort(arr, 0, sizeof(arr) / sizeof(arr[0]) - 2);
// 	//     cout << arr << "\n";
// 
// 	vector<char> vec(100);
// 	generate(vec.begin(), vec.end(), []()->char {
// 		auto re = default_random_engine(random_device{}());
// 		const char* alphbet = "abcdefghijklmnopqrstuvwxyz";
// 		return alphbet[re() % 26];
// 	});
// 	cout << string(vec.begin(), vec.end()) << endl;
// 	quicksort(vec);
// 	// 	quicksort(vec.begin(), vec.end());
// 	cout << string(vec.begin(), vec.end()) << endl;
// 	return 0;
// }
