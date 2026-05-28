#include "precompiled.h"
#include "Distance.h"
using namespace Insitu::Math;

void basicTest()
{
    Meters a(3.5), b(3.5);
    Feet c(4.5);

    bool result;

    // Check each operation is legal
    result = a < b;
    result = a <= b;
    result = a == b;
    result = a != b;
    result = a >= b;
    result = a > b;

    // Check conversion is also legal
    result = a < c;  result = c < a;
    result = a <= c; result = c <= a;
    result = a == c; result = c == a;
    result = a != c; result = c != a;
    result = a >= c; result = c >= a;
    result = a > c;  result = c > a;
}

void dividedUnitTest()
{
    Meters a(3.5);
    Seconds b(4.5);
    MetersPerSecond c = a/b;
    MetersPerSecond d = a/b;

    bool result;

    // Check each operation's legality
    result = c < d;
    result = c <= d;
    result = c == d;
    result = c != d;
    result = c >= d;
    result = c > d;
}

void multipliedUnitTest()
{
    Meters a(3.5);
    Seconds b(4.5);
    MultipliedUnit<Meters, Seconds> c = a*b;
    MultipliedUnit<Meters, Seconds> d = a*b;

    bool result;

    // Check each operation's legality
    result = c < d;
    result = c <= d;
    result = c == d;
    result = c != d;
    result = c >= d;
    result = c > d;
}



