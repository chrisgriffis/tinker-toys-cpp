#include <iostream>

using namespace std;


struct node_w_par
{
    int val;
    node_w_par* parent;
    node_w_par* left;
    node_w_par* right;
};

struct node_wo_par
{
    int val;
    node_wo_par* left;
    node_wo_par* right;
};

int successor(node_w_par* p)
{
    if (p)
    {
        //case 1 and case 2
        if (p->right)
        {
            node_w_par* curr(p->right);
            while (curr->left)
                curr = curr->left;
            return curr->val;
        }
        if (p->parent)
        {
            //case 3 and case 4 and case 5
            //parent to my left
            node_w_par* curr(p);
            while (curr->parent && curr->parent->right == curr)
            {
                curr = curr->parent;
            }
            if (curr->parent && curr->parent->left == curr)
                return curr->parent->val;
        }
    }
    return -100;
}



// int successor(node_wo_par* root, int val)
// {
// 
//     
//     //if first step is right subtree, find successor
// 
//     if (root)
//     {
//         if (val == root->val)
//         {
//             //if first step is left subtree, kepe track. if target has a right subtree, find successor.
//             if (root->right)
//             {
//                 node_wo_par* curr(root->right);
//                 while (curr->left)
//                     curr = curr->left;
//                 return curr->val;
//             }
//         }
//         else if (val > root->val)
//         {
// 
//         } 
//         else
//         {
//         }
//         //case 1
//         else if (p->parent)
//         {
//             //parent to my left
//             node_wo_par* curr(p->parent);
//             while (curr->parent->right == curr)
//             {
//                 curr = curr->parent;
//             }
//             if (curr->parent->left == curr)
//                 return curr->parent->val;
//         }
//         else return -1;
//     }
// 
// }

int main()
{
    //            /f
    //           b
    //          /  \d
    //         a  /c \e
    //
    node_w_par a{ -2,nullptr,nullptr,nullptr };
    node_w_par b{ -1,nullptr,nullptr,nullptr };
    node_w_par c{ 0,nullptr,nullptr,nullptr };
    node_w_par d{ 1,nullptr,nullptr,nullptr };
    node_w_par e{ 2,nullptr,nullptr,nullptr };
    node_w_par f{ 3,nullptr,nullptr,nullptr };

    b.left = &a;
    b.right = &d;
    b.parent = &f;

    a.parent = &b;
    c.parent = &d;
    e.parent = &d;

    d.left = &c;
    d.right = &e;
    d.parent = &b;

    f.left = &b;


    cout << successor(&a) << " -1" << endl;
    cout << successor(&b) << " 0" << endl;
    cout << successor(&c) << " 1" << endl;
    cout << successor(&d) << " 2" << endl;
    cout << successor(&e) << " 3" << endl;
    cout << successor(&f) << " -100" << endl;
    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}