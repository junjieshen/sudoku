CXX      = g++
CXXFLAGS = -g -std=c++11
DEPS     = sudoku.h
OBJS     = main.o sudoku.o

sudoku: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f sudoku *.o
