// create 2 sets of characters
// print out the characters that are common to both sets
// print out the characters that are unique to both sets
// for extra credit provide an ordering function


#include <set>
#include <vector>
#include <algorithm>
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

using namespace std;

void printset(std::set<char>& s)
{
    for (std::set<char>::iterator it = s.begin(); it != s.end(); it++ )
    {
        printf("%c ",*it);
    }
    printf("\n");
}
void printset(std::vector<char>& v)
{
    for (std::vector<char>::iterator it = v.begin(); it != v.end(); it++ )
    {
        printf("%c ",*it);
    }
    printf("\n");
}

int main(int argc, char **argv)
{

    const char lookup[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

    srand(time(NULL));

    std::set<char> set1;
    std::set<char> set2;

    for (int i = 0; i < 10 ; i++)
    {
        set1.insert(lookup[rand() % 26]);
        set2.insert(lookup[rand() % 26]);
    }

    printset(set1);
    printset(set2);


    std::vector<char>::iterator resultit;
    std::vector<char> setintersec(20);
    std::vector<char> setdiff(20);

    resultit = set_intersection(set1.begin(),set1.end(),set2.begin(),set2.end(),setintersec.begin());
    printset(setintersec);

    resultit = set_difference(set1.begin(),set1.end(),set2.begin(),set2.end(),setdiff.begin());
    printset(setdiff);
    system("pause");
    return 0;
}