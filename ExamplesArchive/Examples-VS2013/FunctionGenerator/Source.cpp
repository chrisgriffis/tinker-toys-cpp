#include <iostream>
#include <cstdlib> //rand
#include <stack>
#include <sstream>
#include <vector>
#include "time.h"
#include <functional>

class BinaryOperation 
{
public:
	BinaryOperation(std::function<double (double,double)> operation,std::string infixRep):
		_operation(operation),
		_infixRep(infixRep){}
	double operator()(double a,double b){return _operation(a,b);}
	const char* getSymbolString() const {return _infixRep.c_str();}
private:
	std::function<double (double,double)> _operation;
	std::string _infixRep;
};

class BinaryOpGenerator 
{
public:
	BinaryOpGenerator(): _registry() 
	{
		_registry.push_back(BinaryOperation([](double a, double b) -> double {return a+b;},std::string("+")));
		_registry.push_back(BinaryOperation([](double a, double b) -> double {return a-b;},std::string("-")));
		_registry.push_back(BinaryOperation([](double a, double b) -> double {return a*b;},std::string("*")));
		_registry.push_back(BinaryOperation([](double a, double b) -> double {return a/b;},std::string("/")));
	}
	BinaryOpGenerator(const std::vector<BinaryOperation>& registry):_registry(registry)
	{
		if (_registry.size() < 1)
		{
			_registry.push_back(BinaryOperation([](double a, double b) -> double {return a;},std::string("noop")));
		}
	}
	BinaryOperation generateOp(){ return _registry[rand()%_registry.size()];}
private:
	std::vector<BinaryOperation> _registry;
};

class Function
{
public:
	Function(unsigned char maxops = 15, float inclusionprob = 0.7f)
	{
		generateOps(_opstk,maxops,inclusionprob);
	}
	double operator()(std::vector<double>& inputvec)
	{
		std::stack<BinaryOperation> opstk(_opstk);
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
					leftside = opstk.top()(leftside,rightside); 
					opstk.pop();
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
		std::stack<BinaryOperation> opstk(_opstk);
		while (!opstk.empty() && arg!=inputvec.end())
		{
			std::stringstream infixStringStream;
			infixStringStream << "(" << infixString << " " << opstk.top().getSymbolString() << " " << *arg++ << ")"; 
			opstk.pop();
			infixString = infixStringStream.str();
		}
		return infixString;
	}
private:
	void generateOps( std::stack<BinaryOperation>& opstk, unsigned char maxops = 10, float inclusionprob = 0.5f)
	{
		for (int attempt = 0; attempt < maxops ; attempt++)
		{
			if (static_cast<float>(rand())/static_cast<float>(RAND_MAX) < inclusionprob)
			{
				opstk.push(BinaryOpGenerator().generateOp());
			} 
		}
	}
	std::stack<BinaryOperation> _opstk;
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

