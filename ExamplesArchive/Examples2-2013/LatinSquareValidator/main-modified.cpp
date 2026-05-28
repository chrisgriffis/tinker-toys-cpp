//Sudoku Validator

#include <set>
#include <iostream>

// class SudokuUints
// {
//     typedef SymbolType unsigned int;
// };
// 
// template<class PuzzleEntryType = SudokuUints>
template<class DERIVED>
class LatinSquareChecker
{
public:
    template <unsigned SQLENGTH>
    bool IsValid(unsigned (&arr)[SQLENGTH][SQLENGTH])
    {
        return static_cast<DERIVED*>(this)->IsValid(arr);
    }
};

 template <unsigned N>
 struct ISQRT
 {
     enum Chk { N_SQUARED = N * N };

    template <unsigned GUESS>
    struct Guess
    {
        const static unsigned result = typename ISQRT<N>::GuessImpl<GUESS>::result;
        const static unsigned isValidResult = (result * result == N);
    };

    template <unsigned GUESS>
    struct GuessImpl
    {
        const static unsigned result = ISQRT<N>::Guess<GUESS+1>::result;
    };

    template <>
    struct GuessImpl<N_SQUARED>
    {
        const static unsigned result = N;
    };
 };


class TwoByTwoValidator : public LatinSquareChecker<TwoByTwoValidator>
{
public:
    template <unsigned SQLENGTH>
    bool IsValid(unsigned (&arr)[SQLENGTH][SQLENGTH])
    {
        return SQLENGTH == 2*2;
    }
};

class SudokuValidator : public LatinSquareChecker<SudokuValidator>
{

public:
    template <unsigned SQLENGTH>
    bool IsValid(unsigned (&puzzle)[SQLENGTH][SQLENGTH])
    {
		if(!ISQRT<SQLENGTH>::isValidResult) return false;
        std::set<unsigned> rows[SQLENGTH];
        std::set<unsigned> cols[SQLENGTH];
        std::set<unsigned> subsqs[SQLENGTH];
        for(unsigned i=0; i<SQLENGTH; i++)
        {
            rows[i].clear();
            cols[i].clear();
            subsqs[i].clear();
        }
        for(unsigned i=0; i<SQLENGTH; i++)
            for(unsigned j=0; j<SQLENGTH; j++)
            {
                unsigned temp = puzzle[i][j];
                if(temp <= SQLENGTH && temp > 0)
                {
                    rows[i].insert(temp);
                    cols[j].insert(temp);
					subsqs[i/ISQRT<SQLENGTH>::result + ISQRT<SQLENGTH>::result*(j/ISQRT<SQLENGTH>::result)].insert(temp);
                }
            }
            bool retval = true;
            for(unsigned i=0; i<SQLENGTH; i++)
            {
                retval = 
                    retval && 
                    (rows[i].size() == SQLENGTH) &&
                    (cols[i].size() == SQLENGTH) &&
                    (subsqs[i].size() == SQLENGTH);
            }
            return retval;


    }

};

static unsigned test2[9][9] = {{-1}};

static unsigned test1[9][9] = 
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


static unsigned test3[4][4] = 
{
    {1,2,3,4},
    {2,3,4,1},
    {3,4,1,2},
    {4,1,2,3}
};

int main()
{
    LatinSquareChecker<SudokuValidator>* sv = new SudokuValidator();
    std::cout << "result1: " << (sv->IsValid(test1) ? "Valid" : "Invalid") << std::endl;
    test1[4][4] = 4;
    test1[4][5] = 7; 
    std::cout << "result1 modified: " << (sv->IsValid(test1) ? "Valid" : "Invalid") << std::endl;
    test2[5][5] = 5;
    std::cout << "result2: " << (sv->IsValid(test2) ? "Valid" : "Invalid") << std::endl;
    LatinSquareChecker<TwoByTwoValidator>* tbtv = new TwoByTwoValidator();
    std::cout << "result tbtv on 2x2: " << (tbtv->IsValid(test3) ? "Valid" : "Invalid") << std::endl;
    std::cout << "result sv on 2x2: " << (sv->IsValid(test3) ? "Valid" : "Invalid") << std::endl;


//    std::cout << "template meta 4: "<< ISQRT<4>::Guess<1, ISQRT>::result << ", isvalid: " << (ISQRT<4>::isValidResult ? "Valid" : "Invalid") << std::endl;
//    std::cout << "template meta 9: "<< ISQRT<9>::Guess<1, ISQRT>::result << ", isvalid: " << (ISQRT<9>::isValidResult ? "Valid" : "Invalid") << std::endl;
//    std::cout << "template meta 256: "<< ISQRT<64>::Guess<1, ISQRT>::result << ", isvalid: " << (ISQRT<64>::isValidResult ? "Valid" : "Invalid") << std::endl;
    std::cout << "template meta 10: "<< ISQRT<1024>::Guess<1>::result << ", isvalid: " << (ISQRT<1024>::Guess<1>::isValidResult ? "Valid" : "Invalid") << std::endl;
    system("pause");
}
