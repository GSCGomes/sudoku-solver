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
int Cell::value(){ return value_; };

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
/*****************End of Cell methods**********************/



/******************Sudoku methods**************************/
//Constructor
Sudoku::Sudoku(){
  state_ = FALSE;
}

//Destructor
Sudoku::~Sudoku(){}

//Receives coordinates and returns value, 0 for an empty cell
int Sudoku::value(int row, int col){
  return  cell[row][col].value();
}

//Returns TRUE if the program can't find anything else to do
bool Sudoku::state(){ return state_; }

//Sets the value of a given cell
void Sudoku::setValue(int in_value, int row, int col){

    cell[row][col].setValue(in_value);
    int sec = sector(row, col);

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
    for(int i = (row - 2); i <= (row + 2); i ++){
      for(int j = (col - 2); j <= (col + 2); j ++){
        if(i != row && j != col) //if this is not the current cell
          if(sec == sector(i,j)) //if this cell is in the same sector than the current
            cell[i][j].cannotBe(in_value);
      }
    }

}

//Sets sudoku state
void Sudoku::setState(bool in_state){ state_ = in_state; }

//Fills the cells with data from file
void Sudoku::writeFromFile(FILE * file){

  char c = '.'; //anything different than EOF
  for(int i = 0; i < 9; i ++){
    for(int j = 0; j < 9; j++){
      if(c == EOF) break;
      c = fgetc(file);
      if(c >= 48 && c <= 57){ //if it's a number
        cell[i][j].setValue(c - 48);
        c = fgetc(file);
      }
    }
  }

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

/***************End of Sudoku methods**********************/
