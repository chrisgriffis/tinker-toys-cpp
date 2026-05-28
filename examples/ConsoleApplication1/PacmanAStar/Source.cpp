#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <unordered_set>
#include <utility>
using namespace std;

using Pos = pair<int, int>;

struct Cell
{
    using Trail = vector<Pos>;
    Pos cellpos;
    Trail trail;
};

vector<Cell> getAdjCells(const vector< vector < char > >& gameboard, Cell c);

int main()
{
    Pos pacmanStart{ 0,0 };
    Pos food{ 0,0 };
    cin >> pacmanStart.first;
    cin >> pacmanStart.second;
    cin >> food.first;
    cin >> food.second;

    int rowcount, colcount;
    cin >> rowcount;
    cin >> colcount;

	//minheap of cells prioritized by minimizing cost function
    std::priority_queue <Cell,vector<Cell>,std::function<bool(Cell, Cell)>>
    nextMoves(
		//cost function lambda as constructor arg
        [pacmanStart, food](Cell a, Cell b)->bool
    {
        auto abs = [](int a) { return a < 0 ? -a : a; };
        auto metric = [abs](Pos start, Pos end)
        {
            return 
                abs(start.second - end.second) + 
                abs(start.first - end.first);
        };
        return //compare costs of each cell
            (a.trail.size() + metric(a.cellpos,food)) > //greater = min_heap
            (b.trail.size() + metric(b.cellpos,food));
    });

	//don't reprocess visited cells; keep track of them with hashtable
    std::unordered_set<Pos,std::function<unsigned int(Pos)>> 
    visited(
        rowcount*colcount,
        [colcount](Pos p) -> unsigned int
    {
        return p.first*colcount + p.second;
    });


	//read in the gameboard
    vector< vector < char > > gameboard;
    for (int row = 0; row < rowcount ; row++)
    {
        vector<char> wholerow(colcount);
        for (int col = 0; col < colcount ; col++)
            cin >> wholerow[col];
        gameboard.emplace_back(wholerow);
    }

    Cell::Trail victoryTrail;
    nextMoves.push(Cell{ pacmanStart,Cell::Trail{pacmanStart} });
    while (!nextMoves.empty())
    {
        auto currentCell = move(nextMoves.top());
        nextMoves.pop();
        //stopping condition
        if (currentCell.cellpos == food)
        {
            victoryTrail = move(currentCell.trail);
            break;
        }
        visited.insert(currentCell.cellpos);
        auto adjCells = getAdjCells(gameboard, currentCell);
        for (auto& possibleMove : adjCells)
            if (visited.find(possibleMove.cellpos) == visited.end())
                nextMoves.push(possibleMove);
    }

    cout << victoryTrail.size()-1 << endl;
    for (auto position : victoryTrail)
        cout << position.first << " " << position.second << endl;
}

vector<Cell> getAdjCells(const vector< vector < char > >& gameboard, Cell c)
{
    vector<Cell> adjCells;
    auto addCell = [&](int row, int col)
    {
        Pos p{ row, col };
        Cell::Trail t(c.trail);
        t.push_back(p);
        adjCells.emplace_back(Cell{ p,t });
    };
    //up
    if (c.cellpos.first - 1 >= 0 && gameboard[c.cellpos.first - 1][c.cellpos.second] != '%')
        addCell(c.cellpos.first - 1, c.cellpos.second);
    //left
    if (c.cellpos.second - 1 >= 0 && gameboard[c.cellpos.first][c.cellpos.second - 1] != '%')
        addCell(c.cellpos.first, c.cellpos.second - 1);
    //right
    if (c.cellpos.second + 1 < gameboard[0].size() && gameboard[c.cellpos.first][c.cellpos.second + 1] != '%')
        addCell(c.cellpos.first, c.cellpos.second + 1);
    //down
    if (c.cellpos.first + 1 < gameboard.size() && gameboard[c.cellpos.first + 1][c.cellpos.second] != '%')
        addCell(c.cellpos.first + 1, c.cellpos.second);
    return adjCells;
}
