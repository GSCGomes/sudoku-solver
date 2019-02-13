//solver.cpp, Guilherme Gomes
//This file contains some functions used to solve this problem


// #include "header.h"

//Receives coordinate and returns sector number, 0 for invalid coordinates
int sector(int row, int col){
//the sudoku grid has 9 3x3 sectors which will be
//labelled as follows:
//   1 | 2 | 3
//   4 | 5 | 6
//   7 | 8 | 9
//

  int exit = 0;

  if(!row || !col || row >= 9 || col >= 9) //if coordinates are invalid
    return 0;

  if(row <= 2){ //given coordinate is in the top three rows
    if(col <= 2) exit = 1;
    else if(col <= 5) exit = 2;
    else exit = 3;
  }
  else if(row <= 5){ //given coordinate is in the three middle rows
    if(col <= 2) exit = 4;
    else if(col <= 6) exit = 5;
    else exit = 6;
  }
  else{ //given coordinate is in the three bottom rows
    if(col <= 2) exit = 7;
    else if(col <= 6) exit = 8;
    else exit = 9;
  }

  return exit;
}
