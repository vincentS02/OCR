//matrix.h
//----------------------------------------------------------------------------|
#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>
#include <stdlib.h>

// Structure of matrices
struct Matrix
{
        // Size of the matrix
        int rows;
        int columns;

        float *mat;
};


// Function change a value in a matrix
void editMatrix (struct Matrix matrix, int x, int y, float val);

// Function to move in the matrix; return matrix(x, y)
float moveInMatrix(struct Matrix matrix, int x, int y);

void initMatrix(struct Matrix matrix);

void initMatrix_zero(struct Matrix matrix);
// Create a matrix; return the matrix
struct Matrix createMatrix(int rows, int colums);

// Print the matrix
void printMatrix(struct Matrix matrix);
float Random();

#endif
