//#include "../Binary Tree/btree.h"
//#include "../SinglyLinkedList/SinglyLinkedList.h"
//#include <stdlib.h> //srand, rand
//#include "time.h"
#include "../List/list.h"
#include <iostream>
//int main(int argc, char **argv)
//{
//    srand(time(NULL));
//    Griffis::BinaryTree* bt = new Griffis::BinaryTree;
//    for (int i = 0; i < 2000 ; i++)
//    {
//        bt->CreateNode(i);
//    }
//    bt->GetNode(1999);
//
//    delete bt;
//}


// typedef Griffis::SimpleList<int> MyList;
// 
// int main(int argc, char **argv)
// {
// 	int arr[5] = {0,1,2,3,4};
// 	MyList mylist1(arr[1]);
// 	MyList mylist2(arr[2]);
// 	MyList mylist3(arr[3]);
// 	MyList mylist4(arr[4]);
// 
// 	mylist4.append(mylist3.append(mylist2.append(mylist1)));
// 
// 	mylist4.append(mylist4);
// 
// }

// struct FakePolicy
// {
//     static bool comparelt(const int& lhs, const int& rhs){return true;}
// };
// 
// int main(int argc, char **argv)
// {
//     int* x =  new int (1);
//     Griffis::TBinaryTree<int, FakePolicy> bt(x);
//     x =  new int(5);
//     bt.CreateNode(x, bt.m_Root);
//     x = new int(-6);
//     bt.CreateNode(x, bt.m_Root);
//     x = new int(3);
//     bt.CreateNode(x, bt.m_Root);
// }
// 


int main(int argc, char **argv)
{
    Griffis::List<int> list;
    for (int i = 0; i < 5 ; i++)
    {
    	list.put(i);
    }
    for (Griffis::List<int>::iterator it =  list.begin(); it != list.end(); ++it)
    {
        std::cout << (*it).key << std::endl;
    }

    return 0;
}