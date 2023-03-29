#ifndef SOLVER_SUDOKU_H_
#define SOLVER_SUDOKU_H_

#include<stdlib.h>
#include<stdio.h>

int file_to_array(FILE * fd,int arr[9][9]);
void print(int arr[0][0]);
int isSafe(int grid [9][9], int row, int col, int num);
int solveSudoku(int grid[9][9], int row, int col);
void printSudoku(FILE * FD, char * restrict);

#endif
