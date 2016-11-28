#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class Cell
{
public:
    string name;
    vector<char> domain;

    Cell(int pos, char digit);
    Cell(const Cell& cell);
    ~Cell()
    {
    };

    virtual void copyFrom(const Cell& cell);
    virtual void findAndDelete(const char val);
    virtual void assign(const char c);
    virtual bool isAssigned()
    {
        return (domain.size() == 1);
    };
    virtual char getLeastConstrainedValue();
};

class Board
{
private:
    vector<Cell *> cells;

    // Each group stores a bunch of cell indices
    static vector<vector<int> > conflictGroups;
    // Indices of peers for each cell
    static vector<vector<int> > cellPeers;

public:
    Board(const string& boardString);
    Board(const Board& board);
    ~Board();

    virtual void initializeCells(const string& boardString);
    virtual void initializeConflictGroups();
    virtual void initializeCellPeers();
    virtual void copyResultFrom(const Board& board);
    virtual bool isSolved();
    virtual int getMostConstrainedCellIndex();
    virtual bool eliminateTwins(const vector<int>& cg, const int idx1, const int idx2);
    virtual bool eliminateCell(const int idx);
    virtual bool eliminateConflictGroup(const vector<int>& cg);
    virtual bool constraintPropagation();
    virtual bool solve();

    virtual void printBoardString()
    {
        for (auto &c : cells)
        {
            cout << c->domain[0];
        }
        cout << endl;
    }

    virtual void printBoard()
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
                if (cells[i*9 + j]->isAssigned()) {
                    cout << cells[i*9 + j]->domain[0] << ' ';
                } else {
                    cout << ". ";
                }
            }
            cout << "|" << endl;
        }
        cout << "  +-------+-------+-------+" << endl << endl;
    };

    virtual void printConflictGroups()
    {
        for (auto &cg : conflictGroups)
        {
            for (int i = 0; i < 9; i++)
            {
                cout << cg[i] << " ";
            }
            cout << endl;
        }
    }

    void printDomains()
    {
        cout << endl;
        cout << "     1          2          3           4          5          6           7          8          9" << endl;
        for (int i = 0; i < 9; i++) {
            if (i%3 == 0)
            cout << "  +----------------------------------+---------------------------------+---------------------------------+" << endl;
            cout << string(1, 'A' + i) << " | ";
            for (int j = 0; j < 9; j++) {
                if (j!=0 && j%3 == 0) {
                    cout << "|";
                }
                cout << " ";
                vector<char> dm = cells[i*9 +j]->domain;
                for (auto &c : dm) {
                    cout << c;
                }

                for (int s = dm.size(); s <= 9; s++) {
                    cout << " ";
                }
            }
            cout << "|" << endl;
        }
        cout << "  +----------------------------------+---------------------------------+---------------------------------+" << endl;
    }
};

