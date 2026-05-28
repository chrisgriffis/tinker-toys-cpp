#include "helper.h"

struct TropicalDouble //act like a double but know about tropical algebra
{
    TropicalDouble();
    TropicalDouble(double);
    TropicalDouble operator+(const TropicalDouble& rhs);
    TropicalDouble operator*(const TropicalDouble& rhs);
    operator double() const;
    double val;
};

