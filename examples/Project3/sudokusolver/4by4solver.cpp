#include <iostream>
#include <array>
#include <bitset>
#include <sstream>
#include <string>

using namespace std;

template <size_t N>
using grid = array<array<char,N*N>,N*N>;

grid<4> to_indexes(grid<4> chars)
{
	for (auto& row : chars)
		for (auto& c : row)
			if (c == '.') continue;
			else if (c <= '9')
				c -= '0';
			else c -= ('a'-10);
	return chars;
}

grid<4> from_indexes(grid<4> indexes)
{
	for (auto& row : indexes)
		for (auto& c : row)
			if (c == '.') continue;
			else if (c <= 9)
				c += '0';
			else c += ('a'-10);
	return indexes;
}

template <size_t N>
grid<N> solve(grid<N> unsolved)
{
	grid<N> candidate(unsolved);
	auto checkrow = [&cand = candidate](int row)
	{
		bitset<N*N> b;
		for (size_t col = 0; col < N*N; ++col)
			if (cand[row][col] == '.') continue;
			else if (b[cand[row][col]]) return false;
			else b.set(cand[row][col]);
			return true;
	};
	auto checkcol = [&cand = candidate](int col)
	{
		bitset<N*N> b;
		for (size_t row = 0; row < N*N; ++row)
			if (cand[row][col] == '.') continue;
			else if (b[cand[row][col]]) return false;
			else b.set(cand[row][col]);
			return true;
	};
	auto checkbox = [&cand = candidate](int row, int col)
	{
		bitset<N*N> b;
		row /= N; row *= N;
		col /= N; col *= N;
		for (size_t index = 0; index < N*N; ++index)
			if (cand[row + index / N][col + index % N] == '.') continue;
			else if (b[cand[row + index / N][col + index % N]]) return false;
			else b.set(cand[row + index / N][col + index % N]);
			return true;
	};
	int row = 0;
	int col = 0;
	while (candidate[row][col] < N*N || candidate[row][col] == '.')
	{

		if (candidate[row][col] == '.')
			candidate[row][col] = 0;
		else
		{
			if (candidate[row][col] == unsolved[row][col])
			{
				//already filled, advance current cell
				++col;
				if (col > (N*N-1))
				{
					col = 0;
					++row;
				}
				if (row > (N*N-1)) break;
				continue;
			}
		}

		//try current value
		if (checkrow(row) && checkcol(col) && checkbox(row, col))
		{
			//advance
			++col;
			if (col > (N*N-1))
			{
				col = 0;
				++row;
			}
			if (row > (N*N-1)) break;
			continue;
		}

		while (candidate[row][col] == unsolved[row][col] || ++candidate[row][col] > (N*N-1))
		{
			if (candidate[row][col] != unsolved[row][col])
				candidate[row][col] = '.';
			if (col < 1)
			{
				--row;
				col = (N*N-1);
			}
			else --col;
			if (row < 0) return unsolved;
		}
	}
	return candidate;
}

ostream& operator<<(ostream& s, const grid<4>& g)
{
	int x = 0;
	for (const auto& v : g)
	{
		for (const auto& c : v)
		{
			s << c << " ";
			if (++x % 4 == 0) s << '\t';
		}
		s << endl;
		if (x % (4*4*4) == 0) s << endl;
	}
	s << endl;
	return s;
}

int main()
{
	grid<4> puz1 =
		grid<4>{
		array<char,16>{'.','.','5','.',   '.','4','.','.',    '.','2','.','.',    'c','0','e','.'},
		array<char,16>{'.','e','.','.',   '.','f','.','7',    '3','4','.','a',    '5','.','d','.'},
		array<char,16>{'3','.','1','0',   'b','d','.','.',    '.','e','.','.',    '.','.','f','8'},
		array<char,16>{'.','2','a','c',   '.','e','.','.',    'f','8','.','1',    '.','.','4','3'},

		array<char,16>{'b','.','.','3',   '.','c','.','.',    '2','9','.','.',    '6','a','.','1'},
		array<char,16>{'c','.','.','a',   '8','.','.','e',    '4','1','6','.',    'd','.','2','.'},
		array<char,16>{'.','.','e','d',   'a','9','.','.',    '.','.','3','8',    '.','7','c','.'},
		array<char,16>{'.','.','0','.',   '.','2','b','d',    '.','5','.','7',    '.','.','.','f'},

		array<char,16>{'6','.','.','.',   'f','.','5','.',    '1','7','e','.',    '.','4','.','.'},
		array<char,16>{'.','0','3','.',   'd','1','.','.',    '.','.','4','f',    '8','e','.','.'},
		array<char,16>{'.','7','.','e',   '.','b','a','8',    '6','.','.','9',    '3','.','.','5'},
		array<char,16>{'a','.','f','4',   '.','.','c','2',    '.','.','8','.',    '9','.','.','7'},

		array<char,16>{'0','d','.','.',   '2','.','e','4',    '.','.','1','.',    'f','9','3','.'},
		array<char,16>{'8','a','.','.',   '.','.','6','.',    '.','.','9','2',    'b','c','.','0'},
		array<char,16>{'.','6','.','9',   'c','.','0','b',    '7','.','f','.',    '.','.','a','.'},
		array<char,16>{'.','3','b','1',   '.','.','d','.',    '.','.','5','.',    '.','2','.','.'},
	};


// 	size_t row = 0;
// 	size_t col = 0;
// 	string l;
// 	while (getline(cin,l))
// 	{
// 		stringstream ls(l);
// 		while(ls >> puz1[row][col])
// 		{
// 			++col;
// 			if (col > 15)
// 			{
// 				col = 0;
// 				++row;
// 			}
// 			if (row > 15) break;
// 		}
// 	}

	cout << endl << endl << puz1;
	grid<4> solv(from_indexes(solve<4>(to_indexes(puz1))));
	cout << endl << endl << solv;
	cin >> solv[0][0];

	return 0;
}
