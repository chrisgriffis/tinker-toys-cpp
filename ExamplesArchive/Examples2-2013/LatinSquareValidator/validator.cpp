//Sudoku Validator

#include "IntegerSquareRootOf.h"

#include <iostream>
#include <bitset>


template<size_t N> struct StaticInt {static const size_t value = N;};
template<size_t N> struct Rank : public StaticInt<N>{};
template<size_t N> struct Count : public StaticInt<N>{};

template<typename T> struct ArrayDeducer : public Rank<0>, public Count<0>{};
template<typename T> struct ArrayDeducer<T[]> : public Rank<1 + ArrayDeducer<T>::value>, public Count<0>{};
template<typename T,size_t N> struct ArrayDeducer<T[N]> : public Rank<1 + ArrayDeducer<T>::Rank::value>, public Count<N>{ typedef T type;};


class SudokuValidator
{

public:
    template <unsigned SQLENGTH>
    bool IsValid(unsigned (&puzzle)[SQLENGTH][SQLENGTH])
    {

// for each row, use each value in the puzzle wrote to set up its field
        // and validate that the field


        //rows
        for (int rowid = 0; rowid < SQLENGTH ; rowid++)
        {
            std::bitset<SQLENGTH> row;
            for (int value = 0; value < SQLENGTH ; value++)
            {
            	row[puzzle[rowid][value]-1] = 1;
            }
            row.flip();
            if (row.any()) return false;
        }

        //cols
        for (int colid = 0; colid < SQLENGTH ; colid++)
        {
            std::bitset<SQLENGTH> col;
            for (int value = 0; value < SQLENGTH ; value++)
            {
                col[puzzle[colid][value]-1] = 1;
            }
            col.flip();
            if (col.any()) return false;
        }

        //boxes
        {
            std::bitset<SQLENGTH> box;
        }
        return true; 
    }

};

unsigned test2[9][9] = {
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1}
};


 unsigned test1[9][9] = 
{
    {2,4,8,3,9,5,7,1,6},
    {5,7,1,6,2,8,3,4,9},
    {9,3,6,7,4,1,5,8,2},
    {6,8,2,5,3,9,1,7,4},
    {3,5,9,1,7,4,6,2,8},
    {7,1,4,8,6,2,9,5,3},
    {8,6,3,4,1,7,2,9,5},
    {1,9,5,2,8,6,4,3,7},
    {4,2,7,9,5,3,8,6,1}
};

 unsigned test3[4][4] = 
{
    {1,2,3,4},
    {2,3,4,1},
    {3,4,1,2},
    {4,1,2,3}
};

int main()
{
    SudokuValidator* sv = new SudokuValidator();
    std::cout << "result1: " << (sv->IsValid(test1) ? "Valid" : "Invalid") << std::endl;
	static_assert(2 == ArrayDeducer<decltype(test1)>::Rank::value,"rank is not 2");
	static_assert(9 == ArrayDeducer<decltype(test1)>::Count::value,"count is not 9");
	static_assert(1 == ArrayDeducer<ArrayDeducer<decltype(test1)>::type>::Rank::value,"rank is not 1");
	static_assert(9 == ArrayDeducer<ArrayDeducer<decltype(test1)>::type>::Count::value,"count is not 9");
	static_assert(0 == ArrayDeducer<ArrayDeducer<ArrayDeducer<decltype(test1)>::type>::type>::Rank::value,"rank is not 0");	return 0;
	static_assert(0 == ArrayDeducer<ArrayDeducer<ArrayDeducer<decltype(test1)>::type>::type>::Count::value,"count is not 0");	return 0;
	static_assert(IntegerSquareRootOf<sizeof(test1)/sizeof(unsigned)>::isPerfect, "puzzle size not square");
 	static_assert(9 == IntegerSquareRootOf<sizeof(test1)/sizeof(unsigned)>::result, "puzzle size not 9x9");
    test1[0][0] = 1;
    std::cout << "result1 modified: " << (sv->IsValid(test1) ? "Valid" : "Invalid") << std::endl;
    system("pause");


	static_assert(2 ==  IntegerSquareRootOf<4>::result, "IntegerSquareRootOf(4) != 2");
	static_assert(9 ==  IntegerSquareRootOf<81>::result, "IntegerSquareRootOf(81) != 9");
	static_assert(32==  IntegerSquareRootOf<1024>::result, "IntegerSquareRootOf(1024) != 32");
	static_assert(64==  IntegerSquareRootOf<4096>::result, "IntegerSquareRootOf(4096) != 64");
	static_assert(128== IntegerSquareRootOf<16384>::result, "IntegerSquareRootOf(16384) != 128");
    static_assert(150== IntegerSquareRootOf<150*150>::result, "IntegerSquareRootOf(65536) != 256");
    //static_assert(1024==IntegerSquareRootOf<1048756>::result, "IntegerSquareRootOf(1048756) != 1024");
	system("pause");

	return 0;
}
