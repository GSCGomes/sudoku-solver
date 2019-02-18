CC = g++
CFLAGS = -Wall -c

sudoku-solver: functions.o sudoku.o main.o
	$(CC) -o sudoku-solver functions.o sudoku.o main.o

functions.o: functions.cpp header.h
	$(CC) $(CFLAGS) functions.cpp

sudoku.o : sudoku.cpp functions.cpp header.h
	$(CC) $(CFLAGS) sudoku.cpp

main.o : main.cpp sudoku.cpp functions.cpp header.h
	$(CC) $(CFLAGS) main.cpp

clean :
	rm -f *.o output.csv *~
	rm -f sudoku-solver
