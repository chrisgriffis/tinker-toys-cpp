//Classic Command pattern example

#include "stdio.h"
#include <queue>

using namespace std;

class ICommand 
{
public:
    virtual void Execute() = 0;
    virtual ~ICommand(){}
};


class IOutcalls
{
public:
    virtual void Cmd1() = 0;
    virtual void Cmd2() = 0;
    virtual void Cmd3() = 0;
    virtual void Cmd4() = 0;
    virtual ~IOutcalls(){}
};

class Outcalls : public IOutcalls
{
public:
    Outcalls(){}
    void Cmd1()
    {
        printf("Bark!!\n");
    }
    void Cmd2()
    {
        printf(("Meow!\n"));
    }
    void Cmd3()
    {
        printf("Quack!!\n");
    }
    void Cmd4()
    {
        printf("Moo!!\n");
    }
};


#define MakeCCmd(CNAME) \
class CNAME : public ICommand \
{\
public:     \
    CNAME(IOutcalls* r): m_pR(r){}     \
    void Execute(){if(m_pR)m_pR->CNAME();} \
private:     \
    IOutcalls* m_pR; \
}

MakeCCmd(Cmd1);
MakeCCmd(Cmd2);
MakeCCmd(Cmd3);
MakeCCmd(Cmd4);

class IInvoker
{
public:
    virtual void addCommand(ICommand*)=0;
    virtual void run() = 0;
    virtual ~IInvoker(){}
};

class Invoker : public IInvoker
{
    std::queue<ICommand*> m_cmdQueue;
public:
    Invoker():m_cmdQueue(){}
    void addCommand(ICommand* c)
    {
        m_cmdQueue.push(c);
    }
    void run()
    {
//         while (true)
//         {
            /*if*/while(!m_cmdQueue.empty())
            {
                ICommand* c=m_cmdQueue.front();
                if(c)
                {
                    c->Execute();
                    m_cmdQueue.pop();
                    delete c;
                }
            }
//         }
    }
};

int main()
{
    IOutcalls* outcalls = new Outcalls();
    IInvoker* i = new Invoker();
    i->addCommand(new Cmd1(outcalls));
    i->addCommand(new Cmd2(outcalls));
    i->addCommand(new Cmd3(outcalls));
    i->addCommand(new Cmd4(outcalls));
    i->run();
    delete outcalls;
    delete i;
    //system("pause");
    return 0;
}