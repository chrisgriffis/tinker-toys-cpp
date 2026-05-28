/**
 * Angle/revolution units
 * Part of the Insitu Unit Library
 */

#pragma once
#include "UnitBase.h"
#include "CommonDataTypes.h"
#include "Time.hpp"

namespace Insitu
{
    namespace Math 
    {
        class Degrees;
        class Radians;
        class Revolutions;

        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Angle, Degrees);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Angle, Radians);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Angle, Revolutions);

        class Degrees : public UnitBase<Degrees>
        {
        public:
            inline explicit Degrees(double value):UnitBase(value) {}
            inline Degrees(Degrees const &copy):UnitBase(static_cast<double>(copy)) {}

            Degrees(Radians const &);
            Degrees(Revolutions const &);
            Degrees(Insitu::DDS::Angle const &);

            operator Radians() const;
            operator Revolutions() const;
            operator Insitu::DDS::Angle() const;

            using UnitBase<Degrees>::operator =;
        };

        class Radians : public UnitBase<Radians>
        {
         public:
            inline explicit Radians(double value):UnitBase(value) {}
            inline Radians(Radians const &copy):UnitBase(static_cast<double>(copy)) {}

            Radians(Degrees const &);
            Radians(Revolutions const &);
            Radians(Insitu::DDS::Angle const &);

            operator Degrees() const;
            operator Revolutions() const;
            operator Insitu::DDS::Angle() const;

            using UnitBase<Radians>::operator =;       
        };

        class Revolutions : public UnitBase<Revolutions>
        {
          public:
            inline explicit Revolutions(double value):UnitBase(value) {}
            inline Revolutions(Revolutions const &copy):UnitBase(static_cast<double>(copy)) {}

            Revolutions(Degrees const &);
            Revolutions(Radians const &);
            Revolutions(Insitu::DDS::Angle const &);

            operator Degrees() const;
            operator Radians() const;
            operator Insitu::DDS::Angle() const;

            using UnitBase<Revolutions>::operator =;              
        };

        // Some useful typedefs
        typedef DividedUnit<Degrees, Seconds> DegreesPerSecond;
        typedef DividedUnit<Radians, Seconds> RadiansPerSecond;
        typedef DividedUnit<Revolutions, Seconds> RevolutionsPerSecond;
        typedef DividedUnit<Revolutions, Minutes> RevolutionsPerMinute;
    }
}