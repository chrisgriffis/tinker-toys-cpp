/**
 * Distance units
 * Part of the Insitu Unit Library
 */

#pragma once
#include "UnitBase.h"
#include "Time.hpp"
#include "CommonDataTypes.h"

namespace Insitu
{
    namespace Math
    {
        class Feet;
        class Meters;

        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Distance, Meters);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Distance, Feet);

        class Meters : public UnitBase<Meters>
        {
        protected:
            typedef Insitu::DDS::Distance DDSType;
        public:
            explicit inline Meters(double value):UnitBase(value) {}

            inline Meters(Meters const &copy):UnitBase(copy) {}
            inline Meters(Insitu::DDS::Distance const &copy):UnitBase(copy.InMeters) {}
            using UnitBase<Meters>::operator =;
            Meters const & operator =(Insitu::DDS::Distance const &rhs) { *this = Meters(rhs); return *this; }

            operator Feet() const;
            operator Insitu::DDS::Distance() const;

            Meters(Feet const &feet);
        };

        class Feet : public UnitBase<Feet>
        {
        protected:
            typedef Insitu::DDS::Distance DDSType;
        public:
            explicit inline Feet(double value):UnitBase(value) {}

            inline Feet(Feet const &copy):UnitBase(copy) {}
            inline Feet(Insitu::DDS::Distance const &copy):UnitBase(Meters(copy.InMeters)) {};
            using UnitBase<Feet>::operator =;
            Feet const & operator =(Insitu::DDS::Distance const &rhs) { *this = Feet(rhs); return *this; }

            operator Meters() const; 
            operator Insitu::DDS::Distance() const;

            Feet(Meters const &feet);
        };

        // Some typedefs that are useful
        typedef DividedUnit<Meters, Seconds> MetersPerSecond;
        typedef DividedUnit<Feet, Seconds> FeetPerSecond;
        typedef DividedUnit<Meters, MultipliedUnit<Seconds, Seconds> > MetersPerSecondPerSecond;
        typedef DividedUnit<Feet, MultipliedUnit<Seconds, Seconds> > FeetPerSecondPerSecond;

    }
}