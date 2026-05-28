#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <random>
using namespace std;



struct Item
{
	int weight;
	int cost;
};

int MaxSubjectToItemAndCapacity(vector<Item>& items, vector < vector<int>>& table, int k, int avail_capacity);
int MaxSubjectToCapacity(vector<Item> items, int capacity)
{
	vector< vector<int>> table(items.size(), vector<int>(capacity + 1, -1));
	return MaxSubjectToItemAndCapacity(items, table, items.size() - 1, capacity);
}

int MaxSubjectToItemAndCapacity(vector<Item>& items, vector < vector<int>>& table, int k, int avail_capacity)
{
	if (k < 0)return 0;

	if (table[k][avail_capacity] == -1)
	{
		//figure without item
		int without = MaxSubjectToItemAndCapacity(items, table, k - 1, avail_capacity);
		//figure with item
		int with = (items[k].weight > avail_capacity) ? 
			0 : (items[k].cost + MaxSubjectToItemAndCapacity(items, table, k - 1, avail_capacity - items[k].weight));

		//find max, store in cache
		table[k][avail_capacity] = max(with, without);
	}
	return table[k][avail_capacity];
}

// int main()
// {
// 	// your code goes here
// 	return 0;
// }