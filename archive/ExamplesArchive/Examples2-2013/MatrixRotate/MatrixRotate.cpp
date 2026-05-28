#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


typedef unsigned int uint;

class MatrixRotator
{
public:
	MatrixRotator(int M, int N);
	MatrixRotator& operator=(vector<vector<int>>);
	void Rotate();
	void Display();
	int getM(){ return _M; }
	int getN(){ return _N; }
private:
	int** allocateMatrix(int M, int N);
	void deallocate(int** matrix, const int M, const int N);
	void rotateRing(int** inmat, int left, int right, int top, int bottom, int** outmat);
	void translateLeft(int** inmat, int currentrow, int left, int right, int** outmat);
	void translateDown(int** inmat, int currentcol, int top, int bottom, int** outmat);
	void translateRight(int** inmat, int currentrow, int left, int right, int** outmat);
	void translateUp(int** inmat, int currentcol, int top, int bottom, int** outmat);

	int** _2dArr;
	const int _M;
	const int _N;
};
MatrixRotator::MatrixRotator(int M, int N) : _2dArr(nullptr),_M(M), _N(N)
{
	_2dArr = allocateMatrix(M, N);
}

int** MatrixRotator::allocateMatrix(int M, int N)
{
	int** matrix = new int*[M];
	for (int m = 0; m < M; m++)
	{
		matrix[m] = new int[N];
		for (int n = 0; n < N ; n++)
		{
			matrix[m][n] = m*N + n;
		}
	}
	return matrix;
}

void MatrixRotator::deallocate(int** matrix, const int M, const int N)
{
	for (int m = 0; m < M; m++)
	{
		delete [] matrix[m];
	}
}

void MatrixRotator::Display()
{
	for (int m = 0; m < _M ; m++)
	{
		for (int n = 0; n < _N ; n++)
		{
			cout << _2dArr[m][n] << " ";
		}
		cout << "\n";
	}
// 	cout << "\n\n";
}

void MatrixRotator::Rotate()
{
	int** newmatrix = allocateMatrix(_M, _N);
	rotateRing(_2dArr,0,_N-1,0,_M-1,newmatrix);
	int** temp = _2dArr;
	_2dArr = newmatrix;
	deallocate(temp, _M, _N);
}

void MatrixRotator::rotateRing(int** inmat, int left, int right, int top, int bottom, int** outmat)
{
	if (right<left || top>bottom){ return; }
	translateLeft(inmat,top,left,right,outmat);
	translateDown(inmat,left,top,bottom,outmat);
	translateRight(inmat,bottom,left,right,outmat);
	translateUp(inmat,right,top,bottom,outmat);
 	rotateRing(inmat,left+1,right-1,top+1,bottom-1,outmat);
}

void MatrixRotator::translateLeft(int** inmat, int currentrow, int left, int right, int** outmat)
{
	for (int j = left; j < right ; j++)
	{
		outmat[currentrow][j] = inmat[currentrow][j + 1];
	}
}

void MatrixRotator::translateDown(int** inmat, int currentcol, int top, int bottom, int** outmat)
{
	for (int i = top; i < bottom; i++)
	{
		outmat[i+1][currentcol] = inmat[i][currentcol];
	}
}

void MatrixRotator::translateRight(int** inmat, int currentrow, int left, int right, int** outmat)
{
	for (int j = right; j > left;  j--)
	{
		outmat[currentrow][j] = inmat[currentrow][j - 1];
	}
}

void MatrixRotator::translateUp(int** inmat, int currentcol, int top, int bottom, int** outmat)
{
	for (int i = bottom; i > top;  i--)
	{
		outmat[i-1][currentcol] = inmat[i][currentcol];
	}
}

MatrixRotator& MatrixRotator::operator=(vector<vector<int>> v	)
{
	for (uint i = 0; i < v.size() ; i++)
	{
		for (uint j = 0; j < v[i].size() ; j++)
		{
			_2dArr[i][j] = v[i][j];
		}
	}
	return *this;
}


int main() {
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */
	int m(0), n(0), r(0);
	cin >> m;
	cin >> n;
	cin >> r;
	MatrixRotator rotatable(m, n);
	//this is horrible and assumes valid inputs perfectly configured
	int val(0);
	vector<vector<int>> v;
	for (int i = 0; i < m ; i++)
	{
		vector<int> w;
		for (int j = 0; j < n ; j++)
		{
			cin >> val;
			w.push_back(val);
		}
		v.push_back(w);
	}
	rotatable = v;
	for (int i = 0; i < r ; i++)
	{
		rotatable.Rotate();
	}
	rotatable.Display();
// 	system("pause");
	return 0;
}
