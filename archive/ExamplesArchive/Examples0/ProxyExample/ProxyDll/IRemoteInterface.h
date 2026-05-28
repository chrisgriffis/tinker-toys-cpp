#pragma once
#include <iostream>

namespace chrisg
{
    class IRemoteInterface
    {
    public:
        virtual void DoSomethingRemotely() =0;
    };

    class  CRemoteServer1 : public IRemoteInterface
    {
    public:
        void DoSomethingRemotely()
        {
            std::cout << "Hello from Remote server One!!" << std::endl;
        }
    };

    class  CRemoteServer2 : public IRemoteInterface
    {
    public:
        void DoSomethingRemotely()
        {
            std::cout << "Hello from Remote server Two!!" << std::endl;
        }
    };

    class ProxyClass : public IRemoteInterface
    {
    public:
        ProxyClass() : m_pRemoteInterface(0){}
        void DoSomethingRemotely()
        {
            std::cout << "calling my proxy to do my remote negotiating for me" << std::endl;
            if(m_pRemoteInterface) m_pRemoteInterface->DoSomethingRemotely();
            else std::cout << "No remote server set!" << std::endl;
        }
        void SetRemoteServer(IRemoteInterface& iri)
        {
            m_pRemoteInterface = &iri;
        }
    private:
        IRemoteInterface* m_pRemoteInterface;
    };
}
