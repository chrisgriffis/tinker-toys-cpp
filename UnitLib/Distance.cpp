// Implementation for Mass unit functions
#include "precompiled.h"
#include "Distance.h"

using Insitu::Math::Meters;
using Insitu::Math::Feet;

Meters::Meters(Feet const &feet):UnitBase(static_cast<double>(feet)*0.3048) {}
Feet::Feet(Meters const &meters):UnitBase(static_cast<double>(meters)*3.2808399) {}
