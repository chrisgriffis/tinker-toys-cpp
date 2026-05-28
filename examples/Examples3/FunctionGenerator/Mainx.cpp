#include <iostream>
#include <cstdlib> //rand
#include <stack>
#include <map>
#include <sstream>
#include <vector>
#include "time.h"

struct BinaryOperation 
{
    virtual double operator()(double a,double b) = 0; 
    virtual const char* getSymbolString() const = 0;
};
struct BinaryOperationFactory {virtual BinaryOperation* get() = 0;};


struct NOOPOperationFactory : public BinaryOperationFactory
{
    struct NOOP : public BinaryOperation 
    { 
        double operator()(double a,double b){return a;}
        const char* getSymbolString() const {return "<>";}
    };

    BinaryOperation* get()
    {
        return new NOOP;
    }
};


#define MakeBinaryOpFactory(opname,opsymbol) \
struct opname##OperationFactory : public BinaryOperationFactory\
{\
    struct opname : public BinaryOperation \
    { \
        double operator()(double a,double b){return a opsymbol b;} \
        const char* getSymbolString() const {return #opsymbol;}\
    };\
\
    BinaryOperation* get()\
    {\
        return new opname;\
    }\
}

class BinaryOpGenerator 
{
public:
    BinaryOpGenerator(): _registry() 
    {
        _registry.push_back(new AddOperationFactory);
        _registry.push_back(new SubOperationFactory);
        _registry.push_back(new MultOperationFactory);
        _registry.push_back(new DivOperationFactory);
    }
    BinaryOpGenerator(const std::vector<BinaryOperationFactory*>& registry):_registry(registry)
    {
        if (_registry.size() < 1)
        {
            _registry.push_back(new NOOPOperationFactory);
        }
    }
    BinaryOperation* generateOp(){ return _registry[rand()%_registry.size()]->get();}
private:
    std::vector<BinaryOperationFactory*> _registry;
    MakeBinaryOpFactory(Add,+);
    MakeBinaryOpFactory(Sub,-);
    MakeBinaryOpFactory(Mult,*);
    MakeBinaryOpFactory(Div,/);
};

class Function
{
public:
    Function(unsigned char maxops = 20, float inclusionprob = 0.8f)
    {
        generateOps(_opstk,maxops,inclusionprob);
    }
    double operator()(std::vector<double>& inputvec)
    {
        std::stack<BinaryOperation*> opstk(_opstk);
        std::vector<double>::iterator arg = inputvec.begin();
        double leftside = 0;
        if (inputvec.size() > 0)
        {
            leftside = *arg++;
            while (arg!=inputvec.end())
            {
                double rightside = *arg++;
                if(!opstk.empty())
                {
                    BinaryOperation* currentop = opstk.top(); opstk.pop();
                    leftside = (*currentop)(leftside,rightside);
                }
            }
        } 
        return leftside;
    }
    std::string getStringInfix(std::vector<double>& inputvec)
    {
        std::vector<double>::iterator arg = inputvec.begin();
        std::stringstream temp;
        temp << *arg++; //equivalent to boost::lexical_cast<std::string>(*arg++);
        std::string infixString = temp.str();
        std::stack<BinaryOperation*> opstk(_opstk);
        while (!opstk.empty() && arg!=inputvec.end())
        {
            std::stringstream infixStringStream;
            infixStringStream << "(" << infixString << " " << opstk.top()->getSymbolString() << " " << *arg++ << ")"; opstk.pop();
            infixString = infixStringStream.str();
        }
        return infixString;
    }
private:
    void generateOps( std::stack<BinaryOperation*>& opstk, unsigned char maxops = 10, float inclusionprob = 0.5f)
    {
        srand(static_cast<unsigned int>(time(0)));
        for (int attempt = 0; attempt < maxops ; attempt++)
        {
            if (static_cast<float>(rand())/static_cast<float>(RAND_MAX) < inclusionprob)
            {
                opstk.push((new BinaryOpGenerator)->generateOp());
            } 
        }
    }
    std::stack<BinaryOperation*> _opstk;
};

class FunctionGenerator
{
public:
    static Function* generateFunction(){return new Function;}
private:
    FunctionGenerator();
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    std::vector<double> inputs;
    int numinputs(1);
    Function* f(NULL);
    while(numinputs != 0)
    {
        numinputs = rand()%20;
        for (int i=1; i <= numinputs ; i++)
        {
            inputs.push_back(( static_cast<float>(rand())/static_cast<float>(RAND_MAX) ) * i);
        }
        f = FunctionGenerator::generateFunction();
        std::cout << f->getStringInfix(inputs) << " = " << (*f)(inputs) << "\n\n";
        std::cin >> numinputs;
    }
    return 0;
}

