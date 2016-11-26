#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#define EMPTY '.'

using namespace std;

class ConflictGroup;

class Cell
{
public:
    string name;
    char value;
    vector<char> domain;
    vector<Cell *> peers;

    Cell(int pos, char digit);
    ~Cell()
    {
    };

    void extractPeersFromConflictGroup(ConflictGroup* cg);
    bool eliminate();

    void printPeers()
    {
        cout << name << ": ";
        for (auto &i : peers)
        {
            cout << i->name << " ";
        }
        cout << endl;
    }

    void printDomain()
    {
        for (auto &i : domain)
        {
            cout << i << " ";
        }
        cout << endl;
    }

};

class ConflictGroup
{
public:
    vector<Cell *> cells;

    ConflictGroup()
    {
    };

    ~ConflictGroup()
    {
    };

    bool eliminate();
    bool isSatisfied();
    bool hasCell(Cell* cell)
    {
        if (find(cells.begin(), cells.end(), cell) != cells.end())
        {
            return true;
        }
        return false;
    }

    void printConflictGroup()
    {
        for (auto &i : cells)
        {
            cout << i->name << " ";
        }
        cout << endl;
    }
};

class Board
{
public:
    Board(const string& boardString);
    ~Board();

    void initializeCells(const string& boardString);
    void initializeConflictGroups();
    bool constraintPropagation();
    bool solve();

    void printBoard()
    {
        cout << endl;
        cout << "    1 2 3   4 5 6   7 8 9 " << endl;
        cout << "  +-------+-------+-------+" << endl;
        for (int i = 0; i < 9; i++) {
            if (i!=0 && i%3 == 0)
                cout << "  +-------+-------+-------+" << endl;
            cout << string(1, 'A' + i) << " | ";
            for (int j = 0; j < 9; j++) {
                if (j!=0 && j%3 == 0) {
                    cout << "| ";
                }
                cout << cells[i*9 + j]->value << ' ';
            }
            cout << "|" << endl;
        }
        cout << "  +-------+-------+-------+" << endl << endl;
    };

private:
    vector<Cell *> cells;
    vector<ConflictGroup *> conflictGroups;
};

