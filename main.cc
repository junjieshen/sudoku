#include "sudoku.h"

using namespace std;

int main(int argc, char* argv[])
{
    string boardString(argv[1]);

    Board board(boardString);

    if (board.solve() != true)
    {
        cout << "Failed to solve the board!" << endl;
    }

    board.printBoardString();

    cout << endl << "Statistics:" << endl;
    //printf("Time: %.6fs\n", time_diff);
    board.printStats();

    return 0;
}
