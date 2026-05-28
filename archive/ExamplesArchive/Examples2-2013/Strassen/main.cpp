#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cstdlib>


int main()
{

    const int pow_of_2 = 128;
    int a[pow_of_2][pow_of_2];
    srand (time(NULL));
   //first array
    std::cout << pow_of_2 <<"\n\n";

    for (int i = 0; i < pow_of_2 ; i++)
    {
        for (int j = 0; j < pow_of_2 ; j++)
        {
            
            std::cout << rand() % 9 + 1 << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "\n";

    for (int i = 0; i < pow_of_2 ; i++)
    {
        for (int j = 0; j < pow_of_2 ; j++)
        {

            std::cout << rand() % 9 + 1 << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
