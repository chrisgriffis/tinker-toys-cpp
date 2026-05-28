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
        static const int ALREADYINITIALIZED = 3;
        static const int EMPTY = 4;
        static const int FULL = 5;

        typedef const int ErrorCodeType;
        typedef EltType EltType;

        virtual int Initialize(size_t queueSize) = 0;
        virtual int Enqueue(EltType inVal) = 0;
        virtual int Dequeue(EltType& outVal) = 0;
        virtual ~IQueue() = 0;
    };
    template<class EltType> inline IQueue<EltType>::~IQueue(){}	
}