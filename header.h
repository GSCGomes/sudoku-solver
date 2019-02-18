//header.h, Guilherme Gomes
//Header file to declare some functions and the classes cell and sudoku
//which represents a cell of the sudoku grid and the sudoku grid itself

#ifndef HEADER_FILE
#define HEADER_FILE

#define TRUE 1
#define FALSE 0

#define MAX_DEPTH 47 //47 is the depth needed to solve a blank sudoku

#include <iostream>
#include <stdio.h>
using namespace std;

typedef struct Guess{
  int row;
  int col;
  int value;
} Guess;

//Receives coordinate and returns sector number, 0 for invalid coordinates
int sector(int, int);

//Given sector it returns its first row
int firstRowOfSector(int);

//Given sector it returns its first col
int firstColOfSector(int);

//Writes 0 in all elements of the matrix
void cleanMatrix(int***, int, int, int );

class Cell{

private:
  int value_;
  bool hashCheck_[9];

public:

  //Constructor
  Cell();

  //Destructor
  ~Cell();

  //Accessors
  //returns the value of the cell, 0 if the cell is empty
  int value();

  //receives a value and tells whether that cell can assume that value given the hashCheck_ array
  bool canItBe(int);

  //returns the number of TRUE values in the hashCheck_ array
  int possibilities();

  //Mutators
  //sets the value of the Cell
  void setValue(int);

  //tells to the cell that it can't be a certain value
  void cannotBe(int);

  //returns one possible value for that cell
  int onePossibleValue();

};

class Sudoku{

public:

  Cell cell[9][9];
  int solvedCells_;

  //Constructor
  Sudoku();

  //Destructor
  ~Sudoku();

  //Sets the value of a given cell and removes that value as a candidate to that cell's neighbors
  void setValue(int, int, int);

  //Fills the sudoku grid with data from file
  void writeFromFile(FILE * );

  //Writes the file with finished sudoku
  void writeToFile(FILE * );

  //Prints the whole sudoku on screen
  void printSudoku();

  //Checks cells' possibilities and writes the value in cells with only one possible value
  int checkCells();

  //Checks rows and returns number of changes
  int checkRows();

  //Checks collumns and returns number of changes
  int checkCols();

  //Checks sectors and returns number of changes
  int checkSectors();

  //Checks if sudoku is valid in returns TRUE or FALSE
  bool amIValid();

  //Receives a number and tells what cells have this number as a valid candidate
  //USEFUL WHILE DEVELOPING
  void showPossibilites(int);

  //Prints how many candidate each sudoku cell has
  //USEFUL WHILE DEVELOPING
  void showPossibilites();

};



#endif
