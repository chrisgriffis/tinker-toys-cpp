#include <iostream>
using namespace std;

struct node { node(node* n = nullptr, int v = 0) :next(n), val(v) {} node* next; int val; };

void print(node* head)
{
	node* current(head);
	while (current) { cout << current->val << ' '; current = current->next; }
	cout << endl;
}

node* merge(node* left, node* right)
{
	node* dummy(new node());
	node* tail = dummy;
	while (left && right)
	{
		if (left->val < right->val)
		{
			tail->next = left;
			left = left->next;
		}
		else
		{
			tail->next = right;
			right = right->next;
		}
	}
	node* curr = left ? left : right;
	while (curr)
	{
		tail->next = curr;
		curr = curr->next;
	}
	node* ret = dummy->next;
	delete dummy;
	return ret;
}
node* sort(node* head)
{
	if (head == nullptr || head->next == nullptr)return head;
	node* slow(head);
	node* fast(head);
	while (slow && fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	node* temp = slow->next;
	slow->next = nullptr;
	return merge(sort(head), sort(temp));
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
										new node(), 71), 32), 83), 14), 905), -6), -77), 28);
	print(head);
	head = sort(head);
	print(head);
	return 0;
}