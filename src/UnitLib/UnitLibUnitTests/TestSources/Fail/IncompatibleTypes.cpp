#include "precompiled.h"
#include "Distance.h"
#include "Time.h"
using namespace Insitu::Math;

#if defined CASE_1
Meters meters = Meters(3.5);
Seconds seconds = meters;
#elif defined CASE_2
Seconds seconds = Meters(3.5);
#elif defined CASE_3
Meters meters = Meters(3.5);
Seconds seconds = Seconds(2.5);
meters = seconds;
#elif defined CASE_4
Meters meters = Meters(3.5);
Seconds seconds = Seconds(2.5);
seconds = meters;
#elif defined CASE_5
Meters meters = Meters(3.5);
Seconds seconds = Seconds(2.5);
void* temporary = (void*) (int)(meters + seconds);
#elif defined CASE_6
MetersPerSecond mps = MetersPerSecond(2);
MetersPerSecondPerSecond accel = MetersPerSecondPerSecond(3);
accel = mps;
#elif defined CASE_7
MetersPerSecond mps = MetersPerSecond(2);
MetersPerSecondPerSecond accel = mps;
#elif defined CASE_8
MultipliedUnit<Meters, Seconds> ms(2);
DividedUnit<Meters, Seconds> mps(3);
ms = mps;
#elif defined CASE_9
MultipliedUnit<Meters, Seconds> ms(2);
DividedUnit<Meters, Seconds> mps(3);
void* temporary = (void*) (int)(ms + mps);
#else
#error OK (No test case in this slot)
#endif 



