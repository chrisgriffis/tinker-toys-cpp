// This is the text editor Terface. 
// Anything you type or change here will be seen by the other person in real time.

// Write a function that applies a given 3x3 filter to a given image
#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <random>


using namespace std;

constexpr bool is_odd(unsigned K) { return (K & 0x1) == 1; }
template<typename T, unsigned M, unsigned N, typename = enable_if_t<std::is_arithmetic<T>::value && is_odd(M) && is_odd(N)>>
vector<vector<T>> apply(const vector<vector<T>>& target, T(&filter)[M][N])
{
	if (target.size() < 1 || target.front().size() < 1) throw std::invalid_argument{"Array bounds are non-positive"}; //assuming exceptions are allowed as part of coding policy
	vector<vector<T>> retval(target.size(), vector<T>(target.front().size(), 0));

	//M and N enforced as odd.
	constexpr unsigned filter_row_mid = M / 2;
	constexpr unsigned filter_col_mid = N / 2;

	//using signed indices to allow for negative values during index arithmetic
	for (int row = 0; row < int(target.size()); ++row)
	{
		for (int col = 0; col < int(target.front().size()); ++col)
		{
			retval[row][col] = 0; //initialize accumulation
			for (int filter_row = 0; filter_row < int(M); ++filter_row)
			{
				for (int filter_col = 0; filter_col < int(N); ++filter_col)
				{
						int target_row = row - filter_row_mid + filter_row;
						target_row = (target_row < 0) ? 0 : (target_row >= int(target.size())) ? (int(target.size()) - 1) : target_row;
						int target_col = col - filter_col_mid + filter_col;
						target_col = (target_col < 0) ? 0 : (target_col >= int(target[row].size())) ? (int(target[row].size()) - 1) : target_col;

						const T filter_v = filter[filter_row][filter_col];
						const T target_v = target[target_row][target_col];
						const T product = filter_v * target_v;

						if (abs(filter_v) > 1 && abs(target_v) > 1 && max(abs(product), max(abs(filter_v), abs(target_v))) != abs(product) ) 
							throw std::overflow_error{"product overflow"}; //assuming exceptions are allowed as part of coding policy
																		   //if unsigned or signs the same
						if (!is_signed<T>::value ||
							(abs(product) == product && abs(retval[row][col]) == retval[row][col]) ||
							(abs(product) == -product && abs(retval[row][col]) == -retval[row][col]))
						{
							if ((numeric_limits<T>::max() - abs(product)) < abs(retval[row][col]))
								throw std::overflow_error{ "cell accumulation overflow" }; //assuming exceptions are allowed as part of coding policy
						}
						// else unsigned and signs are different, overflow not possible with addition when operand types are the same
						retval[row][col] += product;
				}
			}
		}
	}
	return retval;
}

//convenience printing for vector of vector
template<typename C, typename = decltype(declval<C>().begin(), declval<C>().end(), void())>
ostream& operator<<(ostream& s, const C& c)
{
	for_each(c.begin(), c.end(), [&s](auto& e) {
		s << "\t" << e;
	});
	s << endl;
	return s;
}

ostream& operator<<(ostream& s, const char& c)
{
	return (s << short(c));
}

int main()
{
	int filter_int[3][3] =
	{
		{ -1, 0, 1 },
		{ -1, 0, 1 },
		{ -1, 0, 1 }
	};

	char filter_char[3][3] =
	{
		{  0, -1, 0 },
		{ -1,  0, 1 },
		{  0,  1, 0 }
	};
    float filter_float[3][3] =
    {
        { 0, -1.1f, 0 },
        { -1.1f,  0, 1.1f },
        { 0,  1.1f, 0 }
    };
    char bad_row_filter[4][3] =
	{
		{ -1, 0, 1 },
		{ -1, 0, 1 },
		{ -1, 0, 1 },
		{ -1, 0, 1 }
	};
	char bad_col_filter[3][4] =
	{
		{ 0, -1, 0, 1 },
		{ 0, -1, 0, 1 },
		{ 0, -1, 0, 1 }
	};

	constexpr int sq_image_dim = 8;

	vector<vector<float>> my_target_float(sq_image_dim, vector<float>(sq_image_dim, 0));
	for_each(my_target_float.begin(), my_target_float.end(), [sq_image_dim](auto& row){
		iota(row.begin(), row.end(), -sq_image_dim /2 + 0.0f);
		random_shuffle(row.begin(), row.end());
	});
	cout << my_target_float;
	cout << endl << endl;

	//works! numeric type and also odd dimensions
	cout << apply(my_target_float, filter_float); 
	cout << endl;

	//won't compile; filter value type mismatch against target.
	//cout << apply(my_target, filter_int); 
	
	//error C2672: 'apply': no matching overloaded function found -> won't compile; row count isn't odd.
	//cout << apply(my_target, bad_row_filter); 

	//error C2672: 'apply': no matching overloaded function found -> won't compile; col count isn't odd.
	//cout << apply(my_target, bad_col_filter); 

	vector<vector<char>> too_big(sq_image_dim, vector<char>(sq_image_dim, numeric_limits<char>::max()));
	try
	{
		//throws exception; addition overflow
		cout << apply(too_big, filter_char);
	}
	catch (std::exception& e)
	{
		cout << "thrown: " << e.what();
	}


	return 0;
}
