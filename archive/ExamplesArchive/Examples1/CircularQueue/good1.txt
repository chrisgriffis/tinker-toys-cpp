#include "CircularQueue.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <map>


using namespace Griffis;
using namespace std;

namespace Griffis
{
    struct MyConvenientStruct
    {
        IQueue<int>* iq;
        int threadNum;
        unsigned queueSize;
    };
}
void ThreadMain(MyConvenientStruct* s);
HANDLE CreateChildTh(MyConvenientStruct* s);


int main()
{ 
    HANDLE hThreadHandle[3];
    DWORD dwEvent, i;

    int myCount = 1;
    cout << "Enter queue size: ";
    cin >> myCount;
    IQueue<int>* iq = new CircularQueueOfInts();

    MyConvenientStruct s1 = {iq,1,myCount};
    MyConvenientStruct s2 = {iq,2,myCount};
    MyConvenientStruct s3 = {iq,3,myCount};

    hThreadHandle[0] = CreateChildTh(&s1);
    hThreadHandle[1] = CreateChildTh(&s2);
    hThreadHandle[2] = CreateChildTh(&s3);

    dwEvent = WaitForMultipleObjects(3,hThreadHandle,TRUE,INFINITE);

    switch (dwEvent) 
    { 
        // hThreadHandle[0] was signaled
    case WAIT_OBJECT_0 + 0: 
        // TODO: Perform tasks required by this event
        wprintf(L"First event was signaled.\n");
        break; 

        // hThreadHandle[1] was signaled
    case WAIT_OBJECT_0 + 1: 
        // TODO: Perform tasks required by this event
        wprintf(L"Second event was signaled.\n");
        break;

        // hThreadHandle[2] was signaled
    case WAIT_OBJECT_0 + 2: 
        // TODO: Perform tasks required by this event
        wprintf(L"Third event was signaled.\n");
        break; 

    case WAIT_TIMEOUT:
        wprintf(L"Wait timed out...\n");
        break;

        // Return value is invalid.
    default: 
        wprintf(L"Wait error %d\n", GetLastError()); 
        ExitProcess(0); 
    }

    for(i = 0;i<3;i++)
    {
        if(CloseHandle(hThreadHandle[i]) != 0)
            wprintf(L"Closing the hThreadHandle[%d] handle is OK...\n", i);
        else
            wprintf(L"Failed to close the hThreadHandle[%d] handle, error %d...\n", GetLastError());
    }

    delete iq;
    system("pause");
    return 0;
}

void ThreadMain(MyConvenientStruct* s)
{
    map<typename IQueue<int>::ErrorCodeType,string> m;
    m[IQueue<int>::SUCCESS] = "success";
    m[IQueue<int>::FAIL] = "fail";
    m[IQueue<int>::NOTINITIALIZED] = "not initialized";
    m[IQueue<int>::ALREADYINITIALIZED] = "already initialized";
    m[IQueue<int>::EMPTY] = "empty";
    m[IQueue<int>::FULL] = "full";

    {
        string str = m[s->iq->Initialize(s->queueSize)];
        printf("thread %d init queue: %s \n",s->threadNum,&str[0]);
    }

    for(unsigned x = 0; x < 2*s->queueSize; x++)
    {
        string str = m[s->iq->Enqueue(x)];
        printf("thread %d enqueue<- %d: %s \n",s->threadNum,x, &str[0]);
    }
    for(unsigned x = 0; x < 2*s->queueSize; x++)
    {
        int temp = -1;
        string str = m[s->iq->Dequeue(temp)];
        printf("thread %d dequeue-> %d: %s \n",s->threadNum,temp, &str[0]);
    }
    printf("\n\n XXXXXXXXXXXXXXXX thread %d self race XXXXXXXXXXXXXXXXXXXXX\n\n",s->threadNum);
    for(unsigned x = 0; x < 2*s->queueSize; x++)
    {
        int temp = -1;
        string str = m[s->iq->Enqueue(x)];
        printf("thread %d enqueue<- %d: %s \n",s->threadNum,x, &str[0]);
        str = m[s->iq->Dequeue(temp)];
        printf("thread %d dequeue-> %d: %s \n",s->threadNum,temp, &str[0]);
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





