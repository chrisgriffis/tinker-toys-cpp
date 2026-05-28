#include "Bar.h"
#include <cstring>

Bar::Bar(void)
{
}

Bar::Bar( const Bar& other )
{
    memcpy_s(this->_baz,sizeof(this->_baz),other._baz,sizeof(this->_baz));
}

Bar::~Bar(void)
{
}

int Bar::f1( int )
{
     return 0;
}
