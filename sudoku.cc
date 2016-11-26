#include "sudoku.h"

using namespace std;

Cell::Cell(int pos, char digit)
{
    name = string(1, 'A' + pos/9) + to_string(1 + pos%9);
    if (digit == '.' || digit == '0')
    {
        value = EMPTY;
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

void Cell::extractPeersFromConflictGroup(ConflictGroup* cg)
{
    if (!cg->hasCell(this))
    {
        return;
    }

    for (auto &c : cg->cells)
    {
        if (c != this && find(peers.begin(), peers.end(), c) == peers.end())
        {
            peers.push_back(c);
        }
    }
}

bool Cell::eliminate()
{
    if (value != EMPTY) {
        for (auto &peer : peers)
        {
            vector<char>::iterator it = find(peer->domain.begin(), peer->domain.end(), value);
            if (it != peer->domain.end())
            {
                peer->domain.erase(it);

                // If peer have only one element,
                // assign value and eliminate peer's peers.
                if (peer->domain.size() == 1)
                {
                    peer->value = peer->domain[0];
                    if (!peer->eliminate())
                    {
                        return false;
                    }
                }
            }

            if (peer->domain.size() == 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool ConflictGroup::eliminate()
{
    for (auto &cell : cells)
    {
        if (cell->value != EMPTY)
        {
            continue;
        }

        for (auto &c : cell->domain) {
            bool assigned = false;
            for (auto &peer_cell : cells) {
                if (peer_cell == cell) {
                    continue;
                }

                if (find(peer_cell->domain.begin(), peer_cell->domain.end(), c) != peer_cell->domain.end())
                {
                    assigned = true;
                    break;
                }
            }
            if (!assigned)
            {
                cell->value = c;
                cell->domain.clear();
                cell->domain.push_back(c);
                break;
            }
        }

        if (cell->value != EMPTY)
        {
            cell->eliminate();
        }

    }
    return true;
}

bool ConflictGroup::isSatisfied()
{
    for (auto &i : cells)
    {
        if (i->domain.size() != 1)
            return false;
    }
    return true;
}

Board::Board(const string& boardString)
{
    assert(boardString.size() == 81);

    initializeCells(boardString);
    initializeConflictGroups();

    for (auto &cell : cells) {
        for (auto &cg : conflictGroups) {
            cell->extractPeersFromConflictGroup(cg);
        }
    }
}

Board::~Board()
{
    for (auto &i : cells)
    {
        delete i;
    }
    cells.clear();

    for (auto &i : conflictGroups)
    {
        delete i;
    }
    conflictGroups.clear();
}

void Board::initializeCells(const string& boardString)
{
    for (int i = 0; i < boardString.size(); i++)
    {
        cells.push_back(new Cell(i, boardString[i]));
    }
}

void Board::initializeConflictGroups()
{
    for (int row = 0; row < 9; row++) {
        ConflictGroup *cg = new ConflictGroup();
        for (int col = 0; col < 9; col++) {
            cg->cells.push_back(cells[row*9 + col]);
        }
        conflictGroups.push_back(cg);
    }

    for (int col = 0; col < 9; col++) {
        ConflictGroup *cg = new ConflictGroup();
        for (int row = 0; row < 9; row++) {
            cg->cells.push_back(cells[row*9 + col]);
        }
        conflictGroups.push_back(cg);
    }

    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            ConflictGroup *cg = new ConflictGroup();
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    cg->cells.push_back(cells[(row+i)*9 + (col+j)]);
                }
            }
            conflictGroups.push_back(cg);
        }
    }
}

bool Board::constraintPropagation()
{
    cout << "Pruning cell domains..." << endl;
//    cout << "Before: " << endl;
//    for (auto &cell : cells)
//    {
//        cout << cell->name << ": ";
//        cell->printDomain();
//    }

    for (auto &cell : cells)
    {
        if (cell->eliminate() != true)
        {
            return false;
        }
    }

//    cout << "After: " << endl;
//    for (auto &cell : cells)
//    {
//        cout << cell->name << ": ";
//        cell->printDomain();
//    }

    printBoard();

    cout << "Enforing constraints within conflict groups..." << endl;
    for (auto &cg : conflictGroups)
    {
        if (cg->eliminate() != true)
        {
            return false;
        }
    }

//    cout << "After: " << endl;
//    for (auto &cell : cells)
//    {
//        cout << cell->name << ": ";
//        cell->printDomain();
//    }
    printBoard();


    return true;
}

bool Board::solve()
{
    cout << "Original board..." << endl;
    printBoard();
    if (!constraintPropagation())
    {
        return false;
    }

    return true;
}
