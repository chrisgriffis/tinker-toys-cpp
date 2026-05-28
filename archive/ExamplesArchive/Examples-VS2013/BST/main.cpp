#include "BST.h"
#include <iostream>
#include <cstdlib>

int main()
{
	int vals[] = {8,5,12,3,7,10,15,1,2,4,6,9,11,13,16,14};
	BST tree(vals);
	tree.LevelPrint();
	tree.ToDoublyLinkedListInPlace();
	tree.DoublyLinkedListPrint();
	std::cout << "\n\n";

	srand(1377);
	std::vector<int> vals2;
	for (int i = 0; i < 20 ; i++)
	{
		vals2.push_back(rand()%1000);
	}
	BST tree2(vals2);
	tree2.LevelPrint();
	tree2.ToDoublyLinkedListInPlace();
	tree2.DoublyLinkedListPrint();
	std::cout << "\n\n";

	srand(2344);
	std::vector<int> vals3;
	for (int i = 0; i < 500 ; i++)
	{
		vals3.push_back(rand()%2000);
	}
	BST tree3(vals3);
	tree3.LevelPrint();
	tree3.ToDoublyLinkedListInPlace();
	tree3.DoublyLinkedListPrint();

	return 0;
}
