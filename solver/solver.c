#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver_sudoku.h"


int main(int argc, char *argv[])
{
   if (argc==2)
    {
     printf(argv[1]);
      FILE * FD = fopen(argv[1], "r");
      printSudoku(FD, argv[1]);

        return 0;
        
    }
}
