#include "sudoku.h"

using namespace std;

Cell::Cell(char digit)
{
    if (digit == '.' || digit == '0')
    {
        value = '.';
        for (char c = '1'; c <= '9'; c++)
        {
            domain.push_back(c);
        }
    }
    else
    {
        value = digit;
        domain.push_back(digit);
    }
}

ConflictGroup::ConflictGroup()
{
}

Board::Board(string boardString)
{
    assert(boardString.size() == 81);
    for (int i = 0; i < boardString.size(); i++)
    {
        cells.push_back(new Cell(boardString[i]));
    }
}

Board::~Board()
{
    for (auto &i : cells)
    {
        delete i;
    }
    cells.clear();
}

void Board::printBoard()
{
    for (int i = 0; i < 9; i++) {
        if (i!=0 && i%3 == 0)
            cout << "------+------+------" << endl;
        for (int j = 0; j < 9; j++) {
            if (j!=0 && j%3 == 0) {
                cout << "| ";
            }
            cout << *cells[i*9 + j] << ' ';
        }
        cout << endl;
    }
}
