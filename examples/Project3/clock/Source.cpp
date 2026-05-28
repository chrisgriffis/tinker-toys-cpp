#include <iostream>
#include <algorithm>

using namespace std;


float angle(int h, int m)
{
	h %= 12;
	const float deg_per_min = 360.0f / 60.0f;
	const float m_deg = m * deg_per_min;

	const float deg_per_hr = 360.0f / 12.0f; //30.0
	const float deg_per_min_perHr = deg_per_hr / 60.0f;
	const float h_deg = h * deg_per_hr + m * deg_per_min_perHr;

	auto diff = abs(h_deg - m_deg);
	return (diff > 180.0f) ? (360.0f - diff) : diff;
}

int main()
{
	int h, m;
	while (cin >> h)
	{
		cin >> m;
		cout << endl << angle(h, m) << endl;
	}
	int debugwait__;
	cin >> debugwait__;
	return 0;
}