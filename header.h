//header.h, Guilherme Gomes
//Header file to declare some functions and the classes cell and sudoku
//which represents a cell of the sudoku grid and the sudoku grid itself

#ifndef HEADER_FILE
#define HEADER_FILE

#define TRUE 1
#define FALSE 0

#include <stdio.h>

//Receives coordinate and returns sector number, 0 for invalid coordinates
int sector(int, int);


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

  //returns the only value that the cell can assume, check possibilities before calling this
  int onlyValue();

};

class Sudoku{

public:

  Cell cell[9][9];
  bool state_; //it's TRUE if the program can't find anything else to do

  //Constructor
  Sudoku();

  //Destructor
  ~Sudoku();

  //Accessors
  //Receives coordinates and returns value, 0 for an empty cell
  int value(int, int);

  //Returns TRUE if the program can't find anything else to do
  bool state();

  //Mutators
  //Sets the value of a given cell
  void setValue(int, int, int);

  //Sets sudoku state
  void setState(bool);

  //Fills the cells with data from file
  void writeFromFile(FILE * );

  //Prints the whole sudoku on screen
  void printSudoku();
};



#endif
