#include <iostream>
#include <vector>

using namespace std;

long getWays(long amount, vector <long> coins)
{
	vector<long> cache(amount + 1, 0);
	cache[0] = 1L;
	for (int coin : coins) 
	{
		for (long index = coin; index < cache.size(); index++) 
			cache[index] += cache[index - coin];
		for (auto x : cache) cout << x << " "; cout << endl;
	}
	return cache[amount];
}

int main() {
	int n;
	int m;
	cin >> n >> m;
	vector<long> c(m);
	for (int c_i = 0; c_i < m; c_i++) {
		cin >> c[c_i];
	}
	// Print the number of ways of making change for 'n' units using coins having the values given by 'c'
	cout << getWays(n, c);
	return 0;
}
