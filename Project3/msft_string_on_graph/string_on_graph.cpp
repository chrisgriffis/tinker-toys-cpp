#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;


namespace my {
	struct graph
	{
		struct node;
		vector<node*> _edgelist;
	public:
		struct node
		{
			char _val;
			vector<node*> _adjs;
		};
		bool is_on_graph(string s) const 
		{
			//initialize the stack
			using entry = pair<node*, size_t>;
			stack<entry> stk;
			auto iter_to_curr_char = s.begin();
			//find all nodes that have first character
			//and push each to stack
			auto find_all = [](auto low, auto hi, char c) -> vector<node*> {
				vector<node*> res;
				auto found = low;
				do
				{
					found = find_if(found, hi, [&](auto curr_node) {
						return curr_node->_val == c;
					});
					if (found != hi) res.push_back(*found++);
				} while (found != hi);
				return res;
			};
			for(auto& e : find_all(_edgelist.begin(),_edgelist.end(),*iter_to_curr_char))
				stk.push(make_pair(e, 0));
			//keep track of current char, set to init.
			//while the stack isnt empty 
			while(!stk.empty())
			{
				auto curr_entry = stk.top();
				//if top matches first character
				if(curr_entry.first->_val == *iter_to_curr_char && 
					curr_entry.second == (iter_to_curr_char-s.begin()))
				{
					stk.pop();
					//if theres a next char
					if (next(iter_to_curr_char) != s.end())
					{
						//adv char
						++iter_to_curr_char;
						//add all adj nodes that match next char
						auto& adjs = curr_entry.first->_adjs;
						for (auto& e : find_all(adjs.begin(), adjs.end(), *iter_to_curr_char))
							stk.push(make_pair(e, iter_to_curr_char - s.begin()));
					}
					else return true;
				}
				else
				{
					//else revert to previous char (find came up empty)
					if(iter_to_curr_char != s.begin()) 
						--iter_to_curr_char;
					else return false;				
				}
			}
			return false;
		}
	};
}



int main()
{
	my::graph::node s{ 's',{} };
	my::graph::node t2{ 't',{ &s } };
	my::graph::node t1{ 't',{} };
	my::graph::node a2{ 'a',{ &t2 } };
	my::graph::node a1{ 'a',{ &t1 } };
	my::graph::node c{ 'c',{ &a2,&a1 } };
	t2._adjs.emplace_back(&a2);
	a2._adjs.emplace_back(&c);
	t1._adjs.emplace_back(&a1);
	a1._adjs.emplace_back(&c);
	my::graph g{ {&s,&t1,&t2,&a1,&a2,&c} };
	cout << boolalpha << g.is_on_graph(string("catatatats"));
	int debugwait__;
	cin >> debugwait__;
	return 0;
}