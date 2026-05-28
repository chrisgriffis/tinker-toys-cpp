#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////////
// assume not a heap. dont assume no subchildren.
// while not a heap, determine largest child.
// see if largest child is bigger than parent.
// if not, then its a heap, stop
// else, swap parent with child, but small parent may 
// invalidate heap property of child, so need to check
// subchildren. make child the new root and continue
// until the bottom is reached.
template<class T>
void heapify(vector<T>& v, int root, int bottom)
{
	int maxchild;
	bool isHeap = false;
	while ((2*root + 1) < bottom && !isHeap)
	{
		//determine maxchild, may have one or none children
		if (2 * root + 2 == bottom)
			maxchild = 2 * root + 1;
		else if (v[2 * root + 1] > v[2 * root + 2])
			maxchild = 2 * root + 1;
		else
			maxchild = 2 * root + 2;

		//if its not a heap, make it so
		//may violate children heap property
		//so leave isheap as false
		if (v[root] < v[maxchild])
		{
			swap(v[root], v[maxchild]);
			root = maxchild;
		} 
		//both children already assumed to be heaps
		//no change, so nothing violated
		else
		{
			isHeap = true;
		}
	}
}

template<class T>
bool isheap(vector<T>& v, int bottom)
{
	for (int Index = 0; Index < bottom/2 ; Index++)
	{
		int child1 = 2 * Index + 1;
		int child2 = 2 * Index + 2;
		if ((child1 < bottom) && (v[Index] < v[child1])) return false;
		if ((child2 < bottom) && (v[Index] < v[child2])) return false;
	}
	return true;
}

template<class T>
void makeheap(vector<T>& v)
{
	for (int root = v.size()/2 - 1; root >= 0 ; --root)
	{
		heapify(v, root, v.size());
	}
	cout << "makeheap " << boolalpha << isheap(v,v.size()) << endl;
}

template<class T>
void heapsort(vector<T>& v)
{
	for (auto i : v) cout << i << " ";
	cout << endl;
	makeheap(v);
	for (auto i : v) cout << i << " ";
	cout << endl;
	int bottom = v.size() - 1;
	while (bottom > 0)
	{
		swap(v[0], v[bottom]);
		heapify(v, 0, bottom--);
		cout << "heapify " << boolalpha << isheap(v,bottom) << endl;
		for (auto i : v) cout << i << " ";
		cout << endl;
	}
}


int main()
{
	vector<int> v(20);
	iota(v.begin(), v.end(), 0);
	shuffle(v.begin(), v.end(), default_random_engine{});
	heapsort(v);
}

// template<class T>
// void heapify(T* input, unsigned bottom, int root = 0)
// {
//     int done, maxchild, temp;
//     done = 0;
//     while((root*2<=bottom) && !done)
//     {
//         if (root * 2 == bottom)
//         {
//             maxchild = root * 2;
//         }
//         else if (input[root * 2] > input[root * 2 - 1])
//         {
//             maxchild = root * 2;
//         }
//         else maxchild = root * 2 + 1;
// 
//         if (input[root] < input[maxchild])
//         {
//             swap(input[root], input[maxchild]);
//             root = maxchild;
//         } 
//         else
//         {
//             done = 1;
//         }
//     }
// }
// 
// template<class T, unsigned N>
// void makeheap(T(&input)[N])
// {
//     for (int i = N / 2; i >= 0; --i)
//     {
//         heapify(input, N-1, i);
//     }
// }
// 
// 
// template<class T, unsigned N>
// void heapsort(T(&arr)[N])
// {
//     int SIZE = N;
//     makeheap(arr);
//     for (int i = N - 1; i >= 1; i--)
//     {
//         swap(arr[0], arr[i]);
//         heapify(arr, i-1);
//     }
// 
// }
// 
// int main()
// {
// //     char arr[] = "asdsfbryiultlrwqarety3";
//     int arr[] = { 1,2,3,4,5,6,7,8,9 };
//     shuffle(arr, arr + (sizeof(arr) / sizeof(arr[0]) - 2), default_random_engine());
//     cout << arr;
//     //     heapsort(arr, sizeof(arr) / sizeof(arr[0]) - 2);
//     char buff[(sizeof(arr) / sizeof(arr[0])) - 1];
//     char buff1[(sizeof(arr) / sizeof(arr[0]))];
//     buff1[(sizeof(arr) / sizeof(arr[0])) - 1] = '\0';
//     memcpy_s(buff, (sizeof(arr) / sizeof(arr[0])) - 1, arr, (sizeof(arr) / sizeof(arr[0])) - 1);
// //     makeheap(buff);
//     memcpy_s(buff1, (sizeof(arr) / sizeof(arr[0])), buff, (sizeof(arr) / sizeof(arr[0])) - 1);
//     cout << buff1;
//     heapsort(arr);
//     memcpy_s(buff1, (sizeof(arr) / sizeof(arr[0])), buff, (sizeof(arr) / sizeof(arr[0])) - 1);
//     cout << buff1;
//     return 0;
// }
