//main.cpp, Guilherme Gomes
//Main file

// *************************Really short explanation of the logic used here*****************************
//
//   Do some standard checks, if we get stuck, guess a value in a cell with a short number
//   of candidates and go back to the standard checks. Do this until we solve the sudoku.
//
// *************************More detailed explanation of the logic used here****************************
//
//   To solve the sudoku, we first read the given file and write everything
//   in a Sudoku class which contains a 9 by 9 matrix of Cells which is another
//   class. Every Cell stores its value and a bool array, this array is responsible
//   to tell what are the candidates for each cell (example: if array[1] = TRUE
//   than number 1 is a valid candidate, if array[5] = FALSE than number 5 is not
//   a valid candidate for that cell).
//
//   We do 4 types of scan:
//    - cell scan: search for cells with only one valid candidate and write its value
//    - row scan : iterate row by row checking if there is any value that is a valid candidate
//                 in only one cell of the row and write its value
//    - col scan : iterate collumn by collumn checking if there is any value that is a valid
//                 candidate in only one cell of the collumn and write its value
//    - sec scan : iterate sector by sector checking if there is any value that is a valid
//                 candidate in only one cell of the sector and write its value
//
//   If we did all of the scans multiple times and reached a spot where they were causing
//   no change in the sudoku, than it's time for guessing. To make a better choice for the
//   guess, we iterate through the grid and search for a cell with the minimum number of
//   candidates. If the choice leads to an invalid sudoku grid, we go back to the last backup and try again.
//
// *****************************************************************************************************

#include "header.h"

#define OUTPUT_FILENAME "output.csv" //any changes here must be replied in the makefile

void solve(Sudoku & S);

int main(int argc, char** argv){
/*{{{*/
  FILE * input;
  if(! (input = fopen(argv[1], "r") )){
    std::cout << "Error openning sudoku file. Exiting." << std::endl;
    return -1;
  }

  FILE * output;
  if(! (output = fopen(OUTPUT_FILENAME, "w") )){
    std::cout << "Error creating output file. Exiting." << std::endl;
    return -1;
  }

  Sudoku S;
  S.writeFromFile(input);

  solve(S);

  S.writeToFile(output);

  fclose(input);
  fclose(output);

  return 0;/*}}}*/
}

void solve(Sudoku & S){
/*{{{*/

  Sudoku backup[MAX_DEPTH];
  Guess guess[MAX_DEPTH];

  int depth = 0;
  int changes = 1;
  int sweeps = 0;

  std::cout << std::endl << "*********** SUDOKU SOLVER made by Guilherme Gomes ***********" << std::endl
            << std::endl << " This is the sudoku puzzle you entered: " << std::endl;

  S.printSudoku();

  //main program loop, do some standard checks first,
  //if we get stuck do some more advanced checks
  while(changes){

    while(changes){ //standard checks
      changes = 0;
      changes += S.checkSectors();
      changes += S.checkRows();
      changes += S.checkCols();
      changes += S.checkCells();
      sweeps++;
    }

    //if we hit this point, it means that the standard checks were causing no changes

    if(S.amIValid()){
      if(S.solvedCells_ == 81) break; //the sudoku is solved
      else{

        //if we hit this point, it means that the standard check were not able to
        //fully solve the puzzle, so we are going to make a guess.

        backup[depth] = S; //stores the valid sudoku in a backup variable

        int nP = 1; //number of possibilities that a cell must have to be used to guess

        guess[depth].value = 0;
        guess[depth].row = guess[depth].col = -1;

        for(int i = 8; i >= 0; i--){ //search for first blank cell
          for(int j = 8; j >= 0; j--){
            if( !(S.cell[i][j].value()) && (S.cell[i][j].possibilities()) == nP ){
              guess[depth].row = i;
              guess[depth].col = j;
            }
            if((i == 0) && (j == 0) && (guess[depth].row == -1) && (guess[depth].col == -1)){
            //if this is the last iteration and we haven't found a cell with the given number of possibilities
              nP++; //increase number of possibilities
              i = j = 9;
              break;
              //go back to the start
            }
          }
        }

        guess[depth].value = S.cell[guess[depth].row][guess[depth].col].onePossibleValue();
        S.setValue(guess[depth].value, guess[depth].row, guess[depth].col);
        changes++;
      }
      depth++;
      if(depth > MAX_DEPTH){
        std::cout << " Max depth was reached, this is a really hard sudoku, exiting." << std::endl << std::endl;
        exit(1);
      }
    }
    else if(!(S.amIValid())){
      if(!depth){ //the input is not valid
        std::cout << std::endl << std::endl << " This sudoku puzzle is not valid. Exiting." << std::endl << std::endl;
        exit(1);
      } else{
        depth--;
        backup[depth].cell[guess[depth].row][guess[depth].col].cannotBe(guess[depth].value);
        //tells the cell that it cannot assume the guessed value
        S = backup[depth]; //goes back to the backup version
        if(!(S.amIValid())){ //if this backup version is also invalid, goes back to the backup version before this one
          depth--;
          backup[depth].cell[guess[depth].row][guess[depth].col].cannotBe(guess[depth].value);
          S = backup[depth];
        }
        changes++;
      }
    }
  }

  std::cout << std::endl << " And this is your solved sudoku: " << std::endl;
  S.printSudoku();

  std::cout << std::endl << " The number of sweeps through the grid was " << sweeps << "." << std::endl;
  if(depth) std::cout << " The depth of the guessing tree was " << depth << "." << std::endl;
  else std::cout << " No harder than average, guessing was not necessary to solve this." <<  std::endl;

  std::cout << std::endl << " We wrote this solution in a file named " << OUTPUT_FILENAME << std::endl << std::endl;;

/*}}}*/
}
