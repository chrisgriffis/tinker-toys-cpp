#include <cstdio>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>
using namespace std;


struct trie
{
    trie():entries(0),root(new node){}
    void add(string s)
    {
        if(s.empty()) return;
        auto current=&root;
        for(auto c : s)
        {
            current=&(*current)->add(c);
        }
        (*current)->is_entry=true;
        ++(*current)->entries;
        ++entries;
    }
    
    int prefix_count(string s)
    {
        if(s.empty()) return 0;
        auto current=&root;
        for(auto c : s)
        {
            if(!(*current)->get(current,c))
            {
                return 0;
            }
        }
        return (*current)->entries;
    }
private:
    struct node
    {
        node():entries(0),is_entry(false){}
        unique_ptr<node>& add(char c)
        {
            auto child=childs.find(c);
            if(child==childs.end())
            {
                childs[c]=move(make_unique<node>());
            }
            ++entries;
            return childs[c];
        }
        bool get(unique_ptr<node>*& n, char c)
        {
            auto child=childs.find(c);
            if(child!=childs.end())
            {
                n=&((*child).second);
                return true;
            }
            return false;
        }

        bool is_entry;
        int entries;
        unordered_map<char,unique_ptr<node>> childs;
    };

    unique_ptr<node> root;
    int entries;
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int lines;
    string action;
    string partial;
    cin >> lines;
    trie t;
    while(0<lines--)
    {
        cin >> action;
        cin >> partial;
        if(action== "add")
        {
            t.add(partial);
        }
        else if(action== "find")
        {
            cout << t.prefix_count(partial) << endl;
        }
    }
    return 0;
}
