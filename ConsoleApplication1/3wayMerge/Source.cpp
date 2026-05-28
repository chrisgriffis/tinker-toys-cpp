//3-way merge

#include <utility>
#include <assert.h>
#include <queue>
#include <algorithm>
#include <functional>
#include <iostream>

enum Arrays { A, B, C };

template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, unsigned SIZE_M>
void kmerge(int(&a)[SIZE_A], int(&b)[SIZE_B], int(&c)[SIZE_C], int(&merged)[SIZE_M]);

int main()
{
    using namespace std;
    int Ax[] = { 1,5,4,2,5,7,9 };
    int Bx[] = { 6,3,7,9 };
    int Cx[] = { -4,-1,0 };
    int merged[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    sort(Ax, Ax + 7, std::greater<int>());
    sort(Bx, Bx + 4, std::greater<int>());
    sort(Cx, Cx + 3, std::greater<int>());
    kmerge(Ax, Bx, Cx, merged);
    for (auto i : merged)
    {
        cout << i << " ";
    }
}

//descending sort
template<unsigned SIZE_A, unsigned SIZE_B, unsigned SIZE_C, unsigned SIZE_M>
void kmerge(int(&a)[SIZE_A], int(&b)[SIZE_B], int(&c)[SIZE_C], int(&merged)[SIZE_M])
{
    std::priority_queue<std::pair<int, Arrays>> max_heap;

    max_heap.push(std::make_pair(a[0], A));
    max_heap.push(std::make_pair(b[0], B));
    max_heap.push(std::make_pair(c[0], C));

    int indexA(1), indexB(1), indexC(1), indexM(0);
    while ((indexA < SIZE_A || indexB < SIZE_B || indexC < SIZE_C) && indexM < SIZE_M)
    {
        auto currentmin = max_heap.top(); max_heap.pop();
        merged[indexM++] = currentmin.first;
        switch (currentmin.second)
        {
        case A:
        {
            if (indexA < SIZE_A)max_heap.push(std::make_pair(a[indexA++], A)); break;
        }
        case B:
        {
            if (indexB < SIZE_B)max_heap.push(std::make_pair(b[indexB++], B)); break;
        }
        case C:
        {
            if (indexC < SIZE_C)max_heap.push(std::make_pair(c[indexC++], C)); break;
        }
        }
    }
    while (indexM < SIZE_M && !max_heap.empty())
    {
        merged[indexM++] = max_heap.top().first;
        max_heap.pop();
    }

}
