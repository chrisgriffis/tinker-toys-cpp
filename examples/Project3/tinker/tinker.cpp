
#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <vector>
#include <deque>
#include <type_traits>
using namespace std;

namespace fsys = std::experimental::filesystem;

/// Convenient factory for binary ofstreams.
class OutputStream : public std::ofstream {
public: OutputStream(std::string const& f) :
	std::ofstream(f.c_str(), std::ios::binary) { if (!good()) throw ; }
};

/// Convenient factory for binary ifstreams.
class InputStream : public std::ifstream {
public: InputStream(std::string const& f) :
	std::ifstream(f.c_str(), std::ios::binary) { if (!good()) throw ; }
};


template<class T>
void Write(ostream& ofs, const T& val)
{
	ofs.write(reinterpret_cast<const char*>(&val), sizeof(T));
}

template<template <class, class> class C, class T, class A>
void Write(ostream& ofs, const C<T,A>& val)
{
	Write(ofs, val.size());
	ofs.write(reinterpret_cast<const char*>(val.data()), val.size()*sizeof(T));
}

template<template <class, class> class C, class T, class A>
decltype(
	declval<C<T,A>>().resize(size_t()),
	declval<C<T,A>>().data(),
	void())
Read(istream& ifs, C<T,A>& val)
{
	const size_t N = [&]
	{
		size_t val;
		ifs.read(reinterpret_cast<char*>(&val), sizeof(size_t));
		return val;
	}();
	val.resize(N);
	ifs.read(reinterpret_cast<char*>(val.data()), N * sizeof(T));
}




struct A
{
	char a;
	int x;
	float q;
};

struct B : public A
{
	B() = default;
	B(A a, double x,double y) :A(a),_x(x),_y(y) {}
	double _x;
	double _y;

	friend ostream& operator<<(ostream& s, B& b)
	{
		s <<
			"A::a (" << b.A::a << "), " <<
			"A::x (" << b.A::x << "), " <<
			"A::q (" << b.A::q << "), " <<
			"B::_x (" << b.B::_x << "), " <<
			"B::_y (" << b.B::_y << "), " << endl;
		return s;
	}
};

template<template <class,class> class C, class T, class A>
ostream& operator<<(ostream& s, C<T,A>& v)
{
	s << "\t{\n";
	for (T& e : v) s << e;
	s << "\t}\n";
	return s;
}


int main()
{
	B b{ A{'j',5,5.6f},345.345,-12.12 };
// 	using B = int;

	vector<B> foo{ b,b,b };
	cout << foo << endl;

	fsys::path p = fsys::current_path();
	fsys::create_directory( p / "io");
	OutputStream o(p.string() + "\\io\\pants.txt");
	Write(o, foo);
	o.close();

	vector<B> vbar;
	InputStream i(p.string() + "\\io\\pants.txt");
	Read(i, vbar);

	//compile fail, does not support data()
	//deque<B> dbar;
	//Read(i, dbar);

	cout << vbar;

	system("pause");
	return 0;
}
