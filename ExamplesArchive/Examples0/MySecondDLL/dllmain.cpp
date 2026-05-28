
#include <iostream>

typedef int (*Fn)(int,int);


int Add(int a, int b) {std::cout << "add called" << std::endl; return a+b;}
int Sub(int a, int b) {std::cout << "sub called" << std::endl; return a-b;}


extern "C" 
{
 __declspec(dllexport) Fn __cdecl GetFn(int x){if(x==-1)return (Fn)&Sub; else return (Fn)&Add; }
}
