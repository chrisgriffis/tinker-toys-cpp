#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


template<class C, class It>
void permute(C& s, It a, It z)
{
	if(a!=z && (z-a <=2)) 
	{
		cout << s << endl;;
		iter_swap(a, prev(z));
		cout << s << endl;;
		iter_swap(a, prev(z));
		return;
	}
	auto trgt = next(a);
	while (trgt!=z)
	{
		permute(s, next(a), z);
		iter_swap(a,trgt);
		permute(s, next(a),z);
		iter_swap(a, trgt);
		++trgt;
	}
}

//print all permutations

int main()
{
	string s;
	while (cin >> s)
	{
		permute(s, s.begin(), s.end());
	}
	
	return 0;
}