#include <cstdlib>
#include <iostream>
#include <windows.h>

using namespace std;


typedef int (*Fn)(int,int);
typedef Fn (*DllFn)(int);

int main(int argc, char *argv[])
{
    cin.get();
    Fn _fn;
    DllFn _DllFn;
    HMODULE hWinst = LoadLibrary(argv[1]);
    
    if(hWinst)
    {
        _DllFn = (DllFn)GetProcAddress(hWinst, argv[2]);
        
        if(_DllFn){_fn = (Fn)_DllFn(argc - 4);
                    std::cout << _fn(5,5) << std::endl;}
            else {std::cout << "no method" << std::endl;}

    }
    else {std::cout << "no load" << std::endl;}
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
