# Sudoku

Input is a sequence of 81 interers ranging [0,9], encoding the initial board position, left-to-right and top-down, with 0 for empty squares; output should be a sequence of numbers ranging [1,9].

Build:
    make clean
    make

Run Sudoku directly with board string:
    e.g. ./sudoku 003020600900305001001806400008102900700000008006708200002609500800203009005010300

Run Sudoku and get input from board configuration files:
    e.g. ./test_sudoku.sh easy 5
