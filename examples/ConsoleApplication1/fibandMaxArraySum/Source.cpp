#include <vector>
#include <unordered_map>
#include <numeric>
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;

std::unordered_map<int, int> cache;
int fibo(int val)
{
	if (val <= 1)
	{
		return 1;
	}
	else if (cache.find(val) == cache.end())
	{
		cache[val] = fibo(val - 1) + fibo(val - 2);
	}
	return cache[val];
}

int findmaxsubarraysum(vector<int>& v)
{
	int min_sum(0), sum(0), max_sum(0);
	for (int val : v)
	{
		sum += val;
		if (sum<min_sum)
		{
			min_sum = sum;
		}
		if (sum - min_sum>max_sum)
		{
			max_sum = sum - min_sum;
		}
	}
	return max_sum;
}

template<unsigned N>
struct fib
{
	enum { val = fib<N - 1>::val + fib<N - 2>::val };
};
template<>
struct fib<0>
{
	enum { val = 1 };
};
template<>
struct fib<1>
{
	enum { val = 1 };
};


template<unsigned N>
struct Repeat
{
	static inline void Exec()
	{
		cout << "fib for " << N << ": " << fib<N>::val << endl;
		Repeat<N + 1>::Exec();
	}
};

template<>
struct Repeat<15>
{
	static inline void Exec() {}
};

int main()
{
	vector<int> f(50);
	iota(f.begin(), f.end(), -25);
	shuffle(f.begin(), f.end(), default_random_engine{});
	// your code goes here
	for (int i : f)cout << i << ' ';
	cout << endl << "max sum " << findmaxsubarraysum(f);
	Repeat<0>::Exec();
	return 0;
}
