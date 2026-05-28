/**
 * Mass units
 * Part of the Insitu Unit Library
 *
 */

#pragma once
#include "UnitBase.h"
#include "CommonDataTypes.h"

namespace Insitu
{
    namespace Math
    {
        class Pounds;
        class Kilograms;

        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Mass, Kilograms);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::Mass, Pounds);

        class Kilograms : public UnitBase<Kilograms>
        {
        public:
            explicit inline Kilograms(double value):UnitBase(value) {}

            inline Kilograms(Kilograms const &copy):UnitBase(copy) {}
            inline Kilograms(Insitu::DDS::Mass const &copy):UnitBase(copy.InKilograms) {}
            using UnitBase<Kilograms>::operator =;

            inline Kilograms(Pounds const &pounds);

            inline operator Pounds() const;
            inline operator Insitu::DDS::Mass() const;
        };

        class Pounds : public UnitBase<Pounds>
        {
        public:
            explicit inline Pounds(double value):UnitBase(value) {}

            inline Pounds(Pounds const &copy):UnitBase(copy) {}
            inline Pounds(Insitu::DDS::Mass &copy):UnitBase(Kilograms(copy.InKilograms)) {}
            using UnitBase<Pounds>::operator =;

            inline Pounds(Kilograms const &kg);

            inline operator Kilograms() const;
            inline operator Insitu::DDS::Mass() const;
        };
    }
}