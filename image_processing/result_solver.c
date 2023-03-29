//result_solver.c
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <err.h>
#include <time.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "pixel_operations.h"
#include "filetostr.h"
#include "../neural_network/xor.h"
#include "../neural_network/matrix.h"
#include "../image_processing/imgtolist.h"
#include "../test/tofile.h"
#include "../solver/solver_sudoku.h"


void result_solver(SDL_Surface *image)
{
        Uint32 pixel;
	Uint8 r,g,b;
        int width = image->w;
        int height = image->h;
        int i = 0;
        
        //Loads the neural network
        struct neural_network network=NetworkInit();
        load(network, "neural_network/save.csv");
        
        //Creates the grid
        tofile("Result/","grid_scanned",network);
        
        //FILE* FD = fopen("grid_scanned", "r");
        //printSudoku(FD, "grid_scanned");
        
	for (int y=5;y<width;y+=44)
        {
                for (int x = 5 ; x<height; x+=44)
                {

                        //Faire résoudre le sudoku et créer fd le fichier contenant la solution
                        
                        FILE * fd = fopen("solver/grid_01.result", "r");
                        char str2[81];
                        
                        char* result_number = printArraystr(fd, str2);           
                       	if (result_number[i]!='\0')
                        {
                        	int number = ((int) result_number[i]) - 48;
		                pixel = getpixel(image,x,y);
		                SDL_GetRGB(pixel,image->format,&r,&g,&b);

				char path[20];
				sprintf(path,"bank_numbers/%d.png",number);
				SDL_Surface* image2 = IMG_Load(path); 
                        	for (int x_=0;x_<35;x_++)
                        	{
                        		for (int y_=0;y_<35;y_++)
		                	{
		                		pixel=getpixel(image2 , x_, y_);
		                     		putpixel(image,x+x_,y+y_,pixel);
		                	}
                        	}
                        	i++;
                        }	
		}
        }        
}
