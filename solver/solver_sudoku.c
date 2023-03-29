#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int file_to_array(FILE * fd,int arr[9][9])
{
  int i = 0;
  int j = 0;

  char c;
  while ((c = fgetc(fd)) != EOF)
  {
    if (i>=9)
      {
    i = 0;
    j++;
      }
    if (c == '.')
      {
    arr[j][i] = 0;
      }
    if (c == '\n')
      {
    continue;
      }
    if ( c == ' ')
      {
    continue;
      }
    else
      {
	arr[j][i] =  (int) c - 48;
      }
    i++;
  }
  return 0;
}

 

#define N 9
 

void print(int arr[0][0])
{
     for (int i = 0; i < N; i++)
      {
         for (int j = 0; j < N; j++)
            printf("%d ",arr[i][j]);
         printf("\n");
       }
}
 
int isSafe(int grid[9][9], int row,
                       int col, int num)
{
     
    
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
 
   
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;
 
   
    int startRow = row - row % 3,
                 startCol = col - col % 3;
   
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j +
                          startCol] == num)
                return 0;
 
    return 1;
}
 

int solveSudoku(int grid[9][9], int row, int col)
{
     

    if (row == 9 - 1 && col == 9)
        return 1;
 
  
    if (col == 9)
    {
        row++;
        col = 0;
    }
   
    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);
 
    for (int num = 1; num <= N; num++)
    {
         
        if (isSafe(grid, row, col, num)==1)
        {
            
            grid[row][col] = num;
           
           
            if (solveSudoku(grid, row, col + 1)==1)
                return 1;
        }
       
      
        grid[row][col] = 0;
    }
    return 0;
}

void printSudoku(FILE * FD, char * restrict B)
{
  FILE * fd = fopen(B, "r");
  int array[9][9];
  file_to_array(fd, array);
   if (solveSudoku(array,  0, 0)==1)
        {
	  char * name = strcat(B, ".result");
            FD = fopen(name, "w");
            if(FD == NULL)
            {
                printf("Unable to create file. \n");
                exit(EXIT_FAILURE);
            }
            for (int i= 0; i<9; i++)
            {
                for (int j = 0; j<3; j++)
                {
                    int c = array[i][j];
                    //fprintf(FD, "%d", c);
                }
                fputs (" ", FD);
                for (int j = 3; j< 6; j++)
                {
                    int c = array[i][j];
                    //fprintf(FD, "%d", c);
                }
                fputs (" ", FD);
                for (int j = 6; j< 9; j++)
                {
                    int c = array[i][j];
                    //fprintf(FD, "%d", c);
                }
                fputs ("\n", FD);
                if ((i == 2) | (i == 5)){
                    fputs("\n", FD);
                }
            }
            fclose(FD);
        }
        else{
            printf("no solution exists");
        }
}
  
