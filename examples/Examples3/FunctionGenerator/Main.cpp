#include <iostream>
#include <cstdlib> //rand
#include <stack>
#include <map>
#include <sstream>
#include <vector>
#include "time.h"

typedef double (*mathfn)(double,double);

double add(double a,double b){return a+b;}
double mult(double a,double b){return a*b;}
double sub(double a,double b){return a-b;}
double div(double a,double b){return a/b;}

mathfn operations[]={&add,&mult,&div,&sub};


class Function
{
public:
    Function(unsigned char maxops = 20, float inclusionprob = 0.8f)
    {
        generateOps(_opstk,maxops,inclusionprob);
            //the following is terrible. i need to populate a map in a runtime context.
            //im coupling knowledge of operation details into my function class
            //i really should be using function objects that report their own symbol string
            //this is my disclaimer that i know that this is bad code.
        _symbolLookup[&add]="+";
        _symbolLookup[&mult]="*";
        _symbolLookup[&sub]="-";
        _symbolLookup[&div]="/";
    }
    double operator()(std::vector<double>& inputvec)
    {
        std::stack<mathfn> opstk(_opstk);
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
                    mathfn currentop = opstk.top(); opstk.pop();
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
        std::stack<mathfn> opstk(_opstk);
        while (!opstk.empty() && arg!=inputvec.end())
        {
            std::stringstream infixStringStream;
            infixStringStream << "(" << infixString << " " << _symbolLookup[opstk.top()] << " " << *arg++ << ")"; opstk.pop();
            infixString = infixStringStream.str();
        }
        return infixString;
    }
private:
    void generateOps( std::stack<mathfn>& fnstk, unsigned char maxops = 10, float inclusionprob = 0.5f)
    {
        srand(static_cast<unsigned int>(time(0)));
        for (int attempt = 0; attempt < maxops ; attempt++)
        {
            if (static_cast<float>(rand())/static_cast<float>(RAND_MAX) < inclusionprob)
            {
                fnstk.push(operations[rand()%(sizeof(operations)/sizeof(mathfn))]);
            } 
        }
    }
    std::stack<mathfn> _opstk;
    std::map<mathfn,const char*> _symbolLookup;
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
    using namespace std;
    srand(static_cast<unsigned int>(time(0)));
    std::vector<double> inputs;
    int numinputs = rand()%20;
    for (int i=1; i <= numinputs ; i++)
    {
        inputs.push_back(( static_cast<float>(rand())/static_cast<float>(RAND_MAX) ) * i);
    }
    Function* f = FunctionGenerator::generateFunction();
    cout << f->getStringInfix(inputs) << " = " << (*f)(inputs);
    cin >> numinputs;
    return 0;
}
