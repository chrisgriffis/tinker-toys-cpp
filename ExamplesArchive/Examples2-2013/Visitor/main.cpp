#include "stdio.h" // for printf

class IVisitor;
class House;
class Boat;



class IVisitable
{
public:
    virtual void Accept(IVisitor& visitor) = 0;
};

class IVisitor
{
public:
    virtual void Visit(House&) = 0;
    virtual void Visit(Boat&) = 0;
};

class House : public IVisitable
{
public:
    void Accept(IVisitor& visitor)
    {
        visitor.Visit(*this);
    }
};

class Boat : public IVisitable
{
public:
    void Accept(IVisitor& visitor)
    {
        visitor.Visit(*this);
    }
};

class FriendlyVisitor : public IVisitor
{
public:
    void Visit(House& h)
    {
        printf("what a nice house.\n");
    }
    void Visit(Boat& b)
    {
        printf("what a nice boat.\n");
    }
};

class GrumpyVisitor : public IVisitor
{
public:
    void Visit(House& h)
    {
        printf("dont like the house.\n");
    }
    void Visit(Boat& b)
    {
        printf("dont like the boat.\n");
    }
};



int main1()
{
    //demonstrate visitor pattern
    House house;
    Boat boat;
    FriendlyVisitor fv;
    GrumpyVisitor gv;

    //2x2 visitations!
    house.Accept(fv);
    house.Accept(gv);

    boat.Accept(fv);
    boat.Accept(gv);

    return 0;
}
