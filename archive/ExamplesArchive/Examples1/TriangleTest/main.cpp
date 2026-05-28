#include <iostream>

#include "TriangleTest.h"
#include <map>

using namespace Griffis;
using namespace std;

int main()
{

    int again = 0;
    int a(0);
    int b(0);
    int c(0);

    map<int,const char*> m;
    m[Helper::ERROR] = "error";
    m[Helper::ISCOCELES] = "isoceles";
    m[Helper::EQUILATERAL] = "equilateral";
    m[Helper::SCALENE] = "scalene";

    do 
    {
        cout << "a: " << endl;
        cin >> a;
        cout << "b: " << endl;
        cin >> b;
        cout << "c: " << endl;
        cin >> c;

        cout << "result: " << m[Griffis::Helper::TriangleTest(a,b,c)] << endl;

        cout << "again? " << endl;
        cin >> again;

    } while (again > 0);

    system("pause");
    return 0;
}