// Implementation for Time unit functions
#include "precompiled.h"
#include "Angle.h"
#include "InsituConstants.h"

using Insitu::Math::Degrees;
using Insitu::Math::Radians;
using Insitu::Math::Revolutions;

Degrees::Degrees(Radians const &rad):UnitBase(static_cast<double>(rad) * DEGREES_PER_RADIAN)
{
}

Degrees::Degrees(Revolutions const &rev):UnitBase(static_cast<double>(rev) * 360.0)
{
}

Degrees::Degrees(Insitu::DDS::Angle const &angle):UnitBase(angle.InRadians * DEGREES_PER_RADIAN)
{
}

Degrees::operator Radians() const 
{
    Radians rad = *this;
    return rad;
}

Degrees::operator Revolutions() const 
{
    Revolutions rev = *this;
    return rev;
}

Degrees::operator Insitu::DDS::Angle() const 
{
    Insitu::DDS::Angle dds;
    dds.InRadians = static_cast<double>(*this) * RADIANS_PER_DEGREE;
    return dds;
}

Radians::Radians(Degrees const &deg):UnitBase(static_cast<double>(deg) * RADIANS_PER_DEGREE)
{
}

Radians::Radians(Revolutions const &rev):UnitBase(static_cast<double>(rev) * PI_TIMES_2)
{
}

Radians::Radians(Insitu::DDS::Angle const &angle):UnitBase(angle.InRadians)
{
}

Radians::operator Degrees() const 
{
    Degrees deg = *this;
    return deg;
}

Radians::operator Revolutions() const 
{
    Revolutions rev = *this;
    return rev;
}

Radians::operator Insitu::DDS::Angle() const 
{
    Insitu::DDS::Angle dds;
    dds.InRadians = static_cast<double>(*this);
    return dds;
}

Revolutions::Revolutions(Degrees const &deg):UnitBase(static_cast<double>(deg) / 360.0)
{
}

Revolutions::Revolutions(Radians const &rad):UnitBase(static_cast<double>(rad) / PI_TIMES_2)
{
}

Revolutions::Revolutions(Insitu::DDS::Angle const &angle):UnitBase(angle.InRadians / PI_TIMES_2)
{
}

Revolutions::operator Degrees() const 
{
    Degrees deg = *this;
    return deg;
}

Revolutions::operator Radians() const 
{
    Radians rad = *this;
    return rad;
}

Revolutions::operator Insitu::DDS::Angle() const 
{
    Insitu::DDS::Angle dds;
    dds.InRadians = static_cast<double>(*this) * PI_TIMES_2;
    return dds;
}


