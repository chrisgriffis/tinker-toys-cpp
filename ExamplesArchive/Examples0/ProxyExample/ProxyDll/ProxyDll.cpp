#include "ICommandable.h"
#include "IRemoteInterface.h"
#include "ExampleCommand.h"

extern "C" __declspec(dllexport) void __cdecl GetCommand(chrisg::ICommandable*& pICmd)
{
    pICmd = (chrisg::ICommandable*) chrisg::ProxyClassCommand::Get();
}