// Implementation for Mass unit functions
#include "precompiled.h"
#include "Mass.h"

using Insitu::Math::Kilograms;
using Insitu::Math::Pounds;

Kilograms::Kilograms(Pounds const &pounds):UnitBase(static_cast<double>(pounds)*0.45359237) {}
Pounds::Pounds(Kilograms const &kg):UnitBase(static_cast<double>(kg)*2.20462262) {}
