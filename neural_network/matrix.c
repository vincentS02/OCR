//matrix.c
//----------------------------------------------------------------------------|
#include "matrix.h"

// Create a matrix; return the matrix
struct Matrix createMatrix(int rows, int columns)
{
	struct Matrix matrix;
       	
	matrix.rows = rows;
	matrix.columns = columns;

	matrix.mat = malloc(sizeof(float)*columns*rows);

	return matrix;
}


// Function to navigate in the matrix; return matrix(x, y)
float moveInMatrix(struct Matrix matrix, int x, int y)
{
	int i = x * matrix.columns + y;
	return *(matrix.mat + i);
}

// It changes a value in a matrix
void editMatrix(struct Matrix matrix, int x, int y, float val)
{
	*(matrix.mat + x * matrix.columns + y) = val;
}

// Print the matrix
void printMatrix(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
	{
		for(int j = 0; j < matrix.columns; j++)
		{
			printf("%f ", moveInMatrix(matrix, i, j));
		}
		printf("\n");
	}
}

// Init the matrix with zeros
void initMatrix(struct Matrix matrix)
{
    for (int i = 0; i < matrix.rows; i++)
    {
	for(int j = 0; j < matrix.columns; j++)
	{
		if(rand()%2==0)
		{
			editMatrix(matrix, i, j, Random());

		}
		else
		{
			editMatrix(matrix, i, j, -Random());


		}
	}
    }    
}

void initMatrix_zero(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
        {
                for(int j = 0; j < matrix.columns; j++)
                {
                        editMatrix(matrix, i, j, 0);
                }
        }

}

float Random()
{
        return (float)rand()/(float)RAND_MAX;
}


