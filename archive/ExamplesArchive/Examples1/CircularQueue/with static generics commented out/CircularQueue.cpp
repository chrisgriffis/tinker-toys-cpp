// CircularQueue.cpp : Defines the entry point for the console application.
//

#include "CircularQueue.h"
#include <stdio.h>
#include <iostream>
#include <map>


using namespace Griffis;
using namespace std;


int main()
{ 
    int myCount = 10;
    cin >> myCount;

    map<typename IQueue<int>::ErrorCodeType,string> m;
    m[IQueue<int>::SUCCESS] = "success";
    m[IQueue<int>::FAIL] = "fail";
    m[IQueue<int>::NOTINITIALIZED] = "not initialized";
    IQueue<int>* iq = new CircularQueueOfInts();
    iq->Initialize(myCount);

    for(int i = myCount; i > 0; i--)
    {
        iq->Enqueue(i);
    }
    for(int i = 0; i < myCount+10; i++)
    {
        int temp = 0;
        string str = m[iq->Dequeue(temp)];
        printf("%s %d\n",&str[0],temp);
    }
    delete iq;
    system("pause");
    return 0;
}


// int main(int a)
// { 
// 	map<RESULT,string> m;
// 	m[SUCCESS] = "success";
// 	m[FAIL] = "fail";
// 	IQueue<>* iq = new CircularQueue<10, typename IQueue<>::EltType>();
// 	iq->Initialize();
// 
// 	int dude = 10;
// 	for(int i = dude; i > 0; i--)
// 	{
// 		iq->Enqueue(i);
// 	}
// 	for(int i = 0; i < dude; i++)
// 	{
// 		int temp = 0;
// 		const char * str(&m[iq->Dequeue(temp)][0]);
// 
//         printf("%s %d\n",str,temp);
// 	}
// 	for(int i = dude; i > 0; i--)
// 	{
// 		iq->Enqueue(i);
// 	}
// 	for(int i = 0; i <= dude; i++)
// 	{
// 		int temp = 0;
// 		const char * str(&m[iq->Dequeue(temp)][0]);
//         printf("%s %d\n",str,temp);
// 	}
// 	system("pause");
// 	return 0;
// }
// 
