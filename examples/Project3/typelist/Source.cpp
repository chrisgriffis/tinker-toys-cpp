//simple tuple example (variadic templates)
#include <iostream>

namespace clg
{
	template<class... T> struct tuple {};

	template<class H, class... T>
	struct tuple<H,T...>
	{
		using This = tuple<H, T...>;
		tuple() = default;
		tuple(H h, T... t) :_h(h), _t(t...) {};
		H _h;
		tuple<T...> _t;

		friend std::ostream& operator<<(std::ostream& strm, This& tt)
		{
			strm << tt._h << " | " << tt._t;
			return strm;
		}
	};

	template<class H>
	struct tuple<H>
	{
		using This = tuple<H>;
		tuple() = default;
		tuple(H h) :_h(h) {};
		H _h;

		friend std::ostream& operator<<(std::ostream& strm, This& tt)
		{
			strm << tt._h << std::endl;
			return strm;
		}
	};

	template<>
	struct tuple<void>
	{
	};

	template<class T>
	struct Leaf
	{
		Leaf() = default;
		Leaf(T t) :_t(t) {}
		T _t;
	};

	template<class... Ts>
	struct Inheriter : public Leaf<Ts>...
	{

	};
}

#include <tuple>
#include <vector>
#include <algorithm>
int main()
{
	using namespace clg;
	tuple<void> n;
	tuple<int, int, bool, bool, char> t(1, 1, 0, 0, 'f');
	Inheriter<int, float, char, double> i; //classes must be different unless adding static indexing
	std::cout << t;

	using foo = std::tuple<int, int, int, int>;
	foo a(7, 2, 9, 8), b(7, 3, 9, 8), c(7, 2, 8, 9), d(6, 2, 9, 8);
	std::vector<foo> v{a,b,c,d};
	std::sort(v.begin(),v.end());
}

//typelist tuple

// #include <iostream>
// 
// namespace clg
// {
// 	struct InvalidType;
// 	struct Nulltype {};
// 
// 	template<class H, class T>
// 	struct tlisttuple
// 	{
// 		tlisttuple() = default;
// 		tlisttuple(H& h, T& t) :_h(h), _t(t) {};
// 		tlisttuple(H h, T t) :_h(h), _t(t) {};
// 		H _h;
// 		T _t;
// 
// 		friend std::ostream& operator<<(std::ostream& str, tlisttuple& me)
// 		{
// 			str << me._h << " | " << me._t;
// 			return str;
// 		}
// 	};
// }
// 
// #define tlisttuple3(x1,x2,x3) tlisttuple<x1,tlisttuple<x2,x3>>
// #define tlisttuple4(x1,x2,x3,x4) tlisttuple<x1,tlisttuple3(x2,x3,x4)>
// #define tlisttuple5(x1,x2,x3,x4,x5) tlisttuple<x1,tlisttuple4(x2,x3,x4,x5)>
// 
// using namespace clg;
// 
// int main()
// {
// 
// 	tlisttuple5(int, int, int, bool, char) t/*(1, 2, 3, 0, 'x')*/;
// 	t._h = 1;
// 	t._t._h = 2;
// 	t._t._t._h = 3;
// 	t._t._t._t._h = false;
// 	t._t._t._t._t = 'x';
// 	std::cout << t;
// }
