#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct cell { int row, col; };

bool tour(int startrow, int startcol, vector<cell>& result, vector<bool>& visited)
{
    //mark this visited
    visited[startrow * 8 + startcol] = true;
    //add self to path
    result.emplace_back(cell{ startrow,startcol });

    //stopping condition
    if (result.size() > 63) return true;

    //construct list of feasible options
    vector<cell> next;
    static constexpr int offset[][2] = { { 2,1 },{ 2,-1 },{ -2,1 },{ -2,-1 },{ 1,-2 },{ 1,2 },{ -1,-2 },{ -1,2 } };
    for (int Index = 0; Index < 8; Index++)
    {
        cell nextcell{ startrow + offset[Index][0], startcol + offset[Index][1] };
        if (nextcell.row >= 0 && nextcell.row < 8 && nextcell.col >= 0 && nextcell.col < 8 && !visited[nextcell.row * 8 + nextcell.col])
            next.emplace_back(nextcell);
    }

    //warnsdorff heuristic
    constexpr int neighborcount[] = {
        2,3,4,4,4,4,3,2,
        3,4,6,6,6,6,4,3,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        4,6,8,8,8,8,6,4,
        3,4,6,6,6,6,4,3,
        2,3,4,4,4,4,3,2
    };
    sort(next.begin(), next.end(), [&neighborcount](auto& l, auto& r) {
        return neighborcount[l.row * 8 + l.col] < neighborcount[r.row * 8 + r.col];
    });

    //try all feasible options, could be an empty list
    for (auto& e : next)
        if (tour(e.row, e.col, result, visited))
            return true;

    //no luck, backup
    if (result.size() > 0)
        result.pop_back();
    visited[startrow * 8 + startcol] = false;
    return false;
}

vector<cell> tour(bool wantclosed = false)
{
    vector<bool> visited(64, false);
    vector<cell> path;
    for (int Index = 0; Index < 64; Index++)
    {
        if (tour(Index / 8, Index % 8, path, visited))
            if (wantclosed)
            {
                static constexpr int offset[][2] = { { 2,1 },{ 2,-1 },{ -2,1 },{ -2,-1 },{ 1,-2 },{ 1,2 },{ -1,-2 },{ -1,2 } };
                for (int Index = 0; Index < 8; Index++)
                {
                    if (path[63].row + offset[Index][0] == path[0].row &&  path[63].col + offset[Index][1] == path[0].col)
                        return path;
                }
                path.swap(vector<cell>());
                visited.swap(vector<bool>(64, false));
            }
            else break;
    }
    return path;
}

int main()
{
    vector<cell> v(tour(true));
    int board[8][8] = { { 0 } };
    int step = 0;
    for (auto& e : v)
    {
        board[e.row][e.col] = ++step;
    }
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            cout << "\t" << board[row][col] << " ";
        }
        cout << endl << endl << endl;
    }
    int sdfgwert;
    cin >> sdfgwert;
    return 0;
}