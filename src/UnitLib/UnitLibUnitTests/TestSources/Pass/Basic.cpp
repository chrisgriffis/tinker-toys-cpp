#include "precompiled.h"
#include "Distance.h"
#include "Time.h"

using namespace Insitu::Math;

Meters distance = Meters(4);
Seconds t = Seconds(3);

MetersPerSecond velocity = distance / t;
MetersPerSecondPerSecond acceleration = distance / t / t;
Meters distance2 = velocity * t;

MultipliedUnit<Meters, Seconds> dt = distance * t;