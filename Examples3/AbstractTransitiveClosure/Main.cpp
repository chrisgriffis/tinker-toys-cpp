
#include "helper.h"
#include "GenericExponentiation.h"
#include "AlgebraicBool.h"
#include "AlgebraicMatrix.h"


//in main function,
int main()
{
    bool friendsArray[7][7] = 
    {
        {1,1,0,1,0,0,0},
        {1,1,0,0,0,1,0},
        {0,0,1,1,0,0,0},
        {1,0,1,1,0,1,0},
        {0,0,0,0,1,0,1},
        {0,1,0,1,0,1,0},
        {0,0,0,0,1,0,1}
    };
    AlgebraicMatrix<AlgebraicBool,7> friends;
    for (int i = 0; i < 7 ; i++)
    {
        for (int j = 0; j < 7 ; j++)
        {
            friends.arr[i][j]=AlgebraicBool(friendsArray[i][j]);
        }
    }

    printSqArr(friends.arr);
    AlgebraicMatrix<AlgebraicBool,7> X = 
        power_monoid(friends,11);
    printSqArr(X.arr);
    return 0;
}


