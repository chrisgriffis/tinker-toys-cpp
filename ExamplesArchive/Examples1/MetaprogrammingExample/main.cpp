#include <iostream>


template<size_t N>
struct FACT
{
	static const int RESULT = N*FACT<N-1>::RESULT;
};

template<>
struct FACT<1>
{
	static const int RESULT = 1;
};

template<size_t BASE, size_t EXP>
struct POW
{
	static const size_t RESULT = BASE*POW<BASE,EXP-1>::RESULT;
};

template<size_t BASE>
struct POW<BASE,1>
{
	static const size_t RESULT = BASE;
};

int main()
{
	int i = POW<2,5>::RESULT;
	std::cout << i << std::endl;
}