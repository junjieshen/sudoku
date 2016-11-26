#include "sudoku.h"

using namespace std;

string grid1("003020600900305001001806400008102900700000008006708200002609500800203009005010300");
string grid2("4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......");
string hard1(".....6....59.....82....8....45........3........6..3.54...325..6..................");

int main(int argc, char* argv[])
{
    string boardString;
    if (argc == 2)
    {
        boardString = argv[1];
    }
    else
    {
        boardString = grid2;
    }

    Board board(boardString);
    if (board.solve() != true)
    {
        cout << "Failed to solve the board!" << endl;
    }

    return 0;
}
