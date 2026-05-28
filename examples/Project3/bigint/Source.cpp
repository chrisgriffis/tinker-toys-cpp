#include <cmath>
#include <cstdio>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <utility>
using namespace std;



struct bigint : public string //basic_string<unsigned char>
{
	bigint() = default;
	bigint(const string& s) : base(s) { for_each(begin(), end(), [](char& dig) {dig -= '0'; }); }
	bigint(bigint::iterator b, bigint::iterator e) : base(b, e) {}
	bigint(unsigned int n, char c) : bigint(string(n, c)) {}

	bigint& operator<<(unsigned long n)
	{
		return *this;
	}

	friend ostream& operator<<(ostream& s, const bigint& bi)
	{
		string bi_str;
		for_each(bi.begin(), bi.end(), [&bi_str](char dig) {bi_str += ('0' + dig); });
		return s << bi_str;
	}

	friend bigint operator+(const bigint& lval, const bigint& rval)
	{
		bigint ret(1 + max(lval.size(), rval.size()), '0');
		auto l = lval.rbegin(); auto le = lval.rend();
		auto r = rval.rbegin(); auto re = rval.rend();
		auto o = ret.rbegin(); auto oe = ret.rend();
		unsigned carry(0);
		char acc(0);
		while (l != le || r != re || o != oe)
		{
			if (acc < 0) { *o = carry; break; }
			else acc = *l + *r + carry;
			carry = acc / 10; *o = carry % 10;
			if (l != le) ++l; else acc = -1;
			if (r != re) ++r; else acc = -1;
			if (o != oe) ++o; else { /*do something!*/ }
		}
		return ret[0] == 0 ? bigint(next(ret.begin()), ret.end()) : ret;
	}

private:
	using base = string; //basic_string<unsigned char>;

};


int main()
{
	bigint x(string{ "1204" });
	bigint y(string{ "8065" });
	cout << x + y;
	return 0;
}