#pragma once

#include <exception>

namespace Griffis
{
    class UnitializedObjectException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Attempted use of uninitialized object";
        }
    };
    class NullPointerException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Attempted use object with NULL pointer";
        }
    };

}
