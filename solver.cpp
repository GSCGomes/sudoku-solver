//solver.cpp, Guilherme Gomes
//This file contains some functions used to solve this problem


// #include "header.h"
#include <iostream>
using namespace std;

//Receives coordinate and returns sector number, 0 for invalid coordinates
int sector(int row, int col){
//the sudoku grid has 9 3x3 sectors which will be
//labelled as follows:
//   1 | 2 | 3
//   4 | 5 | 6
//   7 | 8 | 9
//

  int exit = 0;

  if((row < 0) || (col < 0) || (row > 8) || (col > 8)){ //if coordinates are invalid
    cout << "invalid coordinates." << " row = " << row << "  col = " << col << endl;
    return exit;
  }

  if(row <= 2){ //given coordinate is in the top three rows
    if(col <= 2) exit = 1;
    else if(col <= 5) exit = 2;
    else exit = 3;
  }
  else if(row <= 5){ //given coordinate is in the three middle rows
    if(col <= 2) exit = 4;
    else if(col <= 5) exit = 5;
    else exit = 6;
  }
  else if(row <= 8){ //given coordinate is in the three bottom rows
    if(col <= 2) exit = 7;
    else if(col <= 5) exit = 8;
    else exit = 9;
  }

  return exit;
}

//Given sector it returns its first row
int firstRowOfSector(int sector){

  int exit = 0;
  if(sector == 1 || sector == 2 || sector == 3) exit = 0;
  else if(sector == 4 || sector == 5 || sector == 6) exit = 3;
  else if(sector == 7 || sector == 8 || sector == 9) exit = 6;

  return exit;
}

//Given sector it returns its first col
int firstColOfSector(int sector){

  int exit = 0;
  if(sector == 1 || sector == 4 || sector == 7) exit = 0;
  else if(sector == 2 || sector == 5 || sector == 8) exit = 3;
  else if(sector == 3 || sector == 6 || sector == 9) exit = 6;

  return exit;
}
