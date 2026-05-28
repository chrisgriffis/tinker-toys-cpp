#include "precompiled.h"

#include "UnitBase.h"

#include "Mass.h"
#include "Distance.h"
#include "Time.h"

void tests()
{
    using namespace Insitu::Math;

    Meters distance = Meters(4);
    Seconds time = Seconds(3);

    // Units stick around through operations; this knowledge is retained by the compiler
    // via template metaprogramming. Only the basic types are defined. MetersPerSecond is 
    // actually a typedef to a template definition of "the type you get when you divide meters by seconds"
    MetersPerSecond velocity = distance / time;
    MetersPerSecondPerSecond acceleration = velocity / time;

    // Or you can skip the intermediary step altogether:
    MetersPerSecondPerSecond alsoAcceleration = distance / time / time;

    // Unit conversion is also built in:
    FeetPerSecond fps = velocity; // This will automatically do the appropriate scaling meters->feet

    // All of the basic operations are supported. But in all cases it enforces that the type is correct.
    velocity = velocity + MetersPerSecond(4.2);

    // You are not allowed to do something like
    // velocity = velocity + 3; because the compiler cannet be sure that "3" is actually in m/s, etc.

    // Comparison operators are supported, but only for valid comparisons
    fps < fps; // legal - same type
    fps < velocity; // legal - can be converted


#if FALSE
    // But you are protected against invalid constructs
    Meters notReallyMeters = velocity; //  error C2440: 'initializing' : cannot convert from 'Insitu::Math::MetersPerSecond' to 'Insitu::Math::Meters'
    MetersPerSecond wrongOperatorUsed = distance * time; // error C2440: 'initializing' : cannot convert from 'Insitu::Math::MultipliedUnit<A,B>' to 'Insitu::Math::DividedUnit<N,D>'
#endif 

#if FALSE
    velocity < acceleration; // error C2679: binary '=' : no operator found which takes a right-hand operand of type 'const Insitu::Math::MultipliedUnit<A,B>' (or there is no acceptable conversion)
                             // ... see reference to function template instantiation 'bool Insitu::Math::operator <<Insitu::Math::DividedUnit,Insitu::Math::DividedUnit,Insitu::Math::Meters,Insitu::Math::Seconds,Insitu::Math::Meters,Insitu::Math::MultipliedUnit<A,B>>(const Insitu::Math::DividedUnit<N,D> &,const Insitu::Math::DividedUnit<N,Insitu::Math::MultipliedUnit<A,B>> &)' being compiled

    MetersPerSecond mps = acceleration; // error C2679: binary '=' : no operator found which takes a right-hand operand of type 'const Insitu::Math::MultipliedUnit<A,B>' (or there is no acceptable conversion)
                                        // ... see reference to function template instantiation 'Insitu::Math::DividedUnit<N,D>::DividedUnit<Insitu::Math::Meters,Insitu::Math::MultipliedUnit<A,B>>(const Insitu::Math::DividedUnit<N,Insitu::Math::MultipliedUnit<A,B>> &)' being compiled
#endif


    
#if FALSE
    // You are also protected against constructs that are unsafe (i.e., the compiler can't be sure that
    // the units are the same) such as attempting to add a scalar
    velocity = velocity + 3.0;

    /* 
    1>d:\dev\main\system\unitlib\main.cpp(41) : error C2679: binary '+' : no operator found which takes a right-hand operand of type 'double' (or there is no acceptable conversion)
    1>        d:\dev\main\system\unitlib\unitbase.h(214): could be 'Insitu::Math::DividedUnit<N,D> Insitu::Math::DividedUnit<N,D>::operator +(const Insitu::Math::DividedUnit<N,D> &) const'
    1>        with
    1>        [
    1>            N=Insitu::Math::Meters,
    1>            D=Insitu::Math::Seconds
    1>        ]
    1>        while trying to match the argument list '(Insitu::Math::MetersPerSecond, double)'
    */
#endif

#if FALSE 
    // The error isn't always entirely readable in advanced cases, but it always takes you to the line that caused the error
    // if you look at the last line reported. We may be able to clean this up later.
    acceleration = acceleration / time / time / time / time / time;

    /*
    1>d:\dev\main\system\unitlib\unitbase.h(94) : error C2679: binary '=' : no operator found which takes a right-hand operand of type 'const Insitu::Math::MultipliedUnit<A,B>' (or there is no acceptable conversion)
    1>        with
    1>        [
    1>            A=Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::Seconds,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,
    1>            B=Insitu::Math::Seconds
    1>        ]
    1>        d:\dev\main\system\unitlib\unitbase.h(307): could be 'Insitu::Math::UnitBase<T> &Insitu::Math::UnitBase<T>::operator =(const Insitu::Math::UnitBase<T> &)'
    1>        with
    1>        [
    1>            T=Insitu::Math::Seconds
    1>        ]
    1>        while trying to match the argument list '(Insitu::Math::Seconds, const Insitu::Math::MultipliedUnit<A,B>)'
    1>        with
    1>        [
    1>            A=Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::Seconds,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,
    1>            B=Insitu::Math::Seconds
    1>        ]
    1>        d:\dev\main\system\unitlib\unitbase.h(209) : see reference to function template instantiation 'const Insitu::Math::MultipliedUnit<A,B> &Insitu::Math::MultipliedUnit<A,B>::operator =<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<A,B>,B>,B>,B>,B>,Insitu::Math::Seconds>(const Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<A,B>,B>,B>,B>,B>,B> &)' being compiled
    1>        with
    1>        [
    1>            A=Insitu::Math::Seconds,
    1>            B=Insitu::Math::Seconds
    1>        ]

              ******* BELOW IS THE MESSAGE TO LOOK AT *******
              This will take you to the line that caused the error

    1>        d:\dev\main\system\unitlib\main.cpp(35) : see reference to function template instantiation 'Insitu::Math::DividedUnit<N,D> Insitu::Math::DividedUnit<N,D>::operator =<N,Insitu::Math::MultipliedUnit<A,B>>(const Insitu::Math::DividedUnit<N,Insitu::Math::MultipliedUnit<A,B>> &)' being compiled
    1>        with
    1>        [
    1>            N=Insitu::Math::Meters,
    1>            D=Insitu::Math::MultipliedUnit<Insitu::Math::Seconds,Insitu::Math::Seconds>,
    1>            A=Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::MultipliedUnit<Insitu::Math::Seconds,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,Insitu::Math::Seconds>,
    1>            B=Insitu::Math::Seconds
    1>        ]
    */

#endif

   

}