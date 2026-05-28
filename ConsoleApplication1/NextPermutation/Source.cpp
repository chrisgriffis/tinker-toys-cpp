
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

using uint = unsigned int;
namespace toy
{
	//////////////////////////////////////////////////////////////////////////
	/*
		1. see a uint as a collection of digits in the form [ prefix | suffix ]
		the suffix is the longest monotonically decreasing sequence the contains the least significant digit.
		the prefix is the digits not in the suffix.
		2. find the lowest digit in suffix greater than the least sig digit in prefix
		3. swap those digits
		4. sort the suffix in ascending order of digits.
	*/
	uint next_permutation(uint val)
	{
		//convert uint into a collection of digits
		//use string mechanics for convenient container manip
		//appending yields a string of digits in reverse order
		//but prepending is messier with string manip. 
		//just work with it reversed.
		std::basic_string<uint> digits;
		while (val)
		{
			digits += val%10;
			val /= 10;
		}
// 		for (uint d : digits)std::cout << d; std::cout << "\n\n";

		//1. see a uint as a collection of digits in the form [ prefix | suffix ]
		//currently have a "stringification" of the digits in reverse order.
		//need to find the index of digit preceding a monotonically decreasing suffix
		//could reverse the string and search for increase in reverse order, 
		//or could just search forward in an already reversed string
		uint indexPrefixLeastSigDigit(0);
		for (uint index = 1; index < digits.size() ; ++index)
		{
			if (digits[index] < digits[index - 1])
			{
				indexPrefixLeastSigDigit = index;
				break;
			}
		}

		//2. find the lowest digit in suffix greater than the least sig digit in prefix
		//again, string is reversed so search forward, ascending
		auto smallestSuffixValGreaterThanPrefixLsdItr = 
			std::find_if(digits.begin(), digits.begin() + indexPrefixLeastSigDigit, 
			[digits,indexPrefixLeastSigDigit](const uint& digit)->bool
		{
			return digit > digits[indexPrefixLeastSigDigit];
		});

		// 3. swap those digits
		std::swap(digits[indexPrefixLeastSigDigit], *smallestSuffixValGreaterThanPrefixLsdItr);
// 		for (uint d : digits)std::cout << d; std::cout << "\n\n";

		//4. sort the suffix in ascending order of digits.
		//the digit string is already reversed so the suffix is already sorted!
		//need to retore correct order of prefix, which is reversed and out of position
		std::reverse(digits.begin() + indexPrefixLeastSigDigit, digits.end());
// 		for (uint d : digits)std::cout << d; std::cout << "\n\n";

		//restore the position of the prefix
		std::rotate(digits.begin(), digits.begin() + indexPrefixLeastSigDigit, digits.end());
// 		for (uint d : digits)std::cout << d; std::cout << "\n\n";

		//transform back into a uint
		uint retval(0);
		for (uint d:digits)
		{
			retval = 10 * retval + d;
		}
		return retval;
	}
}


int main()
{
	std::cout << std::boolalpha << (toy::next_permutation(13524u) == 13542u) << std::endl;
	std::cout << std::boolalpha << (toy::next_permutation(13542u) == 14235u) << std::endl;
	std::cout << std::boolalpha << (toy::next_permutation(12345u) == 12354u) << std::endl;
	int k;
	std::cin >> k;
}