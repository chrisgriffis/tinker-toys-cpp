
//Classic Command pattern example

#include "stdio.h"
#include <queue>
#include <future>

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
    void Execute(){if(m_pR)m_pR->C##CNAME();} \
private:     \
    IOutcalls* m_pR; \
}

MakeCCmd(md1);
MakeCCmd(md2);
MakeCCmd(md3);
MakeCCmd(md4);

class IInvoker
{
public:
    virtual void addCommand(ICommand*)=0;
    virtual void run() = 0;
    virtual ~IInvoker(){}
};


class SingletonInvoker : public IInvoker
{
    SingletonInvoker():m_cmdQueue(){}
    static IInvoker* _instance;
    std::queue<ICommand*> m_cmdQueue;
public:
    static IInvoker* Instance()
    {
        if (_instance == NULL)
        {
            _instance = new SingletonInvoker();
        }
        return _instance;
    }
    void addCommand(ICommand* c)
    {
        m_cmdQueue.push(c);
    }
    void run()
    {
        while (true)
        {
            if(!m_cmdQueue.empty())
            {
                ICommand* c=m_cmdQueue.front();
                if(c)
                {
                    c->Execute();
                    m_cmdQueue.pop();
                    delete c;
                }
                else
                {
                    break;
                }
            }
        }
    }
};

IInvoker* SingletonInvoker::_instance = NULL;

class OutcallsProxy : public IOutcalls
{
    IOutcalls* outcalls;
public:
    OutcallsProxy(IOutcalls* target):outcalls(target){}
    virtual void Cmd1() 
    {
        SingletonInvoker::Instance()->addCommand(new md1(outcalls));
    }

    virtual void Cmd2() 
    {
        SingletonInvoker::Instance()->addCommand(new md2(outcalls));
    }

    virtual void Cmd3() 
    {
        SingletonInvoker::Instance()->addCommand(new md3(outcalls));
    }

    virtual void Cmd4() 
    {
        SingletonInvoker::Instance()->addCommand(new md4(outcalls));
    }

};


int main()
{
    IOutcalls* outcalls = new Outcalls();
    IOutcalls* outcallsproxy = new OutcallsProxy(outcalls);
    auto X = async(std::launch::async, []{
        SingletonInvoker::Instance()->run();
    });
    for (int j = 0; j < 3 ; j++)
    {
        outcallsproxy->Cmd1();
        outcallsproxy->Cmd2();
        outcallsproxy->Cmd3();
        outcallsproxy->Cmd4();
    }
    SingletonInvoker::Instance()->addCommand(NULL);
    X.get();
    delete outcallsproxy;
    delete outcalls;
    system("pause");
    return 0;
}