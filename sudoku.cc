#include "sudoku.h"

using namespace std;

vector<vector<int> > Board::conflictGroups;
vector<vector<int> > Board::cellPeers;

Cell::Cell(int pos, char digit)
{
    name = string(1, 'A' + pos/9) + to_string(1 + pos%9);
    if (digit == '.' || digit == '0')
    {
        for (char c = '1'; c <= '9'; c++)
        {
            domain.push_back(c);
        }
    }
    else
    {
        domain.push_back(digit);
    }
}

Cell::Cell(const Cell& cell)
{
    this->copyFrom(cell);
}

void Cell::copyFrom(const Cell& cell)
{
    this->name = cell.name;
    this->domain = cell.domain;
}

void Cell::findAndDelete(const char val)
{
    vector<char>::iterator it = find(domain.begin(), domain.end(), val);
    if (it != domain.end())
    {
        domain.erase(it);
        if (domain.size() == 1)
        {
            assign(domain[0]);
        }
    }

}

void Cell::assign(const char c)
{
    domain.clear();
    domain.push_back(c);
}

char Cell::getLeastConstrainedValue()
{
    assert(domain.size() > 1);
    return domain[0];
}

Board::Board(const string& boardString)
{
    assert(boardString.size() == 81);

    initializeCells(boardString);
    initializeConflictGroups();
    initializeCellPeers();
}

Board::Board(const Board& board)
{
    for (auto &cell : board.cells)
    {
        this->cells.push_back(new Cell(*cell));
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

void Board::initializeCells(const string& boardString)
{
    for (int i = 0; i < boardString.size(); i++)
    {
        cells.push_back(new Cell(i, boardString[i]));
    }
}

void Board::initializeConflictGroups()
{
    // Rows
    for (int row = 0; row < 9; row++) {
        vector<int> cg;
        for (int col = 0; col < 9; col++) {
            cg.push_back(row*9 + col);
        }
        conflictGroups.push_back(cg);
    }

    // Columns
    for (int col = 0; col < 9; col++) {
        vector<int> cg;
        for (int row = 0; row < 9; row++) {
            cg.push_back(row*9 + col);
        }
        conflictGroups.push_back(cg);
    }

    // Blocks
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            vector<int> cg;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    cg.push_back((row+i)*9 + (col+j));
                }
            }
            conflictGroups.push_back(cg);
        }
    }
}

void Board::initializeCellPeers()
{
    for (int i = 0; i < cells.size(); i++) {
        vector<int> peers;
        for (auto &cg : conflictGroups) {
            // Skip if cell is not in the conflict group
            if (find(cg.begin(), cg.end(), i) == cg.end())
            {
                continue;
            }

            peers.insert(peers.end(), cg.begin(), cg.end());
        }

        // Remove duplicates
        sort(peers.begin(), peers.end());
        peers.erase(unique(peers.begin(), peers.end()), peers.end());

        // Remove self
        for (vector<int>::iterator iter = peers.begin(); iter != peers.end(); iter++) {
            if (*iter == i) {
                peers.erase(iter);
                break;
            }
        }

        assert(peers.size() == 20);
        cellPeers.push_back(peers);
    }
}

void Board::copyResultFrom(const Board& board)
{
    for (int i = 0; i < cells.size(); i++)
    {
        cells[i]->copyFrom(*board.cells[i]);
    }

}

bool Board::isSolved()
{
    for (auto &c : cells)
    {
        if (c->domain.size() != 1)
            return false;
    }
    return true;
}

int Board::getMostConstrainedCellIndex()
{
    int idx = -1;
    int domainSize = 9;
    for (int i = 0; i < cells.size(); i++)
    {
        if (cells[i]->isAssigned())
        {
            continue;
        }

        if (cells[i]->domain.size() < domainSize)
        {
            idx = i;
            domainSize = cells[i]->domain.size();
        }
    }

    return idx;
}

bool Board::eliminateTwins(const vector<int>& cg, const int idx1, const int idx2)
{
    for (auto &c : cg)
    {
        if (c == idx1 || c == idx2)
        {
            continue;
        }

        cells[c]->findAndDelete(cells[idx1]->domain[1]);
        cells[c]->findAndDelete(cells[idx1]->domain[0]);
        if (cells[c]->isAssigned())
        {
            if (!eliminateCell(c))
            {
                return false;
            }
        }

        if (cells[c]->domain.size() == 0)
        {
            return false;
        }
    }

    return true;
}

bool Board::eliminateCell(const int idx)
{
    // Only eliminate cells that are assigned a value
    if (cells[idx]->isAssigned()) {
        for (auto &p : cellPeers[idx])
        {
            vector<char>::iterator it = find(cells[p]->domain.begin(), cells[p]->domain.end(),
                    cells[idx]->domain[0]);
            if (it != cells[p]->domain.end())
            {
                cells[p]->domain.erase(it);

                // If peer have only one element,
                // assign value and eliminate peer's peers.
                if (cells[p]->domain.size() == 1)
                {
                    cells[p]->assign(cells[p]->domain[0]);
                    if (!eliminateCell(p))
                    {
                        return false;
                    }
                }
            }

            if (cells[p]->domain.size() == 0)
            {
                return false;
            }
        }
    }

    return true;
}

bool Board::eliminateConflictGroup(const vector<int>& cg)
{
    // explored list for naked twin strategy
    vector<int> twinExplored;
    for (auto &c : cg)
    {
        if (cells[c]->isAssigned())
        {
            continue;
        }

        // If value is not in any peer's domain, it must be the value of the current cell
        for (auto &val : cells[c]->domain) {
            bool assigned = false;
            for (auto &p : cg) {
                // Skip self
                if (p == c) {
                    continue;
                }

                if (find(cells[p]->domain.begin(), cells[p]->domain.end(), val) != cells[p]->domain.end())
                {
                    assigned = true;
                    break;
                }
            }
            if (!assigned)
            {
                cells[c]->assign(val);
                break;
            }
        }

        // If cell got assigned, prune again
        if (cells[c]->isAssigned())
        {
            eliminateCell(c);
        }

        // Naked Twins strategy
        if (cells[c]->domain.size() == 2 && find(twinExplored.begin(), twinExplored.end(), c) == twinExplored.end())
        {
            for (auto &twin : cg)
            {
                // Since domain is sorted, we can simply check one by one
                if (twin != c && cells[twin]->domain.size() == 2 &&
                        cells[c]->domain[0] == cells[twin]->domain[0] &&
                        cells[c]->domain[1] == cells[twin]->domain[1])
                {
                    // prune these two value from other domains in the cg
                    if (!eliminateTwins(cg, c, twin))
                    {
                        return false;
                    }

                    twinExplored.push_back(c);
                    twinExplored.push_back(twin);
                    break;
                }

            }

        }
    }

    return true;
}


bool Board::constraintPropagation()
{
    //cout << "Pruning cell domains..." << endl;
    for (int i = 0; i < cells.size(); i++)
    {
        if (eliminateCell(i) != true)
        {
            return false;
        }
    }

    //cout << "Enforcing constraints within conflict groups..." << endl;
    for (int i = 0; i < conflictGroups.size(); i++)
    {
        if (eliminateConflictGroup(conflictGroups[i]) != true)
        {
            return false;
        }
    }

    //printDomains();

    return true;
}

bool Board::solve()
{
    if (!constraintPropagation())
    {
        return false;
    }

    if (isSolved())
    {
        return true;
    }

    int mccIdx = getMostConstrainedCellIndex();
    if (mccIdx == -1)
    {
        return false;
    }

    for (auto &val : cells[mccIdx]->domain)
    {
        Board *newBoard = new Board(*this);
        newBoard->cells[mccIdx]->assign(val);
        //cout << "Assign cell: " << newBoard->cells[mccIdx]->name << " to value: " << val << endl;
        if (newBoard->solve() == true)
        {
            this->copyResultFrom(*newBoard);
            delete newBoard;
            return true;
        }

        delete newBoard;
    }

    return false;
}

