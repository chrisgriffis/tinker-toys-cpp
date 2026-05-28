#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;


int main() {

	std::ifstream input;
	std::ofstream output;
	input.open("input.txt");
	output.open("output.txt");
	int m, n;
	input >> n >> m;
	vector<int> A(n);
	for (int Index = 0; Index < n; Index++)
	{
		input >> A[Index];
	}
	for (int Index = 0; Index < m; Index++)
	{
		int q, lo, hi;
		input >> q >> lo >> hi;
		// 		lo -= 1; hi -= 1;
		switch (q)
		{
		case 1:
			std::rotate(A.begin(), A.begin() + lo - 1, A.begin() + hi);
			break;
		case 2:
			std::rotate(A.begin() + lo - 1, A.begin() + hi, A.end());
			break;
		default:
			throw;
		}
		// 		for (auto e : A)cout << e << " "; cout << endl;

	}
	output << abs(A[0] - A[n - 1]) << endl;
	for (auto e : A)output << e << " "; output << endl;

	return 0;
}
