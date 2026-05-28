#pragma once
#include "helper.h"

struct WrappedBool //act like a bool but i can inherit from you
{
    WrappedBool();
    WrappedBool(bool);
    WrappedBool(int);
    WrappedBool(const WrappedBool&);
    WrappedBool& operator=(const WrappedBool&);
    WrappedBool& operator=(const bool&);
    operator bool() const;
    bool val;
};