#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct node { node() {} node(int v) :val(v) {} int val; node* next; };

void erase(node* p)
{
    if (p && p->next)
    {
        node* temp(p->next);
        swap(p->val, temp->val);
        p->next = temp->next;
        //delete temp;
    }
}

node* reverse(node* p)
{
    if (p && p->next)
    {
        node *prev(nullptr), *curr(p);
        while (curr->next)
        {
            node* next(curr->next);
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        curr->next = prev;
        return curr;
    }
    return p;
}

node* merge(node* L, node* R)
{
    node dummyhead;
    node* curr = &dummyhead;

    while (L && R)
    {
        if (L->val < R->val)
        {
            curr->next = L;
            L = L->next;
        }
        else
        {
            curr->next = R;
            R = R->next;
        }
        curr = curr->next;
    }
    curr->next = (L) ? L : R;
    curr = dummyhead.next;
    return curr;
}

node* sort(node* L)
{
    if (L && L->next)
    {
        node *preslow(nullptr), *slow(L), *fast(L);
        while (slow && fast && fast->next)
        {
            preslow = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        preslow->next = nullptr;
        return merge(sort(L), sort(slow));
    }
    return L;
}

void print(node* L)
{
    node* curr(L);
    while (curr)
    {
        cout << curr->val << " ";
        curr = curr->next;
    }
}
int main()
{
    vector<int> ints(30);
    iota(ints.begin(), ints.end(), -7);
    random_shuffle(ints.begin(), ints.end());
    vector<node> nodes(ints.begin(), ints.end());
    for (auto& n : nodes) { cout << n.val << " "; } cout << endl;
    for (int Index = 0; Index + 1 < nodes.size(); Index++)
    {
        nodes[Index].next = &nodes[Index + 1];
    }
    nodes[nodes.size() - 1].next = nullptr;
    node* l1(&*nodes.begin());
    print(l1);
    cout << endl;

    l1 = reverse(l1);
    print(l1);
    cout << endl;

    l1 = sort(l1);
    print(l1);
    cout << endl;

    l1 = reverse(l1);
    print(l1);
    cout << endl;

    for (; l1->next; )
    {
        erase(l1);
        l1 = reverse(l1);
        print(l1);
        cout << endl;
    }

    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}