#include  <iostream> 

using namespace std;

template<class T>
void myswap(T& a,T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<class T,unsigned N>
void myprint(T (&arr)[N])
{
    for (int i = 0; i < N ; i++)
    {
        cout << arr[i];
    }
    cout << endl;
}

template<class T, unsigned N>
void Permute(T (&arr)[N], int SIZE=N, int startPos = 0)
{
    if (SIZE==1)
    {
        myprint(arr);
    }
    else
    {
        //first process everything in its current order
        Permute(arr,SIZE-1,startPos+1);
        //now swap first position with every other position
        for (int element = startPos+1; element < (SIZE+startPos); element++)
        {
            myswap(arr[startPos],arr[element]);
            Permute(arr,SIZE-1,startPos+1);
            myswap(arr[element],arr[startPos]);
        }
    }
}

int main()
{
    int intArray[] = {1,2,3,4};
    float floatArray[] = {1.1f,2.2f,3.3f,4.4f};
    char charArray[] = "abcd";
    //template function infers type
    Permute(intArray);
    cout << "\n\n";
    Permute(floatArray);
    cout << "\n\n";
    //using actual array length would swap null terminator, so pass strlen
    Permute(charArray,strlen(charArray));
    system("pause");
    return 0;
}