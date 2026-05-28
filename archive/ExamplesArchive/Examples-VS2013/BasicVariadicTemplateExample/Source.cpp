#include <iostream>
#include <vector>
using namespace std;

//practice with variadic templates

struct type1{void operator()(){cout <<"type1 ftor called" << endl;}};
struct type2{void operator()(){cout <<"type2 ftor called" << endl;}};
struct type3{void operator()(){cout <<"type3 ftor called" << endl;}};


void process(){} //terminating case

template<typename Arg1, typename... Args>
void process(Arg1 arg1, Args... args)
{
	arg1();
	process(args...);
}

template<typename... T> struct MyTuple {};





int main() {
	// your code goes here
	process(type1(),type2(),type3());
	process(type2(),type3(),type1());

	vector<type1> t{ type1(), type1(), type1(), type1(), type1() };

	using Tuple1 = MyTuple<int,int>;
	using Tuple2 = MyTuple<int,int>;


	return 0;
}