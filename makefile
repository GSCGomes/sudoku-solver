CC = g++
CFLAGS = -Wall -c
OUTPUT_FILENAME = output.csv

sudoku-solver: solver.o sudoku.o main.o
	$(CC) -o sudoku-solver solver.o sudoku.o main.o

solver.o: solver.cpp header.h
	$(CC) $(CFLAGS) solver.cpp

sudoku.o : sudoku.cpp solver.cpp header.h
	$(CC) $(CFLAGS) sudoku.cpp

main.o : main.cpp sudoku.cpp solver.cpp header.h
	$(CC) $(CFLAGS) main.cpp

clean :
	rm -f *.o output.csv *~
	rm -f sudoku-solver
