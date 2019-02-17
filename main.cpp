//main.cpp, Guilherme Gomes
//Main file

#include "header.h"
#include <iostream>
#include <stdio.h>
using namespace std;

#define OUTPUT_FILENAME "output.csv" //any changes here must be replied in the makefile

int main(int argc, char** argv){

  cout << endl << "SUDOKU SOLVER made by Guilherme Gomes" << endl;

  FILE * input;
  if(! (input = fopen(argv[1], "r") )){
    cout << "Error openning sudoku file. Exiting." << endl;
    return -1;
  }

  FILE * output;
  if(! (output = fopen(OUTPUT_FILENAME, "w") )){
    cout << "Error creating output file. Exiting." << endl;
    return -1;
  }

  cout << endl << "  This is the sudoku puzzle you entered: " << endl;

  Sudoku S;

  S.writeFromFile(input);
  S.printSudoku();

  int changes = 1;
  int count = -1;

  //main program loop, do some standard checks first,
  //if we get stuck do some more advanced checks
  while(changes){
    changes = 0;
    count ++;
    changes += S.checkSectors();
    changes += S.checkRows();
    changes += S.checkCols();
    changes += S.checkCells();
  }

  cout << endl << "  And this is your solved sudoku: " << endl;
  S.printSudoku();
  cout << "  We wrote this solution in a file named " << OUTPUT_FILENAME << endl;

  cout << endl << " i = " << count<< endl;

  // S.showPossibilites();
  S.writeToFile(output);

  fclose(input);
  fclose(output);

  return 1;
}
