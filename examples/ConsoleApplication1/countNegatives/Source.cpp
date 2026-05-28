#include <iostream>
#include <algorithm>

int countNegatives(int M[4][8], int p, int q)
{
	int numberOfNegatives = 0;
	int i(0);
	int upperBoundidx = q;
	//while this row is in range
	//and has negatives in it
	while (i < p && M[i][0] < 0)
	{
		//find the zero-basedindex in this row of the number that is lower bounded by zero
		//this index is the count of the negative numbers
		//keep track of the index since it acts as an upper bound for the row search. This comes from the vertical sort property
		upperBoundidx = (std::lower_bound(M[i], M[i] + upperBoundidx, 0) - M[i]);
		numberOfNegatives += upperBoundidx;
		++i;
	}
	//if the first element of a row is non-negative then that row has no negatives.
	return numberOfNegatives;
}



using namespace std;

int main() {
	// your code goes here
	int M[4][8] = 
	{
		{-15, -14, -13, -12, -11, -10, -9, -8},
		{ -7, -6,  -5,  -4,  -3,  -2,  -1, -1},
		{ -2, -2,   -1,   6,   7,  8},
		{  -1, 10,  11,  12,  13,  14,  15, 16}
	};
	cout << countNegatives(M, 4, 8);
	return 0;
}
