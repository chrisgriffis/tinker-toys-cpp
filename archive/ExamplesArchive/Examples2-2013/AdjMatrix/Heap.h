#include <iostream>
using namespace std;


const int MAX_HEAP_SIZE = 1000;
template<class T>
void myswap(T& a,T& b)
{
    T temp = (a);
    a = (b);
    b = (temp);
}

template<class T>
class Heap
{
public:
    Heap():m_size(0)
    {
//         for (int i = 0; i < MAX_HEAP_SIZE ; i++)
//         {
//             m_vals[i]=0;
//         }
    }
    int Insert(T t)
    {
        m_vals[m_size++]=t;
        heapify(m_vals,m_size);
        return m_size;
    }
    T Delete()
    {
        T return_value = m_vals[0];
        if(m_size)myswap(m_vals[0],m_vals[m_size-- -1]);                    
        heapify(m_vals,m_size);
        return return_value;
    }
    void print()
    {
        for(int i =0; i<m_size; i++)
        {
            cout << m_vals[i] << " ";
        }
        cout << "\n\n";
    }

    bool empty() const
    {
        return m_size==0;
    }

private:
    T m_vals[MAX_HEAP_SIZE];
    int m_size;
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


};

// 
// int main()
// {
//     Heap<int> h;
//     int inval = 0;
//     while (1)
//     {
//         cin >> inval;
//         if(inval == 0) 
//         {
//             h.Delete();
//             h.print();
//         }
//         else if(inval == -1)
//         {
//             exit(0);
//         }
//         else
//         {
//             h.Insert(inval);
//             h.print();
//         }
//     }
// }