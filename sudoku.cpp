//sudoku.cpp, Guilherme Gomes
//Definition of the methods from the cell and sudoku classes

#include "header.h"
#include <iostream>
#include <stdio.h>
using namespace std;

/********************Cell methods**************************/
//Constructor
Cell::Cell(){
  value_ = 0;
  for(int i = 0; i < 9; i ++)
    hashCheck_[i] = TRUE; //initially any cell can have any value
}

//Destructor
Cell::~Cell(){}

//returns the value of the cell, 0 if the cell is empty
int Cell::value(){ return value_;};

//receives a value and tells whether that cell can assume that value given the hashCheck_ array
bool Cell::canItBe(int in_value){

  if(in_value >= 1 && in_value <= 9){
    //the input value must be between 1 and 9 but the index of the array must be between 0 and 8
    //so we have to decrement the value to convert it
    in_value --;
    return hashCheck_[in_value];
  }else return FALSE;
}

//returns the number of TRUE values in the hashCheck_ array
int Cell::possibilities(){

  int count = 0; //counting of TRUE values

  for(int i = 0; i < 9; i++)
    if(hashCheck_[i]) count++;

  return count;
}

//sets the value of the Cell
void Cell::setValue(int value){
  value_ = value;
  for(int i = 0; i < 9; i++)
    hashCheck_[i] = FALSE;
  hashCheck_[value - 1] = TRUE;
};

//tells to the cell that it can't be a certain value
void Cell::cannotBe(int value){
  //the input value must be between 1 and 9 but the index of the array must be between 0 and 8
  //so we have to decrement the value to convert it
  value --;
  hashCheck_[value] = FALSE;

}

//returns one possible value for that cell
int Cell::onePossibleValue(){

  int exit = 0;
  for(int i = 8; i >= 0; i--){
    if(hashCheck_[i]){
      exit = i+1;
    }
  }

  return exit;
}
/*****************End of Cell methods**********************/



/******************Sudoku methods**************************/
//Constructor
Sudoku::Sudoku(){
  solvedCells_ = 0;
}

//Destructor
Sudoku::~Sudoku(){}

//Sets the value of a given cell
void Sudoku::setValue(int in_value, int row, int col){

    if(in_value <= 0 || in_value > 9) in_value = 0;

    cell[row][col].setValue(in_value);
    int sec = sector(row, col);
    if(!sec){
      cout << "error position 1" << endl;
      return;
    }

    //modify hashCheck of cells on same row
    for(int i = 0; i < 9; i++){
      if(col != i) //if this is not the current cell
        cell[row][i].cannotBe(in_value); //tells the cell that it cannot be that value
    }

    //modify hashCheck of cells on same collumn
    for(int i = 0; i < 9; i++){
      if(row != i) //if this is not the current cell
        cell[i][col].cannotBe(in_value); //tells the cell that it cannot be that value
    }

    //modify hashCheck of cells on same sector
    for(int i = firstRowOfSector(sec); i < (3 + firstRowOfSector(sec)); i ++){
      for(int j = firstColOfSector(sec); j < (3 + firstColOfSector(sec)); j ++){
        if(i != row && j != col) //if this is not the current cell
          if(sec == sector(i,j)) //if this cell is in the same sector than the current
            cell[i][j].cannotBe(in_value);
      }
    }

    solvedCells_ ++;

}

//Fills the cells with data from file
void Sudoku::writeFromFile(FILE * file){

  char c = '.'; //anything different than EOF
  for(int i = 0; i < 9; i ++){
    for(int j = 0; j < 9; j++){
      if(c == EOF) break;
      c = fgetc(file);
      if(c >= 48 && c <= 57){ //if it's a number
          setValue(c - 48, i, j);
        c = fgetc(file);
      }
    }
  }

}

//Writes the file with finished sudoku
void Sudoku::writeToFile(FILE * file){

  for(int i = 0; i < 9; i ++){
    for(int j = 0; j < 9; j++){
      if(cell[i][j].value()) fprintf(file, "%d", cell[i][j].value());
      if(j < 8) fprintf(file, ",");
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
}

//Prints the whole sudoku on screen
void Sudoku::printSudoku(){

  for(int i = 0; i < 9; i++){
    cout << endl << "  ";
    for(int j = 0; j < 9; j++){
      if(cell[i][j].value()) cout << " " << cell[i][j].value() << " ";
      else cout << "   ";
      if(j == 2 || j == 5) cout << "|";
    }
    if(i == 2 || i == 5){
      cout << endl << "  ";
      for(int k = 0; k < 30; k++) cout << "-";
    }
  }

  cout << endl << endl;
}

//Checks cells' possibilities and writes the value in cells with only one possible value, returns number of changes
int Sudoku::checkCells(){

  int changes = 0;
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      if( cell[i][j].possibilities() == 1 && !cell[i][j].value()){
        setValue(cell[i][j].onePossibleValue(), i, j);
        changes++;
      }
    }
  }

  return changes;
}

//Checks rows and returns number of changes
int Sudoku::checkRows(){

  int n = 0; //how many cells of this row can assume this value? this variable stores the answer to this question
  int changes = 0;
  bool rw = 0; //0 -> reading, 1 -> writing

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int row = 0; row < 9; row ++){
      for(int col = 0; col < 9; col ++){
        if(cell[row][col].canItBe(currentValue) && (!cell[row][col].value())){
          if(rw == 0) n ++;
          //found one and it's currently reading the row
          else if(rw == 1){
          //found one but we already read the row, we are just searching for this cell to write its value
            setValue(currentValue, row, col);
            rw = 0;
            changes++;
            col = 9; //get out of the for loop
          }
        }
      }

      if(n == 1){ //if there's only one possible place for the current value on this row
        row --;
        rw = 1;
      }

      n = 0;
    }
  }

  return changes;
}

//Checks collumns and returns number of changes
int Sudoku::checkCols(){

  int n = 0; //how many cells of this col can assume this value? this variable stores the answer to this question
  int changes = 0;
  bool rw = 0; //0 -> reading, 1 -> writing

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int col = 0; col < 9; col ++){
      for(int row = 0; row < 9; row ++){
        if(cell[row][col].canItBe(currentValue) && !cell[row][col].value()){
          if(rw == 0) n ++;
          //found one and it's currently reading the col
          else if(rw == 1){
          //found one but we already read the col, we were just searching for this cell to write its value
            setValue(currentValue, row, col);
            rw = 0;
            changes++;
            row = 9; //get out of the for loop
          }
        }
      }

      if(n == 1){
        col --;
        rw = 1;
      }

      n = 0;
    }
  }

  return changes;
}

//Checks sectors and returns number of changes
int Sudoku::checkSectors(){

  int n = 0; //how many cells of this row can assume this value? this variable stores the answer to this question
  int changes = 0;
  bool rw = 0; //0 -> reading, 1 -> writing

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int sec = 1; sec <= 9; sec ++){
      for(int row = firstRowOfSector(sec); row < (3 + firstRowOfSector(sec)); row ++){
        for(int col = firstColOfSector(sec); col < (3 + firstColOfSector(sec)); col ++){
          if(cell[row][col].canItBe(currentValue) && !cell[row][col].value()){
            if(rw == 0) n ++; //found one and it's currently reading the sector
            else if(rw == 1){ //found one but we already read the sector, we were just searching for this cell to write its value
              setValue(currentValue, row, col);
              rw = 0;
              changes++;
              row = col = 9; //get out of the for loop
            }
          }

        }//end for
      }//end for

      if(n == 1){
        sec --;
        rw = 1;
      }

      n = 0;
    }
  }

  return changes;
}

//Checks if sudoku is valid in returns TRUE or FALSE
bool Sudoku::amIValid(){

  for(int i = 0; i < 9; i ++)
    for(int j = 0; j < 9; j ++){
      if( !(cell[i][j].possibilities()) )
        return FALSE;
    }

  return TRUE;
}

void Sudoku::showPossibilites(int in_value){

  cout << endl << "Possibilities for number: " << in_value << endl << endl;;
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      cout << " " << cell[i][j].canItBe(in_value) << " ";
      if(j == 2 || j == 5) cout << "|";
    }
    if(i == 2 || i == 5)
      cout << endl << "-----------------------------";
    cout << endl;
  }
  cout << endl;
}

void Sudoku::showPossibilites(){

  cout << endl << "Possibilities per cell" << endl << endl;;
  for(int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      cout << " " << cell[i][j].possibilities() << " ";
      if(j == 2 || j == 5) cout << "|";
    }
    if(i == 2 || i == 5)
      cout << endl << "-----------------------------";
    cout << endl;
  }
  cout << endl;
}
/***************End of Sudoku methods**********************/
