#include "precompiled.h"
#include "Distance.h"
using namespace Insitu::Math;

void add()
{
    Meters m1(3.5), m2(4.5);
    Seconds s1(-2.5), s2(3.5);
    Meters m = m1 + m2;
    Seconds s = s1 + s2;

    MetersPerSecond v = m1 / s1 + m2 / s2;
}
void subtract()
{
    Meters m1(3.5), m2(4.5);
    Seconds s1(-2.5), s2(3.5);
    Meters m = m1 - m2;
    Seconds s = s1 - s2;

    MetersPerSecond v = m1 / s1 - m2 / s2;
}
void multiply()
{
    Meters m1(3.5), m2(4.5);
    Seconds s1(-2.5), s2(3.5);
    m1 * m2;
    s1 * s2;

    MultipliedUnit<MultipliedUnit<Meters, Seconds>, Meters> mms = m1 * s1 * m1;
}
void divide()
{
    Meters m1(3.5), m2(4.5);
    Seconds s1(-2.5), s2(3.5);
    Meters m = m1 + m2;
    Seconds s = s1 + s2;

    DividedUnit<Meters, MultipliedUnit<Seconds, Seconds> > a = m1 / s1 / s2;
    DividedUnit<MultipliedUnit<Meters, Meters>, MultipliedUnit<Seconds, Seconds> > a2 = a * m1;
    DividedUnit<Meters, Seconds> a3 = a * s1;
    DividedUnit<MultipliedUnit<Meters, Meters>, Seconds> a4 = a2 * s1;
}