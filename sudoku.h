#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cassert>

#define EMPTY -1
using namespace std;

class Cell
{
public:
    string name;
    char value;
    //vector<char> domain;
    bitset<9> domain;

    Cell(int pos, char digit);
    Cell(const Cell& cell);
    ~Cell()
    {
    };

    virtual void copyFrom(const Cell& cell);
    virtual void findAndDelete(const char val);
    virtual void assign(const char val);
    virtual bool isAssigned()
    {
        //return (domain.size() == 1);
        return (value != EMPTY);
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
            char val = '1' + c->value;
            cout << val; //c->domain[0];
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
                    char val = '1' + cells[i*9 + j]->value;
                    cout << val << ' ';
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
                for (int m = 0; m < 9; m++)
                {
                    if (cells[i*9 + j]->domain.test(m))
                    {
                        cout << m+1;
                    }
                }

                for (int s = cells[i*9 + j]->domain.count(); s <= 9; s++) {
                    cout << " ";
                }
            }
            cout << "|" << endl;
        }
        cout << "  +----------------------------------+---------------------------------+---------------------------------+" << endl;
    }
};

