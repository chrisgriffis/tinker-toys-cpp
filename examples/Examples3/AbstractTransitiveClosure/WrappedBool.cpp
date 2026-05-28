#include "WrappedBool.h"

//need to do this because I can't have a bool as a class

WrappedBool::WrappedBool():val(){}
WrappedBool::WrappedBool( bool b):val(b){}
WrappedBool::WrappedBool( int i)
{
    val = ((i==0) ? false:true);
}

WrappedBool::WrappedBool( const WrappedBool& b):val(b.val){}
WrappedBool::operator bool() const{return val;}
WrappedBool& WrappedBool::operator=( const WrappedBool& sb)
{
    val = sb.val;
    return *this;
}

WrappedBool& WrappedBool::operator=( const bool& b)
{
    val = b;
    return *this;
}
