//main.cpp, Guilherme Gomes
//Main file

#include "header.h"
#include <iostream>
#include <stdio.h>
using namespace std;

#define OUTPUT_FILENAME "output.csv" //any changes here must be replied in the makefile

int main(int argc, char** argv){

  cout << endl << "*********** SUDOKU SOLVER made by Guilherme Gomes ***********" << endl;

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
  Sudoku backupS;

  S.writeFromFile(input);
  S.printSudoku();

  int changes = 1;
  int count = -1;
  int maxGuesses = 1000;
  int guesses = 0;

  int guessCol = -1;
  int guessRow = -1;
  int guessValue = 0;

  //main program loop, do some standard checks first,
  //if we get stuck do some more advanced checks
  while(1){

    while(changes){
      changes = 0;
      count ++;
      changes += S.checkSectors();
      changes += S.checkRows();
      changes += S.checkCols();
      changes += S.checkCells();
    }

    if(S.amIValid()){
      if(S.solvedCells_ == 81) break;
      else{
        guesses++;
        if(guesses > maxGuesses) break;
        // cout << " That's a hard sudoku, applying brute force." << endl;
        //if we hit this point, it means that the standard check were not able to
        //fully solve the puzzle, so we are going to make a guess.

        if(guesses == 1) //if this is the first guess tell a message to the user
          cout << " That's a hard sudoku, we will have to make some guesses to work it out."
          << endl;

        backupS = S; //stores the valid sudoku in a backup variable

        int nP = 2; //number of candidates that a cell must have to be used to guess

        guessValue = 0;
        guessRow = guessCol = -1;

        for(int i = 8; i >= 0; i--){ //search for first blank cell
          for(int j = 8; j >= 0; j--){
            if( (S.cell[i][j].possibilities()) == nP ){
              guessRow = i;
              guessCol = j;
            }
            if((i == 0) && (j == 0) && (guessRow == -1) && (guessCol == -1)){
            //if this is the last iteration and we haven't found a cell with the given number of possibilities
              nP++;
              i = 8;
              j = 8;
            }
          }
        }
        guessValue = S.cell[guessRow][guessCol].onePossibleValue();
        S.setValue(guessValue, guessRow, guessCol);
        cout << " Guessing " << guessValue << " at (" << guessRow << ", " << guessCol << ")" << endl;
        changes++;
      }
    }
    if(!(S.amIValid())){
      backupS.cell[guessRow][guessCol].cannotBe(guessValue);
      //tells the cell that it cannot assume the guessed value
      S = backupS; //goes back to the backup version
    }
  }

  cout << endl << "  And this is your solved sudoku: " << endl;
  S.printSudoku();
  cout << "  We wrote this solution in a file named " << OUTPUT_FILENAME << endl;

  // S.showPossibilites();
  S.writeToFile(output);

  fclose(input);
  fclose(output);

  return 1;
} //search for empty cell
