#include "precompiled.h"
#include "Distance.h"
#include "Time.h"
using namespace Insitu::Math;


void test()
{
    Meters m(3.5);
    Seconds s(4.5);
    MetersPerSecond mps = m/s;
    MultipliedUnit<Meters, Seconds> ms = m * s;

    double result;

#if defined CASE_1
    result = m + s;
#elif defined CASE_2
    result = m - s;
#elif defined CASE_3
    result = mps + m;
#elif defined CASE_4
    result = mps - m;
#elif defined CASE_5
    result = m + mps;
#elif defined CASE_6
    result = m - mps;
#elif defined CASE_7
    result = ms + m;
#elif defined CASE_8
    result = ms - m;
#elif defined CASE_9
    result = m + ms;
#elif defined CASE_10
    result = m - ms;
#else 
#error OK (No test case in this slot)
#endif
}
