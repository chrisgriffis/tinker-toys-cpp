#pragma once

//not allowed in this exercise!!
// #include <exception>

namespace Griffis
{
    class MyExceptionBase //: public std::exception
    {
        virtual const char* what() const throw() = 0;
    };

    class UnitializedObjectException : public MyExceptionBase
    {
        virtual const char* what() const throw()
        {
            return "Attempted use of uninitialized object";
        }
    };
    class NullPointerException : public MyExceptionBase
    {
        virtual const char* what() const throw()
        {
            return "Attempted to use object with NULL pointer";
        }
    };

}
