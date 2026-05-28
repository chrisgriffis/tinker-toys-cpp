#pragma once

#include "Baz.h"
class Bar
{
public:
    int _baz[64];
    int f1(int);
    Bar(void);
    Bar(const Bar& other);
    ~Bar(void);
};
