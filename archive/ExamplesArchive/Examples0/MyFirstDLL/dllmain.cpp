
#include <iostream>

using namespace std;

extern "C" 
{
    __declspec (dllexport) void __cdecl FuckAss()
    {
         std::cout << "FuckAss called" << std::endl;
    }
    __declspec (dllexport) void __cdecl Another()
    {
         std::cout << "Another called" << std::endl;
    }
    __declspec (dllexport) void __cdecl StillWorks()
    {
         std::cout << "StillWorks called" << std::endl;
    }
}
