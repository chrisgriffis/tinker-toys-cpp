/**
 * Time units
 * Part of the Insitu Units Library
 */

#pragma once
#include "UnitBase.h"
#include "CommonDataTypes.h"

namespace Insitu
{
    namespace Math
    {
        class Milliseconds;
        class Seconds;
        class Minutes;

        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::TimeSpan, Seconds);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::TimeSpan, Milliseconds);
        UNITLIB_CREATE_CONVERSION_OPERATOR(Insitu::DDS::TimeSpan, Minutes);

        class Seconds : public UnitBase<Seconds>
        {
            static const int MILLISECONDS_IN_SECOND = 1000;
            static const int SECONDS_IN_MINUTE = 60;
        public:
            explicit inline Seconds(double value):UnitBase(value) {}

            inline Seconds(Seconds const &copy):UnitBase(copy) {}
            inline Seconds(Insitu::DDS::TimeSpan const &copy):UnitBase(copy.Milliseconds / MILLISECONDS_IN_SECOND) {}
            using UnitBase<Seconds>::operator =;

            operator Milliseconds() const;
            operator Minutes() const;
            operator Insitu::DDS::TimeSpan() const;
        };

        class Milliseconds : public UnitBase<Milliseconds> 
        {
            static const int MILLISECONDS_IN_SECOND = 1000;
            static const int SECONDS_IN_MINUTE = 60;
        public:
            explicit inline Milliseconds(double value):UnitBase(value) {}

            inline Milliseconds(Milliseconds const &copy):UnitBase(copy) {}
            inline Milliseconds(Insitu::DDS::TimeSpan const &copy):UnitBase(copy.Milliseconds) {}
            using UnitBase<Milliseconds>::operator =;

            operator Seconds() const;
            operator Minutes() const;
            operator Insitu::DDS::TimeSpan() const;
        };

        class Minutes : public UnitBase<Minutes>
        {
            static const int MILLISECONDS_IN_SECOND = 1000;
            static const int SECONDS_IN_MINUTE = 60;
        public:
            explicit inline Minutes(double value):UnitBase(value) {}

            inline Minutes(Minutes const &copy):UnitBase(copy) {}
            inline Minutes(Insitu::DDS::TimeSpan const &copy):UnitBase(copy.Milliseconds / MILLISECONDS_IN_SECOND / SECONDS_IN_MINUTE) {}
            using UnitBase<Minutes>::operator =;

            operator Seconds() const;
            operator Milliseconds() const;
            operator Insitu::DDS::TimeSpan() const;
        };

    }
}