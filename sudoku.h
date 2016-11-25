#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

class Cell
{
public:
    friend ostream& operator<<(ostream &os, const Cell& cell)
    {
        return os << cell.value;
    };

    Cell(char digit);
    ~Cell()
    {
    };

private:
    char value;
    vector<char> domain;
    //vector<ConflictGroup *> conflictGroups;
    vector<Cell *> peers;

};

class ConflictGroup
{
public:
    friend ostream& operator<<(ostream &os, const ConflictGroup& cg)
    {
        for (auto &i : cg.cells)
        {
            os << i << " ";
        }
        return os << endl;
    };

    ConflictGroup();
    ~ConflictGroup();

private:
    vector<Cell *> cells;

};

class Board
{
public:
    Board(string boardString);
    ~Board();

    void printBoard();

private:
    vector<Cell *> cells;
    vector<ConflictGroup *> conflictGroups;

};

