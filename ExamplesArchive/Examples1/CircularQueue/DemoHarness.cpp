//This demo harness demonstrates the utility, operation, and thread safety of the
//CircularQueueOfInts class.

//Because it is a "demo harness," less consideration was put into making it
//"perfect." This means that I'm not worried about tchars and mixing
//wprintf and printf statements, lack of null checking, incomplete commenting, 
//a random mixture of STL objects, etc.
//Also, the facilities used in the demo harness are considered 
//outside the scope of the exercise and therefore are not subject to the
//"do not use existing libraries" clause. 

//////////////////////////////////////////////////////////////////////////
// these classes are used to help set up a demo and exercise the solution
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <map>

//////////////////////////////////////////////////////////////////////////
//This is the solution to the exercise and the subject of the demo
#include "CircularQueue.h"
//////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL 0
#endif


using namespace Griffis;
using namespace std;

namespace Griffis
{
    struct MyConvenientStruct
    {
        IQueue<int>* iq;
        int threadNum;
        unsigned queueSize;
        bool polarity;
    };
}
void ThreadMain(MyConvenientStruct* s);
HANDLE CreateChildTh(MyConvenientStruct* s);


int main()
{ 
    unsigned myCount = 1;
    const int numThr = 4;
    cout << "Enter queue size: ";
    cin >> myCount;
    //CircularQueueOfInts will theoretically still work for large values,
    //so adding a size limit within the class definition would be an arbitrary constraint.
    //That being said, if the user inputs a negative value in the console
    //it will be coerced into a very huge positive number.
    //Here I massage myCount to meet the usage agreement specified by the
    //CircularQueueOfInts class description
    //still, the user might also pick a really big value have to wait a long time,
    //so for demo purposes I will help the user and constrain the value here 
    if (myCount > 31)
    {
        myCount = 32;
    }
    wprintf(L"Using %d as queue size...\n", myCount);
    IQueue<int>* iq = new CircularQueueOfInts();

    //Four are threads given access to the same uninitialized pointer-to-IQueue
    //the desire is to create a race on initializing the object as well as (ab)using it.
    const bool DEQUEUE = false;
    const bool ENQUEUE = true;
    MyConvenientStruct s1 = {iq,1,myCount,DEQUEUE};
    MyConvenientStruct s2 = {iq,2,myCount,ENQUEUE};
    MyConvenientStruct s3 = {iq,3,myCount,DEQUEUE};
    MyConvenientStruct s4 = {iq,4,myCount,ENQUEUE};

    HANDLE hThreadHandle[numThr];
    hThreadHandle[0] = CreateChildTh(&s1);
    hThreadHandle[1] = CreateChildTh(&s2);
    hThreadHandle[2] = CreateChildTh(&s3);
    hThreadHandle[3] = CreateChildTh(&s4);

    WaitForMultipleObjects(numThr,hThreadHandle,TRUE,INFINITE);

    for(int i = 0;i<numThr;i++)
    {
        if(CloseHandle(hThreadHandle[i]) != 0)
            wprintf(L"Closing the hThreadHandle[%d] handle is OK...\n", i);
        else
            wprintf(L"Failed to close the hThreadHandle[%d] handle, error %d...\n", GetLastError());
    }

    //delete via the interface, exercise virtual destructor.
    delete iq;
    system("pause");
    return 0;
}

void ThreadMain(MyConvenientStruct* s)
{
    //set up a map for convenient error code reporting
    map<typename IQueue<int>::ErrorCodeType,string> m;
    m[IQueue<int>::SUCCESS] = "success";
    m[IQueue<int>::FAIL] = "fail";
    m[IQueue<int>::NOTINITIALIZED] = "not initialized";
    m[IQueue<int>::ALREADYINITIALIZED] = "already initialized";
    m[IQueue<int>::EMPTY] = "empty";
    m[IQueue<int>::FULL] = "full";

    //create a race on initializing the queue
    {
        string str = m[s->iq->Initialize(s->queueSize)];
        printf("thread %d init queue: %s \n", s->threadNum, &str[0]);
    }

    //create a race on the queue with equal numbers of competing threads
    for(unsigned x = 0; x < 2*s->queueSize; x++)
    {
        if (s->polarity)
        {
            string str = m[s->iq->Enqueue(x)];
            printf("thread %d enqueue<- %d: %s \n", s->threadNum, x, &str[0]);
        }
        else
        {
            int temp = -1;
            string str = m[s->iq->Dequeue(temp)];
            printf("thread %d   dequeue-> %d: %s \n", s->threadNum, temp, &str[0]);
        }
    }
}

HANDLE CreateChildTh(MyConvenientStruct* s)
{
    HANDLE hThread;
    DWORD dwId;

    hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMain,(LPVOID)s,0,&dwId);

    if(hThread)
    {
        wprintf(L"CreateThread() is OK, ID num %d...\n", dwId);
        return hThread;
    }
    else
    {
        wprintf(L"CreateThread() failed, error %d\n", GetLastError());
        return NULL;
    }
}

#undef NULL