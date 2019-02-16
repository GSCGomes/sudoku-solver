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

//returns one possible value for that cell
int Cell::onePossibleValue(){

  int exit = 0;
  for(int i = 0; i < 9; i++){
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
    for(int i = firstRowOfSector(sec); i < (3 + firstRowOfSector(sec)); i ++){
      for(int j = firstColOfSector(sec); j < (3 + firstColOfSector(sec)); j ++){
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
        // cell[i][j].setValue(c - 48);
        setValue(c - 48, i, j);
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
  int set = 0; //0 -> reading, 1 -> writing, 2 -> advanced check
  int sectors[3] = {0,0,0};

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int row = 0; row < 9; row ++){
      for(int col = 0; col < 9; col ++){
        if(cell[row][col].canItBe(currentValue) && (!cell[row][col].value())){
          if(set == 0) n ++;
          //found one and it's currently reading the row
          else if(set == 1){
          //found one but we already read the row, we are just searching for this cell to write its value
            setValue(currentValue, row, col);
            set = 0;
            changes++;
            col = 9; //get out of the for loop
          }
          else if(set == 2){
          //found one but we already read the row, we are just checking if all the possibilities are in the same sector
            if(!sectors[0]) sectors[0] = sector(row,col);
            else if(!sectors[1]) sectors[1] = sector(row,col);
            else if(!sectors[2]) sectors[2] = sector(row,col);

          }
        }

        if(col == 8 && set == 2){ //if this is the last col iteration and we are doing advanced check (set = 2)
          if(!sectors[2]) sectors[2] = sectors[1];
          //if the sectors[2] equals 0, it means that there were only 2 possible places
          //so we write the value of sectors[1] on it to treate this as we would if there were 3 possible places
          if((sectors[0] == sectors[1]) && (sectors[1] == sectors[2])){
          //if all the possibilities are in the same sector
          //iterate throughout the sector and say to the cells out of the row that it cant be the current value
            for(int i = 0; i < 3; i++){
              for(int j = 0; j < 3; j++){
                if(row != (firstRowOfSector(sectors[0]) + i) //if this is not the current row
                && cell[firstRowOfSector(sectors[0]) + i][firstColOfSector(sectors[0]) + j].canItBe(currentValue)){
                  cell[firstRowOfSector(sectors[0]) + i][firstColOfSector(sectors[0]) + j].cannotBe(currentValue);
                  changes++;
                  cout << "done that fancy thing in the rows" << endl;
                }
              }
            }
          }
          set = 0;
          sectors[0] = sectors[1] = sectors[2] = 0;
        }
      }

      if(n == 1){ //if there's only one possible place for the current value on this row
        row --;
        set = 1;
      }
      else if(n == 2 || n == 3){  //if there are 2 or 3 possible places for the current value on this row
        row --;
        set = 2;
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
  int set = 0; //0 -> reading, 1 -> writing, 2 -> advanced check
  int sectors[3] = {0,0,0};

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int col = 0; col < 9; col ++){
      for(int row = 0; row < 9; row ++){
        if(cell[row][col].canItBe(currentValue) && !cell[row][col].value()){
          if(set == 0) n ++;
          //found one and it's currently reading the col
          else if(set == 1){
          //found one but we already read the col, we were just searching for this cell to write its value
            setValue(currentValue, row, col);
            set = 0;
            changes++;
            row = 9; //get out of the for loop
          }
          else if(set == 2){
          //found one but we already read the col, we are just checking if all the possibilities are in the same sector
            if(!sectors[0]) sectors[0] = sector(row,col);
            else if(!sectors[1]) sectors[1] = sector(row,col);
            else if(!sectors[2]) sectors[2] = sector(row,col);
          }
        }

        if(row == 8 && set == 2){ //if this is the last row iteration and we are doing advanced check (set = 2)
          if(!sectors[2]) sectors[2] = sectors[1];
          //if the sectors[2] equals 0, it means that there were only 2 possible places
          //so we write the value of sectors[1] on it to treate this as we would if there were 3 possible places
          if((sectors[0] == sectors[1]) && (sectors[1] == sectors[2])){
          //if all the possibilities are in the same sector
          //iterate throughout the sector and say to the cells out of the row that it cant be the current value
            for(int i = 0; i < 3; i++){
              for(int j = 0; j < 3; j++){
                if(col != (firstColOfSector(sectors[0]) + j) //if this is not the current col
                && cell[firstRowOfSector(sectors[0]) + i][firstColOfSector(sectors[0]) + j].canItBe(currentValue)){
                  cell[firstRowOfSector(sectors[0]) + i][firstColOfSector(sectors[0]) + j].cannotBe(currentValue);
                  changes++;
                  cout << "done that fancy thing in the cols" << endl;
                }
              }
            }
          }
          set = 0;
          sectors[0] = sectors[1] = sectors[2] = 0;
        }
      }

      if(n == 1){
        col --;
        set = 1;
      }
      else if(n == 2 || n == 3){  //if there are 2 or 3 possible places for the current value on this row
        col --;
        set = 2;
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
  int set = 0; //0 -> reading, 1 -> writing
  int rows[3] = {-1,-1,-1};
  int cols[3] = {-1,-1,-1};

  for(int currentValue = 1; currentValue <= 9; currentValue++){
    for(int sec = 1; sec <= 9; sec ++){
      for(int row = firstRowOfSector(sec); row < (3 + firstRowOfSector(sec)); row ++){
        for(int col = firstColOfSector(sec); col < (3 + firstColOfSector(sec)); col ++){
          if(cell[row][col].canItBe(currentValue) && !cell[row][col].value()){
            if(set == 0) n ++; //found one and it's currently reading the sector
            else if(set == 1){ //found one but we already read the sector, we were just searching for this cell to write its value
              setValue(currentValue, row, col);
              set = 0;
              changes++;
              row = col = 9; //get out of the for loop
            }
            else if(set == 2){
            //found one but we already read the sector, we are just checking if all the possibilities are in the same sector
              if(rows[0] < 0) rows[0] = row;
              else if(rows[1] < 0) rows[1] = row;
              else if(rows[2] < 0) rows[2] = row;

              if(cols[0] < 0) cols[0] = col;
              else if(cols[1] < 0) cols[1] = col;
              else if(cols[2] < 0) cols[2] = col;
            }
          }

          if((row == (2 + firstRowOfSector(sec))) && (col == (2 + firstColOfSector(sec))) && (set == 2)){
          //if this is the last sec iteration and we are doing advanced check (set = 2)
            if(rows[2] < 0) rows[2] = rows[1];
            //if the rows[2] equals 0, it means that there were only 2 possible places
            //so we write the value of rows[1] on it to treate this as we would if there were 3 possible places
            if(cols[2] < 0) cols[2] = cols[1];
            //if the rows[2] equals 0, it means that there were only 2 possible places
            //so we write the value of rows[1] on it to treate this as we would if there were 3 possible places
            if((rows[0] == rows[1]) && (rows[1] == rows[2])){
            //if all the possibilities are in the same rows
            //iterate throughout the row and say to the cells out of the sector that it cant be the current value
              for(int i = 0; i < 9; i++){
                if(sec != sector(rows[0], i) && cell[rows[0]][i].canItBe(currentValue)){
                //if this is not the current sector
                  cell[rows[0]][i].cannotBe(currentValue);
                  changes++;
                  cout << "done that fancy thing in the secs" << endl;
                }//end if
              }//end for
            }//end if

            if((cols[0] == cols[1]) && (cols[1] == cols[2])){
            //if all the possibilities are in the same cols
            //iterate throughout the row and say to the cells out of the sector that it cant be the current value
              for(int i = 0; i < 9; i++){
                if(sec != sector(i, cols[0]) //if this is not the current sector
                && cell[i][cols[0]].canItBe(currentValue)){
                  cell[i][cols[0]].cannotBe(currentValue);
                  changes++;
                }//end if
              }//end for
            }//end if
            set = 0;
            cols[0] = cols[1] = cols[2] = -1;
            rows[0] = rows[1] = rows[2] = -1;
          }//end if
        }//end for
      }//end for

      if(n == 1){
        sec --;
        set = 1;
      }
      else if(n == 2 || n == 3){  //if there are 2 or 3 possible places for the current value on this row
        sec --;
        set = 2;
      }

      n = 0;
    }
  }

  return changes;
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
