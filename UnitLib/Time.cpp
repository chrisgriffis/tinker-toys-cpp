// Implementation for Time unit functions
#include "precompiled.h"
#include "Time.hpp"

using Insitu::Math::Seconds;
using Insitu::Math::Milliseconds;
using Insitu::Math::Minutes;

Seconds::operator Milliseconds() const 
{
    return Milliseconds(static_cast<double>(*this) * MILLISECONDS_IN_SECOND);
}

Seconds::operator Minutes() const 
{
    return Minutes(static_cast<double>(*this) / SECONDS_IN_MINUTE);
}

Seconds::operator Insitu::DDS::TimeSpan() const 
{
    Insitu::DDS::TimeSpan ts;
    ts.Milliseconds = static_cast<double>(*this) * MILLISECONDS_IN_SECOND;
    return ts;
}

Milliseconds::operator Seconds() const 
{
    return Seconds(static_cast<double>(*this) / MILLISECONDS_IN_SECOND);
}

Milliseconds::operator Minutes() const 
{
    return Minutes(static_cast<double>(*this) / MILLISECONDS_IN_SECOND / SECONDS_IN_MINUTE);
}

Milliseconds::operator Insitu::DDS::TimeSpan() const 
{
    Insitu::DDS::TimeSpan ts;
    ts.Milliseconds = static_cast<double>(*this);
    return ts;
}

Minutes::operator Milliseconds() const 
{
    return Milliseconds(static_cast<double>(*this) * MILLISECONDS_IN_SECOND * SECONDS_IN_MINUTE);
}

Minutes::operator Seconds() const 
{
    return Seconds(static_cast<double>(*this) * SECONDS_IN_MINUTE);
}

Minutes::operator Insitu::DDS::TimeSpan() const 
{
    Insitu::DDS::TimeSpan ts;
    ts.Milliseconds = static_cast<double>(*this) * MILLISECONDS_IN_SECOND * SECONDS_IN_MINUTE;
    return ts;
}