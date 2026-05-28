#include <iostream>
using namespace std;

template<int N>
struct isEven
{
	constexpr static bool val = N % 2 == 0;
};

template<unsigned N, bool Pred>
struct collatzStepsImpl
{
	enum { count = 1 + collatzStepsImpl<N / 2, isEven<N / 2>::val>::count };
};

template<unsigned N>
struct collatzStepsImpl<N, false>
{
	enum { count = 1 + collatzStepsImpl<3 * N + 1, isEven<3 * N + 1>::val>::count };
};

template<>
struct collatzStepsImpl<1, false>
{
	enum { count = 0 };
};

template<unsigned N>
struct collatzSteps
{
	enum { count = collatzStepsImpl<N, isEven<N>::val>::count };
};

template<unsigned N>
struct Repeat
{
	static inline void Exec()
	{
		cout << "collatz count for " << N << ": " << boolalpha << collatzSteps<N>::count << endl;
		Repeat<N - 2>::Exec();
	}
};

template<>
struct Repeat<1>
{
	static inline void Exec() {}
};

template<>
struct Repeat<0>
{
	static inline void Exec() {}
};

int main()
{
	Repeat<101>::Exec();
}