#pragma once
#include "WrappedBool.h"
#include "SemiRing.h"
#include "Identity.h"
#include "And.h"
#include "Or.h"

struct WrappedBool;

class AlgebraicBool : 
    public WrappedBool,
    public SemiRing<
        WrappedBool, 
        Or<WrappedBool>,
        And<WrappedBool>
        >,
    public ImbueIdentity<AlgebraicBool>, 
    public ImbueUnity<AlgebraicBool>
{
public:

    AlgebraicBool Identity();
    AlgebraicBool Unity();

    AlgebraicBool(void);
    AlgebraicBool(bool);
    ~AlgebraicBool(void);
};
