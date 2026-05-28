#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

void nextperm(string& s)
{
    auto curr = s.rbegin();
    for (; next(curr) != s.rend(); ++curr)
        if (*next(curr) < *curr) break;
    ++curr;
    if (curr != s.rend())
    {
        auto it = upper_bound(s.rbegin(), curr, *curr);
        iter_swap(curr, it);
    }
    reverse(s.rbegin(), curr);
}


void allperm(string& s, string::iterator rng_start, string::iterator rng_end)
{
	if (rng_end - rng_start == 1) 
	{
		cout << s << endl;
		return;
	}
	for (auto curr = rng_start; curr != rng_end; ++curr)
	{
		iter_swap(curr, rng_start);
		allperm(s, next(rng_start), rng_end);
		iter_swap(curr, rng_start);
	}
}
int main()
{
    string s("12345");
// 	allperm(s, s.begin(), s.end());
    do
    {
        cout << s << endl;
        nextperm(s);
    } while (s != string("12345"));
    cout << s << endl;
    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}