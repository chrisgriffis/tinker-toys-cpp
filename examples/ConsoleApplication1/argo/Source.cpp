// This is the text editor interface. 
// Anything you type or change here will be seen by the other person in real time.

// input = "2+3*5-2+9/4"
// output = 17.25, 17, 18


//alternative 
#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
///
// ONLY SUPPORT INTEGER DIVISION FOR NOW

int main()
{
	using namespace std;
	//cin >> s; //assume no spaces, well formed etc
	using seq_t = basic_string<int>;
	//convert to string of ints
	seq_t s;
	for_each(
		istream_iterator<char>(cin),
		istream_iterator<char>(),
		[&s](const char& c)
	{
		if (c >= '0' &&   c <= '9') s += static_cast<int>((c - '0'));
		else s += c;
	}
	);

	//debug output
	for_each(s.begin(), s.end(), [](int& c) {
		cout << c << ",";
	}); cout << endl;



	//step 2:
	seq_t newval;
	auto itr = s.begin();
	while (itr != s.end())
	{
		if (next(itr) != s.end())
		{
			switch (*next(itr)) //if the one after me is an operator
			{
				//change my val
			case '*':
				advance(itr, 2); //hop over and look backwards so can set current itr to computation and proceed
				*itr = (*prev(prev(itr)) * (*itr));
				break;
			case '/':
				advance(itr, 2);
				*itr = (*prev(prev(itr)) / (*itr));
				break;
			default:
				newval += *itr++;
				break;
			}
		}
		else newval += *itr++;
	}

	for_each(newval.begin(), newval.end(), [](int& c) {
		cout << c << ",";
	}); cout << endl;


	//step 3:
	seq_t endval;
	itr = newval.begin();
	while (itr != newval.end())
	{
		if (next(itr) != newval.end())
		{
			switch (*next(itr)) //if the one after me is an operator
			{
				//change my val
			case '+':
				advance(itr, 2); //hop over and look backwards so can set current itr to computation and proceed
				*itr = (*prev(prev(itr)) + (*itr));
				break;
			case '-':
				advance(itr, 2);
				*itr = (*prev(prev(itr)) + (-1)*(*itr));
				break;
			default:
				endval += *itr++;
				break;
			}
		}
		else endval += *itr++;
	}


	cout << endval[0] << endl;
	return 0;
}


/*
step 1: preprocess 2 -> '\x2'
step 2 search for '*' and '/' and collapse, remove subrange replace with resutl
step 3 same but for addition and subtraction same but treat is as sum of num with -1*right side (preserve commutivity and chars are signed)
*/