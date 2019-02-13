//main.cpp, Guilherme Gomes
//Main file

#include "header.h"
#include <iostream>
#include <stdio.h>
using namespace std;

#define OUTPUT_FILENAME "output.csv" //any changes here must be replied in the makefile

int main(int argc, char** argv){

  FILE * input;
  if(! (input = fopen(argv[1], "r") )){
    cout << "Error openning file. Exiting." << endl;
    return -1;
  }

  FILE * output;
  if(! (output = fopen(OUTPUT_FILENAME, "w") )){
    cout << "Error creating output file. Exiting." << endl;
    return -1;
  }

  Sudoku S;

  S.writeFromFile(input);
  S.printSudoku();





  return 1;
}
