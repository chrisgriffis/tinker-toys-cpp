//Sort Algorithms: merge, heap, quick

#define _CRT_SECURE_NO_WARNINGS

//#define CPP11

#ifdef CPP11
#include <future>
#define MOVE(a) std::move(a)
#else
#define MOVE(a) a
#endif

#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;

template<class T>
void myswap(T& a,T& b)
{
    T temp = MOVE(a);
    a = MOVE(b);
    b = MOVE(temp);
}

template<class T>
void shuffle (T* input,int LeftIdx,int RightIdx)
{
    srand(rand());
    int k  = 0;
    while ((k  + LeftIdx)< RightIdx)
    {
        size_t r = rand() % (RightIdx-LeftIdx);
        myswap(*(input + LeftIdx + k),*(input + LeftIdx + r));
        k++; 
    }
}
template<class T>
void mymerge(T* start1, T* stop1, T* start2, T* stop2, T* output )
{
    int i = 0;
    int j = 0;
    while ((i<(stop1-start1)) && (j<(stop2-start2)))
    {
        if (*(start1+i) < *(start2+j))
        {
            *(output+i+j) = *(start1+i); i++;
        } 
        else
        {
            *(output+i+j) = *(start2+j); j++;
        }
    }
    // one of the arrays is exhausted
    // so copy the rest of the other one directly
    if (i == (stop1 - start1))
    {
        // then copy the rest of J
        while ((j<(stop2-start2)))
        {
            *(output+i+j) = *(start2+j); j++;
        }
    } 
    else
    {
        while ((i<(stop1-start1)))
        {
            *(output+i+j) = *(start1+i); i++;
        }
    }
}

template<class T>
void quicksort(T* inputArr,int LeftIdx,int RightIdx)
{
    if(LeftIdx<=RightIdx)
    {
        int pivot = LeftIdx;
        int i = LeftIdx+1;
        int j = RightIdx;
        while (i<j)
        {
            while(inputArr[i]<=inputArr[pivot] && i<RightIdx)
            {
                i++;
            }
            while(inputArr[j]>inputArr[pivot] && j>LeftIdx)
            {
                j--;
            }
            myswap(inputArr[i],inputArr[j]);
        }
        myswap(inputArr[i],inputArr[j]);
        myswap(inputArr[j],inputArr[pivot]);
#ifdef CPP11
        std::future<void> X, Y;
        X = async([](T* inputArr,int LeftIdx,int RightIdx){
            quicksort(inputArr,LeftIdx,RightIdx);
        },inputArr,LeftIdx,j-1);
        Y = async([](T* inputArr,int LeftIdx,int RightIdx){
            quicksort(inputArr,LeftIdx,RightIdx);
        },inputArr,j+1,RightIdx);
#else
        quicksort(inputArr,LeftIdx,j-1);
        quicksort(inputArr,j+1,RightIdx);
#endif
    }
}

template<class T>
void heapify(T* input,int size)
{
    for (int i = (size-1)/2+1; i >0 ; i--)
    {
        bool heap = false;
        int k =i-1;
        while (!heap && 2*k < size )
        {
            int biggestidx=0;
            if (2*k+1 < size)
            {
                biggestidx = input[2*k] > input[2*k+1] ? 2*k : 2*k+1;
            }
            else
            {
                biggestidx = 2*k;
            }

            if (input[k] >= input[biggestidx])
            {
                heap = true;
            }
            else
            {
                myswap(input[k], input[biggestidx]);
                k = biggestidx;
            }
        }
    }
}

template<class T>
void heapsort(T* inputArr,int size)
{
    if (size > 1)
    {
        heapify(inputArr, size);
        myswap(inputArr[0],inputArr[size -1]);
        heapsort(inputArr, size -1);
    }
}

template<class T>
void mergeSort(T* inputArr,int size)
{
    if(size>1)
    {
        T* B = new T[(size/2)];
        T* C = new T[size-(size/2)];
        memcpy(B,inputArr,((size/2))*sizeof(T));
        memcpy(C,inputArr+(size/2),(size-(size/2))*sizeof(T));
        mergeSort(B,(size/2));
        mergeSort(C,size-(size/2));
        mymerge(B,B+(size/2),C,C+(size-(size/2)),inputArr);
        delete[] B;
        delete[] C;
    }
}


int main()
{
    char arr[] = "asdfahsdgfiquwrqwertyuiopasqwertyuiopasdfghjklzxcvbnmdfghjkllzxcvbnmbivwbrpoqiwueroidnsfbkjnxnvgoerihnvwoiuenvsfvbnskjdnfvoiwrngvjdhfkjgbsjfvtpigbqueprginkljdfsgnbkj";

    stringstream s;
    for (int i = 0; i < 10 ; i++)
    {
        s << arr;
    }
    char* arr1 = new char[s.str().size()+1];
    strcpy(arr1,s.str().c_str());

    cout <<  "\n\nmerge\n";
    shuffle(arr1, 0,strlen(arr1));
    cout << arr1 << "\n\nmergesorted\n";
    mergeSort(arr1,strlen(arr1));

    cout << arr1 << "\n\nquick\n";
    shuffle(arr1, 0,strlen(arr1));
    cout << arr1 << "\n\nquicksorted\n";
    quicksort(arr1,0,strlen(arr1)-1);

    cout << arr1 << "\n\nheap\n";
    shuffle(arr1, 0,strlen(arr1));
    cout << arr1 << "\n\nheapsorted\n";
    heapsort(arr1,strlen(arr1));
    cout << arr1 << "\n";

    //system("pause");
    return 0;
}