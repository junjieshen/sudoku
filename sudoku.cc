#include <iostream>
#include <cmath>
#include <cassert>

#define H_BAR "------+-------+------"
#define V_BAR "|"

typedef int RC;

using namespace std;

inline int getOrder(int row, int col)
{
    return row*9+col;
}

RC printBoard(string &board) 
{
    if (board.size() != 81) return -1;
    cout << endl;
    for (int i = 0; i < 9; i++) {
        if (i!=0 && i%3 == 0)
            cout << H_BAR << endl;
        for (int j = 0; j < 9; j++) {
            if (j!=0 && j%3 == 0) {
                cout << V_BAR << ' ';
            }
            if (board[getOrder(i,j)] == '0') {
                cout << '.';
            } else {
                cout << board[getOrder(i,j)];
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << endl;
    return 0;
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        cout << "Please provide board parameter!" << endl;
        return -1;
    }

    string board(argv[1]);
    if (printBoard(board) != 0) {
        cout << "Incompatible board! Please check..." << endl;
        return -1;
    }

    return 0;
}
