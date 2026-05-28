//Sort Algorithms: merge, heap, quick

#define _CRT_SECURE_NO_WARNINGS

#define CPP11

#ifdef CPP11
#include <future>
#define MOVE(a) std::move(a)
#else
#define MOVE(a) a
#endif

#include <iostream>
#include <sstream>

using namespace std;

template<class T>
void myswap(T& a,T& b)
{
    T temp = MOVE(a);
    a = MOVE(b);
    b = MOVE(temp);
}

template<class T>
void mymerge(T* start1, T* stop1, T* start2, T* stop2, T* output )
{
    size_t i = 0;
    size_t j = 0;
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
        auto X = async(std::launch::deferred,quicksort<T>,inputArr,LeftIdx,j-1);
        std::bind([](T* inputArr,int LeftIdx,int RightIdx){
            quicksort(inputArr,LeftIdx,RightIdx);
        },inputArr,j+1,RightIdx)();
        X.get();
#else
        quicksort(inputArr,LeftIdx,j-1);
        quicksort(inputArr,j+1,RightIdx);
#endif
    }
}

template<class T>
void heapify(T* inputArr,int size)
{
    for (int i = (size-1)/2+1; i >0 ; i--)
    {
        bool heap = false;
        int k =i-1;
        T v = inputArr[k];
        while (!heap && 2*k < size )
        {
            int j = 2*k;
            if (j < size-1)
            {
                if (inputArr[j] < inputArr[j+1])
                {
                    ++j;
                }
            }
            if (v >= inputArr[j])
            {
                heap=true;
            }
            else
            {
                inputArr[k] = inputArr[j];
                k = j;
            }
        }
        inputArr[k] = v;
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
    //char arr[] = "asdfahsdgfiquwrqwert";

    stringstream s;
    for (int i = 0; i < 10 ; i++)
    {
        s << arr;
    }
    char* arr1 = new char[s.str().size()+1];
    strcpy(arr1,s.str().c_str());
//     cout << arr1 << "\n\nmerge\n";
//     mergeSort(arr1,strlen(arr1));
    cout << arr1 << "\n\nquick\n";
    quicksort(arr1,0,strlen(arr1)-1);
//     cout << arr1 << "\n\nheap\n";
//     heapsort(arr1,strlen(arr1));
   cout << arr1 << "\n";
    system("pause");
    return 0;
}