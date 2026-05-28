#include <list>
#include <algorithm>
#include <iostream>

class Observer
{
public:
    virtual void update() = 0;
    virtual ~Observer(){}
};

class subject
{
    std::list<Observer*> m_observers;
public:
    subject():m_observers(){}
    virtual ~subject(){}
    virtual void attach(Observer* o)
    {
        if(o) m_observers.push_back(o);
    }
    virtual void detach(Observer* o)
    {
        if (o) m_observers.remove(o);
    }
    virtual void notify()
    {
        for (std::list<Observer*>::const_iterator i = m_observers.begin(); i != m_observers.end(); i++)
        {
            (*i)->update();
        }
    }
};

class publisher : public subject
{
public:
    publisher():state(0){}
    void set_state(int X){state = X; notify();}
    int get_state() {return state;}
    ~publisher(){}
protected:

private:
    int state;
};

#define makesubscriber(tag) class subscriber_##tag : public Observer\
{\
public:\
    subscriber_##tag(publisher* S): publisher_state(0),m_S(S){}\
    void update(){if (m_S)publisher_state = m_S->get_state();}\
    ~subscriber_##tag(){}\
    int publisher_state;\
protected:\
    publisher* m_S;\
private:\
}

makesubscriber(one);
makesubscriber(two);
makesubscriber(three);
makesubscriber(four);

int main()
{
    using namespace std;
    publisher* P = new publisher;
    subscriber_one r1(P);
    subscriber_two r2(P);
    subscriber_three r3(P);
    subscriber_four r4(P);
    P->attach(&r1);
    P->attach(&r2);
    P->attach(&r3);
    P->attach(&r4);

    cout << " 1: "    << r1.publisher_state  << " 2: " << r2.publisher_state << " 3: " << r3.publisher_state << " 4: " << r4.publisher_state << endl;
    P->set_state(1234);
    cout << " 1: "    << r1.publisher_state  << " 2: " << r2.publisher_state << " 3: " << r3.publisher_state << " 4: " << r4.publisher_state << endl;
    system("pause");
}