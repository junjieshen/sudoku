#include "sudoku.h"

using namespace std;

string grid1("003020600900305001001806400008102900700000008006708200002609500800203009005010300");
string grid2("4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......");
string hard1(".....6....59.....82....8....45........3........6..3.54...325..6..................");

int main(int argc, char* argv[])
{
//    if (argc != 2) {
//        cout << "Sample output:" << endl;
//        printBoard(grid1);
//        cout << "Please provide board parameter!" << endl;
//        return -1;
//    }

    Board board(grid1);
    board.printBoard();

    return 0;
}
