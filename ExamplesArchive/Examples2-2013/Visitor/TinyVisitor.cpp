#include <cstdio>


template <class T>
class Visitor
{
public:
	virtual void visit(T& v) = 0;
};

template <class T>
class Visitable
{
public:
	void accept(Visitor<T>& v)
	{
		v.visit(dynamic_cast<T&>(*this));
	}
	virtual ~Visitable(){} //to make Visitable<T> polymorphic
};

class number : public Visitable < number > {};
class character : public Visitable < character > {};
class glyph : public Visitable < glyph > {};
class things : public Visitable < things > {};

class glyphVisitor : public virtual Visitor < glyph >
{
public:
	virtual void visit(glyph& v)
	{
		printf("The method or operation is not implemented.");
	}

};

class Add : public Visitor<number>, public Visitor<character>, public glyphVisitor
{
public:
	void visit(number& n)
	{
		printf("Add numbers\n");
	}
	void visit(character& n)
	{
		printf("Add characters\n");
	}
};



int main()
{
	number n;
	Add a;
	glyphVisitor gv;
	n.accept(a);
	glyph g;
	g.accept(a);
	return 0;
}