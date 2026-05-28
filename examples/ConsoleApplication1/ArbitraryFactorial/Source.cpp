#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

vector <int> factorial(unsigned int x);
void factorial(vector<int>& multiplicand, unsigned int multiplier);
unsigned int digitCount(unsigned int x);
void print(const vector<int>& v);


int main()
{
	print(factorial(10));
	print(factorial(20));
	print(factorial(30));
	print(factorial(40));
	print(factorial(50));
	print(factorial(60));
	print(factorial(70));
	print(factorial(80));
	print(factorial(88));
	print(factorial(888));
}

vector <int> factorial(unsigned int x)
{
	//rough guess on size
	vector<int> result(x*digitCount(x));
	//initialize start value to 1
	for (auto& x : result) x = 0;
	result[result.size() - 1] = 1;
	//calculate the factorial
	factorial(result, x);
	//rbvo the result
	return result;
}

void factorial(vector<int>& multiplicand, unsigned int multiplier)
{
	if (multiplier <= 1) return;
	unsigned long carry(0);
	//for each digit in the MULTIPLICAND, use it to calculate an
	//integer multiple of the multiplier. then calculate carry
	// and the least sig digit. replace with least sig.
	for (int digitidx = multiplicand.size() - 1; digitidx >= 0; digitidx--)
	{
		multiplicand[digitidx] *= multiplier;
		multiplicand[digitidx] += carry;
		//carry can become large, may require a few iterations to 'digitize'
		carry = multiplicand[digitidx] / 10;
		multiplicand[digitidx] %= 10;
	}
	//decrease by one (my original formulation)
	return factorial(multiplicand, multiplier - 1);
}

unsigned int digitCount(unsigned int x)
{
	unsigned int r = 0;
	while (x)
	{
		x /= 10;
		++r;
	}
	return r;
}

void print(const vector<int>& v)
{
	bool nonzero = false;
	for (auto e : v)
	{
		if (nonzero || e > 0)
		{
			nonzero = true;
			cout << e;
		}
	}
	cout << endl; cout << endl;
}
