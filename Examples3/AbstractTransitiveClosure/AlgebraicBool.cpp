#include "AlgebraicBool.h"

AlgebraicBool::AlgebraicBool(void)
{
}

AlgebraicBool::AlgebraicBool( bool ab)
{
    val = ab;
}

AlgebraicBool::~AlgebraicBool(void)
{
}

AlgebraicBool AlgebraicBool::Identity()
{
    AlgebraicBool retval;
    retval.val = false;
    return retval;
}

AlgebraicBool AlgebraicBool::Unity()
{
    AlgebraicBool retval;
    retval.val = true;
    return retval;
}
