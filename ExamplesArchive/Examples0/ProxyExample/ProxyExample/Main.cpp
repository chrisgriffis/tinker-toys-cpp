#include <Windows.h>
#include <iostream>
#include <tchar.h>	// Include Unicode support functions
#include <stdlib.h>
#include <string.h>


using namespace std;

namespace chrisg
{
    class ICommandable
    {
    public:
        virtual void Invoke() = 0;
    };
}

typedef void (*GetCommandFn)(chrisg::ICommandable*&);

int main()
{
	typedef unsigned short wchar_t
	wchar_t* dllName;
	cin.get(dllName,15);
    HMODULE hMod = (HMODULE) LoadLibrary(dllName);

    if(hMod)
    {
        GetCommandFn Fn;
        Fn = (GetCommandFn) GetProcAddress(hMod, "GetCommand");

        if (Fn)
        {
            chrisg::ICommandable* icmd =  NULL;
            Fn(icmd);

            if (icmd)
            {
                icmd->Invoke();
                delete icmd;
                icmd = NULL;
            }
            else cout << "no command" << endl;
        }
        else cout << "no function" << endl;
    }
    else cout<< "no load" << endl;
    return 0;
}

