#pragma once
#include "ICommandable.h"
#include "IRemoteInterface.h"


namespace chrisg
{
    class ProxyClassCommand : public ICommandable
    {
    private:
        static ProxyClassCommand* m_pProxyClassCommand;
        ProxyClassCommand(){}
    public:
        static ProxyClassCommand* Get()
        {
            if(m_pProxyClassCommand) return m_pProxyClassCommand;
            else {m_pProxyClassCommand = new ProxyClassCommand(); return m_pProxyClassCommand;}
        }
        void Invoke()
        {
            CRemoteServer1 server1;
            CRemoteServer2 server2;

            ProxyClass proxy;

            std::cout << "I want to do something remotely" << std::endl;

            proxy.DoSomethingRemotely();

            std::cout << "I want to do something remotely on server1" << std::endl;

            proxy.SetRemoteServer(server1);
            proxy.DoSomethingRemotely();

            std::cout << "I want to do something remotely on server2" << std::endl;

            proxy.SetRemoteServer(server2);
            proxy.DoSomethingRemotely();

            system("Pause");
        }
    };
}

chrisg::ProxyClassCommand* chrisg::ProxyClassCommand::m_pProxyClassCommand = NULL;
