/*
given a map to a collection of islands.

write an algorithm that determines the # of islands in the map :
int findNumOfIslands(char[][] map);

===+++===
=++++==++
========+
++++=====

*/
#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <functional>
#include <string>
#include <sstream>


int findNumOfIslands(std::vector<std::vector<char>> map)
{
    using namespace std;
    int island_count(0);
    const char island_piece = '=';
    using coord = pair<int, int>;
    struct coord_hasher { size_t operator()(const coord& c) const { return c.first * 100007 + c.second; } };
    unordered_set<coord, coord_hasher, equal_to<coord>> visited;

    std::function<void(coord)> DFS = [&](coord current) {
        if (map[current.first][current.second] == island_piece && visited.insert(current).second)
        {
            const int row_max(0 + map.size());
            const int col_max(0 + map.begin()->size());
            vector<coord> adjs{
                { current.first, current.second - 1 },
                { current.first, current.second + 1 },
                { current.first + 1, current.second },
                { current.first - 1, current.second }
            };

            for (auto adj : adjs)
            {
                if (adj.first >= 0 && adj.first < row_max && adj.second >= 0 && adj.second < col_max)
                    DFS(adj);
            }
        }
        else return;
    };

    for (auto row_itr = map.begin(); row_itr != map.end(); ++row_itr)
        for (auto col_itr = row_itr->begin(); col_itr != row_itr->end(); ++col_itr)
        {
            coord current{ row_itr - map.begin(),col_itr - row_itr->begin() };
            if (map[current.first][current.second] == island_piece  && visited.find(current) == visited.end())
            {
                ++island_count;
                DFS(current);
            }
        }
    return island_count;
}

int main()
{
    using namespace std;
    //string smap("===+++===\n=++++==++\n========+\n++++=====");
    stringstream ss(
        string{ "...===..=............................\n" }+
        string{ ".==.=..==............................\n" }+
        string{ "........=..........=.................\n" }+
        string{ "...===..=........===.................\n" }+
        string{ ".==.=..==...........=................\n" }+
        string{ "........=...........===.....===......\n" }+
        string{ "...===..=...........==......=........\n" }+
        string{ ".==.=..==...................=........\n" }+
        string{ "........=...................===......\n" }+
        string{ "====.................................\n" }
    );
    string line;
    vector<vector<char>> map;
    while (getline(ss, line))
    {
        map.emplace_back(vector<char>{line.begin(), line.end()});
    }
    int k;
    cout << (k = findNumOfIslands(map));
}

