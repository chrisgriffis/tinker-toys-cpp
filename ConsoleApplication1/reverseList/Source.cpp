#include <iostream>
using namespace std;

struct node { node(node* n = nullptr, int v = 0) :next(n), val(v) {} node* next; int val; };

void print(node* head)
{
	node* current(head);
	while (current) { cout << current->val << ' '; current = current->next; }
	cout << endl;
}

node* reverse(node* head)
{
	if (head == nullptr) return head;
	node* previous(nullptr);
	node* current(head);
	while (current && current->next)
	{
		node* rest = current->next;
		current->next = previous;
		previous = current;
		current = rest;
	}
	current->next = previous;
	return current;
}

int main() {
	// your code goes here
	node* head =
		new node(
			new node(
				new node(
					new node(
						new node(
							new node(
								new node(
									new node(
										new node(), 1), 2), 3), 4), 5), 6), 7), 8);
	print(head);
	head = reverse(head);
	print(head);
	return 0;
}