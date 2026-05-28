// Test to ensure conversion between units is allowed
#include "precompiled.h"
#include "Distance.h"
#include "Mass.h"
#include "Angle.h"
#include "CommonDataTypes.h"
using namespace Insitu::Math;

void distance()
{
    Meters m = Meters(3);
    Feet f = m; // Constructor conversion
    f = m;      // Also check assignment operator

    Meters m2 = f;  // Opposite direction
    m2 = f;

    // Implicit conversion to double (provided by UnitBase)
    double result = m;
    double result2 = f;
    double result3 = m + f;

    result = m;
    result = f;
    result = m+f;

    // To DDS
    Insitu::DDS::Distance dds = m;
    Insitu::DDS::Distance dds2 = f;
    dds = f;
    dds = m;

    m = dds;
    f = dds;
    Meters mFromDDS = dds;
    Feet fFromDDS = dds;
}

void time()
{
    Seconds s = Seconds(2);
    Milliseconds ms = s;       // Constructor conversion
    ms = s;                    // Assignment operator

    Seconds s2 = ms;           // opposite direction
    s2 = ms;

    // To DDS
    Insitu::DDS::TimeSpan ts = s;
    Insitu::DDS::TimeSpan ts2 = ms;
    ts = s;
    ts = ms;

    s = ts;
    ms = ts;
    Seconds sFromDDS = ts;
    Milliseconds msFromDDS = ts;
}

void mass()
{
    Pounds lb = Pounds(3);
    Kilograms kg = lb;
    lb = kg;

    Pounds lb2 = kg;
    kg = lb;

    // To DDS
    Insitu::DDS::Mass dds = lb;
    Insitu::DDS::Mass dds2 = kg;

    dds = lb;
    dds = kg;

    Kilograms kgFromDDS = dds;
    Pounds lbFromDDS = dds;


}

void angle()
{
    Degrees deg = Degrees(1);
    Radians rad = deg;
    Revolutions rev = deg;

    Radians ra2 = Radians(2);
    Degrees deg2 = ra2;
    Revolutions rev2 = ra2;

    Revolutions rev3 = Revolutions(3);
    Degrees deg3 = rev3;
    Radians ra3 = rev3;

    deg = rad;
    deg = rev;

    rad = deg;
    rad = rev;

    rev = deg;
    rev = rad;

    // DDS conversion
    Insitu::DDS::Angle ddsFromDeg = deg;
    Insitu::DDS::Angle ddsFromRad = rad;
    Insitu::DDS::Angle ddsFromRev = rev;

    ddsFromDeg = deg;
    ddsFromRad = rad;
    ddsFromRev = rev;

    deg = ddsFromDeg;
    rad = ddsFromRad;
    rev = ddsFromRev;

    Degrees deg4 = ddsFromDeg;
    Radians ra4 = ddsFromRad;
    Revolutions rev4 = ddsFromRev;

    // Check angular velocity templates
    RadiansPerSecond rps = RadiansPerSecond(3);
    RevolutionsPerMinute rpm = RevolutionsPerMinute(4);



}

