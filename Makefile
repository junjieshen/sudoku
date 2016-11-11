all:
	g++ -O3 -o sudoku sudoku.cc

clean:
	rm -f sudoku *.o
