#include "sudoku.h"

using namespace std;

vector<vector<int> > Board::conflictGroups;
vector<vector<int> > Board::cellPeers;

//typedef std::mt19937 MyRNG;
//uint32_t seed_val;
//
//MyRNG rng;
//
//std::uniform_int_distribution<uint32_t> uint_dist100(0,100);

Cell::Cell(int pos, char digit)
{
    name = string(1, 'A' + pos/9) + to_string(1 + pos%9);
    if (digit == '.' || digit == '0')
    {
        value = EMPTY;
        for (int i = 0; i < 9; i++)
        {
            domain.set(i);
        }
    }
    else
    {
        value = digit - '1';
        domain.reset();
        domain.set(value);
    }
}

Cell::Cell(const Cell& cell)
{
    this->copyFrom(cell);
}

void Cell::copyFrom(const Cell& cell)
{
    this->name = cell.name;
    this->value = cell.value;
    this->domain = cell.domain;
}

vector<char> Cell::getDomainList()
{
    vector<char> dl;
    for (char i = 0; i < 9; i++)
    {
        if (domain.test(i))
        {
            dl.push_back(i);
        }
    }

    return dl;
}

void Cell::findAndDelete(const char val)
{
    domain.reset(val);
    if (domain.count() == 1)
    {
        for (int i = 0; i < 9; i++)
        {
            if (domain.test(i))
            {
                assign(i);
                break;
            }
        }
    }
}

void Cell::assign(const char val)
{
    value = val;
    domain.reset();
    domain.set(val);
}

Board::Board(const string& boardString)
{
    assert(boardString.size() == 81);

    // Initialize random function with seed
//    rng.seed(seed_val);

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
    for (int row = 0; row < 9; row++)
    {
        vector<int> cg;
        for (int col = 0; col < 9; col++)
        {
            cg.push_back(row*9 + col);
        }
        conflictGroups.push_back(cg);
    }

    // Columns
    for (int col = 0; col < 9; col++)
    {
        vector<int> cg;
        for (int row = 0; row < 9; row++)
        {
            cg.push_back(row*9 + col);
        }
        conflictGroups.push_back(cg);
    }

    // Blocks
    for (int row = 0; row < 9; row += 3)
    {
        for (int col = 0; col < 9; col += 3)
        {
            vector<int> cg;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    cg.push_back((row+i)*9 + (col+j));
                }
            }
            conflictGroups.push_back(cg);
        }
    }
}

void Board::initializeCellPeers()
{
    for (int i = 0; i < cells.size(); i++)
    {
        vector<int> peers;
        for (auto &cg : conflictGroups)
        {
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
        for (vector<int>::iterator iter = peers.begin(); iter != peers.end(); iter++)
        {
            if (*iter == i)
            {
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
        if (c->domain.count() != 1)
            return false;
    }
    return true;
}

int Board::getMostConstrainedCellIndex()
{
    int idx = -1;
    // Starts with 10 in case the board is completely empty
    int domainSize = 10;
    for (int i = 0; i < cells.size(); i++)
    {
        if (cells[i]->isAssigned())
        {
            continue;
        }

        if (cells[i]->domain.count() < domainSize)
        {
            idx = i;
            domainSize = cells[i]->domain.count();
        }
    }

    return idx;
}

vector<char> Board::getValueOrder(const int cell, const vector<int>& peers)
{
//    assert(cells[cell]->domain.count() > 1);
//    vector<char> valueList = cells[cell]->getDomainList();
//
//    int rv = uint_dist100(rng);
//    if (rv%10 == 5)
//    {
//        cout << "Random permutation over values" << endl;
//        random_shuffle(valueList.begin(), valueList.end());
//        for (int i = 0; i < valueList.size(); i++)
//        {
//            cout << (int)valueList[i] << " ";
//        }
//        cout << endl;
//    }
//
//
//    return valueList;

    vector<char> order;
    int valueCount[9] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (cells[cell]->domain.test(i))
        {
            valueCount[i]++;
            for (auto &p : peers)
            {
                if (cells[p]->domain.test(i))
                {
                    valueCount[i]++;
                }
            }
        }
    }

    int max = 0;
    for (int i = 0; i < 9; i++)
    {
        if (valueCount[i] > max)
        {
            max = valueCount[i];
        }

    }

    // Order high occurance --> low occurance
//    while (max > 0)
//    {
//        for (int i = 0; i < 9; i++)
//        {
//            if (valueCount[i] == max)
//            {
//                order.push_back(i);
//            }
//        }
//        max--;
//    }

    // Order low occurance --> high occurance
    for (int j = 1; j <= max; j++)
    {
        for (int i = 0; i < 9; i++)
        {
            if (valueCount[i] == j)
            {
                order.push_back(i);
            }
        }
    }
    return order;
}

bool Board::eliminateTwins(const vector<int>& cg, const int idx1, const int idx2)
{
    vector<char> twinVal;
    for (int i = 0; i < 9; i++)
    {
        if (cells[idx1]->domain.test(i))
        {
            twinVal.push_back(i);
        }
    }

    assert(twinVal.size() == 2);

    for (auto &c : cg)
    {
        if (c == idx1 || c == idx2)
        {
            continue;
        }

        cells[c]->findAndDelete(twinVal[1]);
        cells[c]->findAndDelete(twinVal[0]);
        if (cells[c]->isAssigned())
        {
            if (!eliminateCell(c))
            {
                return false;
            }
        }

        if (cells[c]->domain.count() == 0)
        {
            return false;
        }
    }

    return true;
}

bool Board::eliminateCell(const int idx)
{
    // Only eliminate cells that are assigned a value
    if (cells[idx]->isAssigned())
    {
        for (auto &p : cellPeers[idx])
        {
            if (cells[p]->domain.test(cells[idx]->value))
            {
                cells[p]->domain.reset(cells[idx]->value);

                // If peer have only one element,
                // assign value and eliminate peer's peers.
                if (cells[p]->domain.count() == 1)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (cells[p]->domain.test(i))
                        {
                            cells[p]->assign(i);
                            break;
                        }
                    }
                    //cells[p]->assign(cells[p]-);
                    if (!eliminateCell(p))
                    {
                        return false;
                    }
                }
            }

            if (cells[p]->domain.count() == 0)
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
        for (int i = 0; i < 9; i++)
        {
            if (!cells[c]->domain.test(i))
            {
                continue;
            }

            bool assigned = false;
            for (auto &p : cg)
            {
                // Skip self
                if (p == c)
                {
                    continue;
                }

                if (cells[p]->domain.test(i))
                {
                    assigned = true;
                    break;
                }
            }
            if (!assigned)
            {
                cells[c]->assign(i);
                break;
            }
        }

        // If cell got assigned, prune again
        if (cells[c]->isAssigned())
        {
            if (!eliminateCell(c))
            {
                return false;
            }
        }

        // Naked Twins strategy
        if (cells[c]->domain.count() == 2 && find(twinExplored.begin(), twinExplored.end(), c) == twinExplored.end())
        {
            for (auto &twin : cg)
            {
                if (twin != c && cells[twin]->domain.count() == 2 &&
                        cells[c]->domain == cells[twin]->domain)
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

    // Get an ordered value list
    vector<char> order = getValueOrder(mccIdx, cellPeers[mccIdx]);
    for (auto &val : order)
    {
        assert(cells[mccIdx]->domain.test(val));

        Board *newBoard = new Board(*this);
        newBoard->cells[mccIdx]->assign(val);
        if (newBoard->solve() == true)
        {
            this->copyResultFrom(*newBoard);
            delete newBoard;
            return true;
        }

        delete newBoard;
    }
//    for (int i = 0; i < 9; i++)
//    {
//        if (!cells[mccIdx]->domain.test(i))
//        {
//            continue;
//        }
//
//        Board *newBoard = new Board(*this);
//        newBoard->cells[mccIdx]->assign(i);
//        if (newBoard->solve() == true)
//        {
//            this->copyResultFrom(*newBoard);
//            delete newBoard;
//            return true;
//        }
//
//        delete newBoard;
//    }

    return false;
}

