#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct node
{
	node(node* n = 0, int d = 0) :next(n), data(d) {}
	node* next;
	int data;
};

void deleteHead(node* head);
void print(node* ll);
node* makelist(int size = 1, bool random = false);
bool IsOverlappingNoCycles(node* l1, node* l2);
node* FindOverlapNoCycles(node* l1, node* l2);
void DeleteKthLast(node* l1, node* l2);
bool IsHasCycle(node* ll);
node* FindCycleStart(node* ll);


int main() {
	// your code goes here
	node* head1 = makelist(3);
	node* head2 = makelist(2);
	node* head3 = makelist(5);
	head1->next->next->next = head3;
	head2->next->next = head3;
	cout << boolalpha << IsOverlappingNoCycles(head1, head2) << endl;

	auto one = makelist(1); one->next = one;
	auto two = makelist(2); two->next->next = two;
	auto three = makelist(3); three->next->next->next = three;
	try
	{
		IsOverlappingNoCycles(head1, three);
	}
	catch (exception e)
	{
		cout << "exception: " << e.what() << endl;
	}
	auto cycleAt5 = makelist(5);
	cycleAt5->next->next->next->next->next = three;
	cout << boolalpha << IsHasCycle(one) << IsHasCycle(two) << IsHasCycle(three) << IsHasCycle(makelist(4)) << IsHasCycle(cycleAt5);

	return 0;
}

void deleteHead(node* head)
{
	if (head)
	{
		if (head->next)
		{
			head->data = head->next->data;
			node* temp = head->next;
			head->next = temp->next;
			delete temp;
		}
		else delete head;
	}
}

void print(node* ll)
{
	for (node* current = ll; current != 0; current = current->next)
	{
		cout << current->data << " ";
	}
	cout << endl;
}

node* makelist(int size, bool random)
{
	node* head = new node(0, size--);
	for (; size > 0; size--)
	{
		head = new node(head, size);
	}
	return head;
}


bool IsOverlappingNoCycles(node* l1, node* l2)
{
	if (IsHasCycle(l1) || IsHasCycle(l2))
		throw std::logic_error("has a cycle");
	node* current = l1;
	while (current && current->next)
		current = current->next;
	const node* l1end = current;
	current = l2;
	while (current && current->next)
		current = current->next;
	return l1end == current;
}

node* FindOverlapNoCycles(node* l1, node* l2)
{
	return nullptr;
}


void DeleteKthLast(node* l1, node* l2)
{

}


bool IsHasCycle(node* ll)
{
	if (ll && ll->next)
	{
		node* slow = ll;
		node* fast = ll;
		while (slow && fast && slow->next && fast->next && fast->next->next)
		{
			slow = slow->next;
			fast = fast->next->next;
			if (slow == fast) return true;
		}
	}
	return false;
}



node* FindCycleStart(node* ll)
{
	return nullptr;
}
