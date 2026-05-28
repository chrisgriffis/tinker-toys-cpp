#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <functional>

using namespace std;
ostream& operator<<(ostream&, const vector<pair<int, int>>&);

template<typename T, unsigned R, unsigned C>
vector<pair<int, int>> 
get_orphaned(pair<int, int>, const T(&)[R][C]);

int main()
{
    int arr[][5] = { 
    { 0, 1, 1, 0, 1 },
    { 0, 1, 0, 0, 1 },
    { 0, 1, 0, 1, 0 },
    { 0, 1, 1, 1, 0 } 
    };

    while(true)
    {
        int row;
        int col;
        cin >> row >> col;
        if (row < 0 || col < 0) break;
        cout 
            << "removing (" << row << "," << col << ") cuts off: " 
            << get_orphaned(make_pair(row, col), arr) << endl;
    }
    return 0;
}

template<typename T, unsigned R, unsigned C>
vector<pair<int, int>>  
get_orphaned(
    pair<int, int> cutpoint,
    const T(&descriptor)[R][C])
{
    using coord = pair<int, int>;
    vector<coord> retval; //list of coordinates that got cut

    std::function<void(coord, coord, bool)>
        dfssearch = [cutpoint, &descriptor, &retval, &dfssearch](
            coord prev,
            coord current,
            bool record = false)
    {
        //if target has been found, start recording, inclusive of cutpoint
        if (current == cutpoint) record = true;
        //all subsequent recursive calls get fwded 
        //the 'record' bool, once true it stays true.
        if (record) retval.emplace_back(current);

        //just a local helper for DRY
        auto consider_adj = [&](bool condition, coord adj)
        {
            if (condition && adj != prev && descriptor[adj.first][adj.second])
                dfssearch(current, adj, record);
        };

        coord
        left    { current.first, current.second - 1 },
        right   { current.first, current.second + 1 },
        down    { current.first + 1, current.second },
        up      { current.first - 1, current.second };

        consider_adj(current.second > 0,       left);
        consider_adj(current.second < (C - 1), right);
        consider_adj(current.first  < (R - 1), down);
        consider_adj(current.first  > 0,       up);
    };

    //nothing has actually happened yet, that was just a dfs definition
    for (int col = 0; retval.size() == 0 && col < C; col++)
        if (descriptor[0][col]) //for each attachment to the roof
            dfssearch(coord{ -1, col }, coord{ 0, col }, false);
    return retval;
}

ostream& operator<<(ostream& s, const vector<pair<int, int>>& v)
{
    for_each(v.begin(), v.end(), [](const pair<int, int>& p) {
        cout << "(" << p.first << "," << p.second << ") ";
    });
    return s;
}