# Sudoku

Input is a sequence of 81 interers ranging [0,9], encoding the initial board position, left-to-right and top-down, with 0 for empty squares; output should be a sequence of numbers ranging [1,9].

### Environment:

This program has been tested on a Ubuntu 16.04.1 64-bit machine. The detailed hardware and compiler configurations are:

```sh
CPU:      Intel(R) Core(TM) i7-6700 CPU @ 3.40GHz
Memory:   8G
Compiler: g++-5.4.0
```

### Build:

```sh
$ make clean
$ make
```

### Run Sudoku directly with board string:

```sh
$ ./sudoku 003020600900305001001806400008102900700000008006708200002609500800203009005010300
```

### Run Sudoku and get input from board configuration files:

```sh
$ ./test_sudoku.sh easy 5
```

### Run all board configurations from files:

```sh
$ ./test_sudoku_all.sh easy
```
