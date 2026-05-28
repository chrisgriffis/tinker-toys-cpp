#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <stdlib.h>
#include <unordered_set>
#include <random>
#include <numeric>
using namespace std;



struct node
{
	node* next;
	int data;
};
node* reverse(node* head)
{
	if (head == nullptr)return nullptr;
	node* prev(nullptr);
	node* current(head);
	while (current && current->next)
	{
		node* temp(current->next);
		current->next = prev;
		prev = current;
		current = temp;
	}
	current->next = prev;
	return current;
}

//calc max profit
void maxprof()
{
	vector<float> input(20);
	float f = -1.0f;
	generate(input.begin(), input.end(), [&f]
	{
		float temp = f;
		f += 0.25;
		return temp;
	});
	shuffle(input.begin(), input.end(), default_random_engine{});
	float minprice(0), maxprof(0);
	for (auto f : input)
	{
		if (f < minprice)
			minprice = f;
		if (f-minprice > maxprof)
		{
			maxprof = f - minprice;
		}
	}
	for (auto i : input) cout << i << " "; cout << endl;
	cout << "maxprof " << maxprof << endl << endl;
}

void reverselist()
{
	vector<int> input(20);
	iota(input.begin(), input.end(), -10);
	shuffle(input.begin(), input.end(), default_random_engine{});
	node *head,*current;
	head = new node{nullptr,50};
	current = head;
	for (auto n : input)
	{
		current->next = new node{ nullptr,n };
		current = current->next;
	}
	current = head;
	while (current)
	{
		cout << current->data << " "; 
		current = current->next;
	}
	cout << endl;
	head = reverse(head);
	current = head;
	while (current)
	{
		cout << current->data << " "; 
		current = current->next;
	}
	cout << endl;
	head = reverse(head);
	current = head;
	while (current)
	{
		cout << current->data << " ";
		current = current->next;
	}
	cout << endl;
}

//calc maxsubarray
void maxsubarray()
{
	vector<float> input(20);
	iota(input.begin(), input.end(), -10);
	shuffle(input.begin(), input.end(), default_random_engine{});
	int sum(0), minsum(0), maxsum(0);
	for (auto i : input)
	{
		sum += i;
		if (sum < minsum)
			minsum = sum;
		if (sum - minsum > maxsum)
			maxsum = sum - minsum;
	}
	for (auto i : input) cout << i << " "; cout << endl;
	cout << "minsum " << minsum << "; max: " << maxsum << endl << endl;
}


//calc clock
void clock()
{
	int hour = 11;
	int minut = 5;
	float hourangle = 360 * (hour / 12.0 + (1 / 12.0)*(minut / 60.0));
	float minangle = 360 * (minut / 60.0);
	float cand = fabsf(minangle - hourangle);
	cout << "hour " << hour << " min " << minut << " angle " << min(cand,360-cand) << endl << endl;
}

int main()
{
	maxprof();
	maxsubarray();
	clock();
	reverselist();
	return 0;
}