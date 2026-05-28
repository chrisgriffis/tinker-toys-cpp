#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <map>
#include <iterator>
#include <xutility>
#include <algorithm>
#include <string>
using namespace std;


//template<class InputIter>
//requires forward_iterable<FwdIter>()
struct NonUniformDistribution
{
	//	NonUniformDistribution(InputIter i);
	NonUniformDistribution::NonUniformDistribution(vector<int> values, vector<double> probs);

	int gen();
	vector<double> m_partitions;
	vector<int> m_values;
	vector<double> m_probs;
};

void printDist(NonUniformDistribution d)
{
	map<int, int> histogram;
	for (int i = 0; i < 10000; ++i)
	{
		++histogram[d.gen()];
	}
	for (auto i : histogram)
	{
		cout << i.first << ' ' << string(i.second / 100, '*') << endl;
	}
}

NonUniformDistribution::NonUniformDistribution(vector<int> values, vector<double> probs) :
	m_values(values),
	m_probs(probs),
	m_partitions()
{
	m_partitions.emplace_back(0.0);
	std::partial_sum(probs.begin(), probs.end(), back_inserter(m_partitions));
}

int NonUniformDistribution::gen()
{
	// 	default_random_engine seed(random_device{}());
	// 	uniform_real_distribution<> dist(0, 1);
	// 	double val = dist(seed);
	double val = uniform_real_distribution<>{ 0, 1 }(default_random_engine{ random_device{}() });
	int idx = distance(m_partitions.begin(), upper_bound(m_partitions.begin(), m_partitions.end(), val)) - 1;
	return m_values[idx];
}

int main()
{
	vector<int> w{ 1,2,3,4 };
	vector<double> p{ .2,.7,.05,.05 };
	NonUniformDistribution nud(w, p);
	printDist(nud);
}

// #include <iostream>
// #include <numeric>
// #include <vector>
// #include <algorithm>
// #include <random>
// using namespace std;
// 
// int main() {
// 	vector<int> f(50);
// 	iota(f.begin(), f.end(), 0);
// 	// your code goes here
// 	for (int i : f)cout << i << ' ';
// 	cout << endl;
// 	shuffle(f.begin(), f.end(), default_random_engine(random_device{}()));
// 	for (int i : f)cout << i << ' ';
// 	return 0;
// }