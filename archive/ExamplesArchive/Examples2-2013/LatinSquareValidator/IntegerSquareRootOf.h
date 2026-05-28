 
template <unsigned> struct IntegerSquareRootOf;
template <unsigned,unsigned> struct IntegerSquareRootOfImpl;
 
template <unsigned V, unsigned G> struct NextResult
{
       enum Get { value = IntegerSquareRootOfImpl<V,G>::result };
};
 
template <unsigned V> struct NextResult<V,0>
{
       // Should never be used
       enum Get { value = 0 };
};
template <unsigned G> struct NextResult<0,G>
{
    // Should never be used
    enum Get { value = 0 };
};
 
template <unsigned V, unsigned G> struct IntegerSquareRootOfImpl
{
       enum Increment { value = ((G*G < V) ? (G + 1) : 0) };
       enum Compute { result = ((G*G < V) ? NextResult<V, value>::value : G) };
};
 
template <unsigned V> struct IntegerSquareRootOf
{
public:
       static const unsigned result = IntegerSquareRootOfImpl<V, 1>::result;
       static const unsigned isPerfect = (result*result == V);
};
 
 
// int main()
// {
//        static_assert(2 ==  IntegerSquareRootOf<4>::result, "IntegerSquareRootOf(4) != 2");
//        static_assert(32==  IntegerSquareRootOf<1024>::result, "IntegerSquareRootOf(1024) != 32");
//        static_assert(64==  IntegerSquareRootOf<4096>::result, "IntegerSquareRootOf(4096) != 64");
//        static_assert(128== IntegerSquareRootOf<16384>::result, "IntegerSquareRootOf(16384) != 128");
//        //     static_assert(256== IntegerSquareRootOf<65536>::result, "IntegerSquareRootOf(65536) != 256");
//        //     static_assert(1024==IntegerSquareRootOf<1048756>::result, "IntegerSquareRootOf(1048756) != 1024");
//  
//        return 0;
// }