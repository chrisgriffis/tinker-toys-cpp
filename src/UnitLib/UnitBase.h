/**
 * @file UnitBase.h
 * @author Matthew P. Del Buono
 *
 * Templates for basic unit combination operations
 * (dividing two unit types or multiplying unit types to construct
 * new unit types, etc.)
 *
 * Abandon all hope, all ye who enter here. Beyond this point, there be dragons.
 */
#pragma once

// Anonymous namespace for things that are only needed locally to this file.
// Stuff in this namespace is just for the template metaprogramming internal logic.
namespace 
{
    // This is used because MSVC doesn't properly support SFINAE, so we can use this type as a
    // trick during template metaprogramming to create operators that can never be used when we need to.
    // A template is used to allow us to create an infinite number of unusable types if necessary
    template <class T, int N = 0> class UnusableType
    {
        UnusableType(); // Private unimplemented default constructor to prevent instantiation
    };

    // This template provides a way to support detection of two type parameters being equivalent. If they are equivalent,
    // the type returned is an UnusableType, otherwise it returns type T when you query CheckUnique<T>::CompareAgainst<A>::Type.
    template <class T> struct CheckUnique {
        template <class C> struct CompareAgainst {
            typedef T Type;
        };

        template <> struct CompareAgainst<T> {
            typedef UnusableType<T> Type;
        };
    };

    // This template provides introspection to support retrieval of a type within a template.
    // For example, GetParameter<MultipliedUnit<A,B>>::Left would return A. In the event
    // that the type provided does not have two parameters (e.g., GetParameter<Gram>) then
    // the type returned is an UnusableType
    template <class T> struct GetParameter { 
        typedef UnusableType<T, 1> Left; typedef UnusableType<T, 2> Right; 
        typedef UnusableType<T, 3> LeftUnique; typedef UnusableType<T, 4> RightUnique;

        // This template must be provided to match the specialization, because VS fails at SFINAE. However,
        // we can just make it work by providing all types as forwarded, since they will be unusable anyway.
        template <template <typename, typename> class T2> struct For {
            typedef typename GetParameter<T>::Left Left;
            typedef typename GetParameter<T>::Right Right;
            typedef typename GetParameter<T>::LeftUnique LeftUnique;
            typedef typename GetParameter<T>::RightUnique RightUnique;
        };
    };
    template <template <typename, typename> class T, class M, class N> struct GetParameter<T<M,N> > { 
        typedef M Left; typedef N Right; 

        // The following two will return the same types if and only if M != N. If M == N, they return the UnusableType.
        typedef typename CheckUnique<M>::CompareAgainst<N>::Type LeftUnique;
        typedef typename CheckUnique<N>::CompareAgainst<M>::Type RightUnique;

        // The following allow for verification that this is a MultipliedUnit or DividedUnit. If not, the UnusableType is returned.
        // To use: GetParameter<MultipliedUnit<A, B>>::For<MultipliedUnit>::Left
        template <template <typename, typename> class T2> struct For {
            typedef UnusableType<T2<M, N>, 5> Left; typedef UnusableType<T2<M, N>, 6> Right;
            typedef UnusableType<T2<M, N>, 7> LeftUnique; typedef UnusableType<T2<M, N>, 8> RightUnique;
        };

        template <> struct For<T> {
            typedef typename GetParameter<T<M,N> >::Left Left;
            typedef typename GetParameter<T<M,N> >::Right Right;
            typedef typename GetParameter<T<M,N> >::LeftUnique LeftUnique;
            typedef typename GetParameter<T<M,N> >::RightUnique RightUnique;
        }; 
    };

    // Determines the result of dividing L by R. Use via GetDivisionResult<L>::WhenDividedBy<R>::Type
    // Returns DividedUnit<L,R> except if L==R, in which case it returns double (a scalar)
    template <class L, template <typename, typename> class T> struct GetDivisionResult {
        template <class R> struct WhenDividedBy {
            typedef T<L, R> Type;
        };

        template <> struct WhenDividedBy<L> {
            typedef double Type;
        };
    };
}

namespace Insitu
{
    namespace Math 
    {
        // Forward decl 
        template<typename, typename> class DividedUnit;
        template<typename, typename> class MultipliedUnit;
        template<typename> class GetDDSType;

        /**
         * Provides the definition of a basic unit from which all other units should be derived.
         * Typename T should be the derived class that is being defined (e.g., class Kilogram : UnitBase<Kilogram>). This
         * provides the template with metainformation useful for creating operators, etc.
         *
         * After providing a unit by deriving from this type, other things to provide in the class would likely be conversion
         * operators between units of the same dimension, etc, as well as an explicit constructor taking a double. The constructor
         * must be explicit to prevent unintended conversions between unit types, because all units can implicitly be turned
         * into a double.
         *
         * The DividedUnit and MultipliedUnit types are NOT derived from this class. It is only provided for convenience
         * and is not meant to express any parent-child relationship between units (which makes sense, because
         * the type T would be different for each type, so they do not inherit from the same parent in the first place)
         */
        template <class T>
        class UnitBase
        {
        private:
            double m_value;
        public:
            explicit UnitBase(double value):m_value(value) {}

            UnitBase(UnitBase<T> const &copy):m_value(copy.m_value) {}

            // Assignment operator also providing conversion mechanisms
            // Conversion will be supported naturally.
            T const & operator =(T const & rhs) 
            {
                m_value = rhs.m_value;
                return *static_cast<T*>(this);
            }; 

            // Automated handling of the DDS type
            T const & operator =(typename GetDDSType<T>::Type const & rhs)
            {
                m_value = T(rhs).m_value;
                return *static_cast<T*>(this);
            }

            // Basic multiplication operator
            template <class R> inline MultipliedUnit<T, R> operator *(R const &rhs) const { return MultipliedUnit<T, R>(*static_cast<T const *>(this), rhs); }

            // Specialization for T * (A/T) --> A
            template <class A> inline A operator *(MultipliedUnit<A, T> const &rhs) const { return A(static_cast<double>(*this) * static_cast<double>(rhs)); }

            // Basic division operator
            template <class D> inline typename GetDivisionResult<T, DividedUnit>::WhenDividedBy<D>::Type operator /(D const &rhs) const { return DividedUnit<T, D>(*static_cast<T const *>(this), rhs); }

            // Specialization of division operator - when we divide by ourselves, expect a scalar (ratio) back
            template <>
            inline double operator /<T>(T const &rhs) const { T rhsConverted = rhs; return m_value / rhsConverted.m_value; }

            // Basic addition operator - only can add like units (and convertibles as defined by that type)
            template <class R>
            inline T operator +(R const &rhs) const { T rhsConverted = rhs; return T(static_cast<double>(*this) + static_cast<double>(rhsConverted)); }

            // Basic subtraction operator, same restrictions
            template <class R>
            inline T operator -(R const &rhs) const { T rhsConverted = rhs; return T(static_cast<double>(*this) - static_cast<double>(rhsConverted)); }

            // Comparison operators. We need to use templates even though we can only really support "T const&" as the type because
            // we don't want to allow the use of the basic double comparison operator implicitly.
            template <class R> inline bool operator <(R const &rhs) const { T rhsConverted = rhs; return static_cast<double>(*this) < static_cast<double>(rhsConverted); }
            template <class R> inline bool operator <=(R const &rhs) const { T rhsConverted = rhs; return static_cast<double>(*this) <= static_cast<double>(rhsConverted); }
            template <class R> inline bool operator ==(R const &rhs) const { T rhsConverted = rhs; return static_cast<double>(*this) == static_cast<double>(rhsConverted); }
            template <class R> inline bool operator !=(R const &rhs) const { return !(operator ==(rhs)); }
            template <class R> inline bool operator >=(R const &rhs) const { return !(operator <(rhs)); }
            template <class R> inline bool operator >(R const &rhs) const { return !(operator <=(rhs)); }


            // Conversion to a scalar is provided implicit. For this reason, constructors which take doubles
            // should always be declared explicit, otherwise type mingling would be allowed implicitly
            inline operator double() const { return m_value; }
        };

        /**
         * Generic class for unit types where two units are multiplied together.
         * For example, a newton-meter would be constructed as MultipliedUnit<Newton,Meter>.
         * Because multiplication is commutative, MultipliedUnit<A,B> is fully compatible with
         * MultipliedUnit<B,A>
         */
        template <class A, class B>
        class MultipliedUnit 
        {
            template <typename, typename> friend class MultipliedUnit;  // Friendship provided so that all types can get access to m_first and m_second, as necessary for construction
        private:
            A m_first;
            B m_second;

            // This template uses template metaprogramming to retrieve type A
            // if A != B, or UnusableType otherwise. This allows us to create constructs
            // such as constructors that take both <A, B> and <B, A>, without worrying about the 
            // case where A == B which would otherwise cause a duplicate definition error.
            template <class T> struct GetFirstType { typedef A Type; };

            // This specialization is what allows this to occur. If B == A, this specialization will be used when
            // we go for GetFirstType<A>::Type, so we will get UnusableType.
            template <> struct GetFirstType<B> { typedef UnusableType<MultipliedUnit<A, B>, 0> Type; };

            // This template provides the same functionality but for the second type (B)
            template <class T> struct GetSecondType { typedef B Type; };
            template <> struct GetSecondType<A> { typedef UnusableType<MultipliedUnit<A, B>, 0> Type; };
            
        public:
            // Explicit constructor from a scalar value
            // Assume that this means value / 1 * (A / B)
            // This constructor must be explicit because we allow for the conversion from MultipliedUnit back to
            // double, so if it wasn't explicit, we would allow implicit type conversions between units.
            explicit inline MultipliedUnit(double value):m_first(value),m_second(1.0) { }

            // Basic constructor from passed-in types matching the two types
            inline MultipliedUnit(A const& first, B const& second):m_first(first),m_second(second) {}

            // Conversion constructor from commutative type
            // We have to be careful we don't redefine the previous constructor, so we use the helper templates
            // GetFirstType<A> and GetSecondType<B> to give us a useless type if A == B.
            inline MultipliedUnit(MultipliedUnit<typename GetSecondType<B>::Type, typename GetFirstType<A>::Type> const &copy):m_first(copy.m_second),m_second(copy.m_first) { }
            inline MultipliedUnit<typename GetFirstType<A>::Type, typename GetSecondType<B>::Type> const & operator =(MultipliedUnit<typename GetSecondType<B>::Type, typename GetFirstType<A>::Type> const& rhs)
            {
                m_first = rhs.m_second;
                m_second = rhs.m_first;
                return *this;
            }

            // Conversion from a compatible alternate type. This also implements the copy constructor.
            template <class C, class D>
            inline MultipliedUnit(MultipliedUnit<C, D> const &copy):m_first(0),m_second(0)
            {
                // Need to use operator = instead of copy constructor to prevent
                // calls to the explicit double constructor allowing invalid conversions
                m_first = copy.m_first;
                m_second = copy.m_second;
            }

            template <class C, class D>
            inline MultipliedUnit<A, B> const & operator=(MultipliedUnit<C, D> const& rhs)
            {
                m_first = rhs.m_first;
                m_second = rhs.m_second;
            }

            // Conversion to a scalar
            inline operator double() const { return static_cast<double>(m_first) * static_cast<double>(m_second); }

            // Conversion to the commutative type
            // But we should only define this if B != A
            inline operator MultipliedUnit<typename GetSecondType<B>::Type, typename GetFirstType<A>::Type>() const { return MultipliedUnit<B, A>(*this); }

            // Conversion to convertable types (e.g., newton-meter to pound-foot)
            template <class A2, class B2>
            inline operator MultipliedUnit<A2, B2>() const { return MultipliedUnit<A2, B2>(m_first, m_second); }

            #pragma region Arithmetic operators
            // It is only valid to perform addition on a unit with the same type.
            // (The conversion operator will take care of the case of MultipliedUnit<B, A> which is also compatible)
            template <class T>
            inline MultipliedUnit<A, B> operator +(T const& rhs) const
            {
                MultipliedUnit<A, B> rhsConverted = rhs;
                return MultipliedUnit<A,B>(static_cast<double>(*this) + static_cast<double>(rhsConverted));
            }

            // It is also only valid to perform subtraction on a unit with the same type.
            // By convention we keep only a single division, and keep it on the outside. (e.g., (A/B) * (C/D) --> (AC / BD))
            template <class T>
            inline MultipliedUnit<A, B> operator -(T const& rhs) const
            {
                MultipliedUnit<A, B> rhsConverted = rhs;
                return MultipliedUnit<A,B>(static_cast<double>(*this) - static_cast<double>(rhsConverted));
            }

            // Multiplication is always valid, but will result in a new unit
            template <class T> inline MultipliedUnit<MultipliedUnit<A, B>, T> operator *(T const &rhs) const
            {
                return MultipliedUnit<MultipliedUnit<A, B>, T>(*this, rhs);
            }

            // Specialize the case where we're doing (AB) * (C/A) --> BC
            template<class C> inline MultipliedUnit<B, C> operator*(DividedUnit<C, A> const &rhs) const 
            {
                return MultipliedUnit<B, C>(static_cast<double>(*this) * static_cast<double>(rhs));
            }

            // Specialize the case where we're doing (AB) * (C/B) --> AC
            // However, be careful of the situation where A == B which would match the previous specialization
            template <class C> inline MultipliedUnit<typename GetFirstType<A>::Type, C> 
            operator*(DividedUnit<C, typename GetSecondType<B>::Type> const &rhs) const
            {
                return MultipliedUnit<A, C>(static_cast<double>(*this) * static_cast<double>(rhs));
            }

            // Division is also always valid, but will result in a new unit.
            // By convention we keep division on the outside, so AB should not include a division unit.
            template <class T> inline DividedUnit<MultipliedUnit<A, B>, T> operator /(T const &rhs) const 
            {
                return DividedUnit<MultipliedUnit<A, B>, T>(*this, rhs);
            }

            // Specialize the case where we're doing (AB) / B --> A
            inline A operator /(B const &rhs) const 
            {
                return A(static_cast<double>(*this) / static_cast<double>(rhs));
            }

            // Same specialization, but for (AB) / A --> B
            // Again, we must be careful for the case where A == B
            inline typename GetSecondType<B>::Type operator /(typename GetFirstType<A>::Type const &rhs) const
            {
                return B(static_cast<double>(*this) / static_cast<double>(rhs));
            }

            // Specialization for (AB) / (N/D) --> ABD / N
            template <class N, class D> inline DividedUnit<MultipliedUnit<MultipliedUnit<A, B>, D>, N> operator /(DividedUnit<N, D> const &rhs) const
            {
                return DividedUnit<MultipliedUnit<MultipliedUnit<A, B>, D>, N>(static_cast<double>(*this) / static_cast<double>(rhs));
            }

            #pragma endregion // Arithmetic operators
        };

        /**
         * Generic class for when two units are divided, resulting in a new unit type.
         * It is generally bad for there to be a DividedUnit<N, D> where N == D, and so a specialization
         * will handle that case, providing a natural conversion to N. It is expected, however, that such a 
         * construct would not be created because the conditions to form such a unit (dividing A by A) would yield
         * something other than DividedUnit<A, A>. For these reasons, we do not specially handle the case where N == D.
         *
         * Typename N represents the numerator. Typename D represents the denominator.
         */
        template <class N, class D>
        class DividedUnit 
        {
            template <typename, typename> friend class DividedUnit; // Friendship provided so that all types can get to the numerator/denominator
        private:
            N m_numerator;
            D m_denominator;
        public:
            // Explicit constructor from a scalar value
            // Again, this is required to be explicit because we implicitly cast down to double, so
            // we need to be absolutely sure we're not unintentionally typecasting through two different types (grams to meters, etc.,)
            // The easiest way to do this is to have a numerator of value and denominator of 1 with appropriate units
            explicit inline DividedUnit(double value):m_numerator(value),m_denominator(1.0) {}

            // Basic constructor from matching types (or types implicitly convertible)
            inline DividedUnit(N const &numerator, D const &denominator):
                m_numerator(numerator),m_denominator(denominator) { }

            // Conversion operator from convertible types N2/D2. Also handles the copy constructor.
            template <class N2, class D2>
            inline DividedUnit(DividedUnit<N2, D2> const &copy):m_numerator(0),m_denominator(0) 
            {
                // We need to use operator = instead of constructor-initializer because
                // constructor-initializer can call explicit constructors.
                // If you're getting an error here, check the error for template instantiation; you 
                // are doing an invalid conversion between types.
                m_numerator = copy.m_numerator;
                m_denominator = copy.m_denominator;
            }

            // Assignment operator from same type
            // This prevents compiler from generating one that would trample over the template version.
            inline DividedUnit<N, D> const & operator =(DividedUnit<N, D> const &rhs)
            {
                return operator =<N, D>(rhs);
            }

            template <class N2, class D2>
            inline DividedUnit<N, D> const & operator =(DividedUnit<N2, D2> const &rhs)
            {
                m_numerator = rhs.m_numerator;
                m_denominator = rhs.m_denominator;
                return *this;
            }

            // Conversion to a scalar
            inline operator double() const { return static_cast<double>(m_numerator) / static_cast<double>(m_denominator); }
    
            #pragma region Arithmetic operators
            // Adding is only supported on the same type (and convertibles). Nothing else is mathematically legal.
            template <class T>
            inline DividedUnit<N, D> operator +(T const &rhs) const { DividedUnit<N, D> rhsConverted = rhs; return DividedUnit<N, D>(static_cast<double>(*this) + static_cast<double>(rhsConverted)); }

            // Subtracting is also only supported on the same type
            template <class T>
            inline DividedUnit<N, D> operator -(T const &rhs) const { DividedUnit<N, D> rhsConverted = rhs; return DividedUnit<N, D>(static_cast<double>(*this) - static_cast<double>(rhsConverted)); }

            // Multiplying by generic type A should result in DividedUnit<MultipliedUnit<N, A>, D>
            // By convention, we keep DividedUnit on the outside to allow for optimizations, etc.
            template <class A>
            inline DividedUnit<MultipliedUnit<N, A>, D> operator *(A const &rhs) const 
            {
                MultipliedUnit<N, A> numerator = MultipliedUnit<N, A>(m_numerator, rhs);
                return DividedUnit<MultipliedUnit<N,A>, D>(numerator, m_denominator);
            }

            // Specialization of the multiplication operator for the case where we multiply by the denominator
            // (N/D) * D --> N
            inline N operator *(D const &rhs) const { return N(static_cast<double>(*this) * static_cast<double>(rhs)); }

            // Specialization of the multiplication operator for the case where we multiply by an element in the denominator
            // (N/(AB)) * A --> N/B
            inline DividedUnit<N, typename GetParameter<D>::For<MultipliedUnit>::Right> operator *(typename GetParameter<D>::For<MultipliedUnit>::Left const & rhs) const {
                return DividedUnit<N, typename GetParameter<D>::For<MultipliedUnit>::Right>(static_cast<double>(*this) * static_cast<double>(rhs));
            }

            // Same specialization for (N/(AB)) * B  --> N/A
            inline DividedUnit<N, typename GetParameter<D>::For<MultipliedUnit>::LeftUnique> operator *(typename GetParameter<D>::For<MultipliedUnit>::RightUnique const & rhs) const {
                return DividedUnit<N, typename GetParameter<D>::For<MultipliedUnit>::LeftUnique>(static_cast<double>(*this) * static_cast<double>(rhs));
            }

            // Multiplying by generic type A should result in DividedUnit<N, MultipliedUnit<D, A>>
            // Again, by convention, we keep DividedUnit on the outside
            template <class A>
            inline DividedUnit<N, MultipliedUnit<D, A> > operator /(A const &rhs) const
            {
                MultipliedUnit<D, A> denominator = MultipliedUnit<D, A>(m_denominator, rhs);
                return DividedUnit<N, MultipliedUnit<D, A> >(m_numerator, denominator);
            }

            // Specialization of dividing out from a multiplied unit (AB/C) / B --> A/C
            // This is ONLY valid on a DividedUnit with a MultipliedUnit in its numerator;
            // GetParameter<N>::Left returns an unusable type if N is not a MultipliedUnit
            inline DividedUnit<typename GetParameter<N>::Left, D> operator /(typename GetParameter<N>::Right const &rhs) const 
            {
                return DividedUnit<typename GetParameter<N>::Left, D>(static_cast<double>(*this), static_cast<double>(rhs)); 
            }

            inline DividedUnit<typename GetParameter<N>::RightUnique, D> operator /(typename GetParameter<N>::LeftUnique const &rhs) const
            {
                return DividedUnit<typename GetParameter<N>::RightUnique, D>(static_cast<double>(*this), static_cast<double>(rhs));
            }

            #pragma endregion // Arithmetic operators


        };

        // General comparison operators for both DividedUnit and MultipliedUnit
        // We have to use two template-template parameters to cover all of our bases. Otherwise if we did
        // meter-seconds vs meters per second, the compiler would not match this template and use its built-in
        // instead. This is bad. So instead we catch it at compile-time ourselves by taking a template parameter 
        // X but forcing it to T.
        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator< (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            T<A, B> rhsConverted = rhs; // Need to convert this way because if we used static_cast the compiler might go through double
                                        // which we do not want to do.
            return static_cast<double>(lhs) < static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator <= (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            T<A, B> rhsConverted = rhs;
            return static_cast<double>(lhs) <= static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator == (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            T<A, B> rhsConverted = rhs;
            return static_cast<double>(lhs) == static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator != (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            return !(lhs == rhs);
        }

        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator >= (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            return !(lhs < rhs);
        }

        template <template <typename, typename> class T, template <typename, typename> class X, class A, class B, class C, class D>
        inline bool operator > (T<A, B> const &lhs, X<C, D> const &rhs)
        {
            return !(lhs <= rhs);
        }

        // The following templates exist solely to ensure that the compiler does not try to come up with its own solution
        // for these comparisons (by casting to double). Such comparisons are pretty much always illegal, but we'll try to
        // do the conversion anyway.
        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator <(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            T<A, B> rhsConverted = rhs; // Need to convert this way because if we used static_cast the compiler might go through double
                                        // which we do not want to do.
            return static_cast<double>(lhs) < static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator <=(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            T<A, B> rhsConverted = rhs; 
            return static_cast<double>(lhs) <= static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator ==(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            T<A, B> rhsConverted = rhs; 
            return static_cast<double>(lhs) == static_cast<double>(rhsConverted);
        }

        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator !=(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            return !(lhs == rhs);
        }

        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator >=(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            return !(lhs < rhs);
        }

        template <template <typename, typename> class T, class A, class B, class C>
        inline bool operator >(T<A,B> const &lhs, UnitBase<C> const &rhs)
        {
            return !(lhs <= rhs);
        }
    }
}

// This macro assists with creating conversion operators from DDS types to UnitBase types, until we find a better
// way to do this. We retain type safety: all this does is rely on the existing constructor in the base types.
#define UNITLIB_CREATE_CONVERSION_OPERATOR(DDSType, BaseType) \
    template <> struct GetDDSType<BaseType> {                 \
        typedef DDSType Type;                                 \
    };                                                         

template <class T> struct GetDDSType
{
    // Empty default implementation. Specializations required.
    // Specializations should provide a GetDDSType<T>::Type.
};