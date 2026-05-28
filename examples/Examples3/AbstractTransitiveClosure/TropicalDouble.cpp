#include "TropicalDouble.hpp"

TropicalDouble::TropicalDouble():val(0){}
TropicalDouble::TropicalDouble( double d):val(d){}
TropicalDouble TropicalDouble::operator+( const TropicalDouble& rhs )
{
    val = min(val,rhs.val);
    return *this;
}
TropicalDouble TropicalDouble::operator*( const TropicalDouble& rhs )
{
    val = val+rhs.val;
    return *this;
}
TropicalDouble::operator double() const{return val;}
