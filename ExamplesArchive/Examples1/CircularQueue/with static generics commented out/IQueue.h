#pragma 

namespace Griffis
{
    template<class EltType = void*>
    class IQueue
    {
    public:
        static const int SUCCESS = 0;
        static const int FAIL = 1;
        static const int NOTINITIALIZED = 2;

        typedef const int ErrorCodeType;
        typedef EltType EltType;

        virtual int Initialize(unsigned queueSize) = 0;
        virtual int Enqueue(EltType) = 0;
        virtual int Dequeue(EltType&) = 0;
        virtual ~IQueue() = 0;
    };
    template<class EltType> inline IQueue<EltType>::~IQueue(){}	
}