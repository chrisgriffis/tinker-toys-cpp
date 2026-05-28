// #include <iostream>
// #include <string>
// #include <future>
// using namespace std;
// 
// int main() {
// 	// your code goes here
// 	std::string s1("hello ");
// 	std::string s2("world");
// 	auto z = [=](std::string x){cout << x; };
// 	auto q = std::async(std::bind(z, s1));
// 	q.get();
// 	cout << s2;
// 	return 0;
// }

#include <iostream>
using namespace std;
int perfectSqrt(int number, int guess);

int perfectSqrt(int number)
{
	//f(a) = f(a) + f'(a)(x-a) + f'(a)(x-a)^2 + ...
	return perfectSqrt(number, 1);
}

int perfectSqrt(int number, int guess)
{
	if (guess*guess < number)
	{
		return perfectSqrt(number, guess + 1);
	}
	else return guess;
}
int main() {
	// your code goes here
	cout << "64: " << perfectSqrt(64) << endl;
	return 0;
}