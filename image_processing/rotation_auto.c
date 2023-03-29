

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pixel_operations.h"
#include "manual_rotation.h"





int number_of_line(SDL_Surface* image)
{
	int width = image->w;
        int height = image->h;
        Uint32 pixel;
        Uint8 r,g,b;
	int number_of_line = 0;


        //line_detection
	
 
	//line_detection
        int anotherline = 0;

        for (int y=0;y<height;y++)
        {

                int numberof1together = 0;
                int max1together = 0;
                for (int x = 0; x<width; x++)
                {

                        pixel = getpixel(image,x,y);
                        SDL_GetRGB(pixel,image->format, &r, &g, &b);

                        if (r == 0)
                        {
                                numberof1together+=1;
                        }
                        else
                        {
                                if (numberof1together > max1together)
                                {
                                        max1together = numberof1together;
				}
                                numberof1together = 0;
                        }
                }
                if (numberof1together > max1together)
                {
                        max1together = numberof1together;
		}
                if (max1together > width/3)
                {
                        if (anotherline != 1)
                        {
				anotherline = 1;
				number_of_line ++;
 			}
                }
                else
                        anotherline = 0;

        }
	return number_of_line;

}

int rotateauto(SDL_Surface* image)
{
    int line_max = 0;
    int angle = 0;

	
    for (int i = 0; i < 45; i++)
    {
        SDL_Surface* image2 = manual_rotation_with_redim(image, i);
   	int n_line =  number_of_line(image2);
	if (n_line >line_max)
	{
		line_max = n_line;
		angle = i;
	}
  
    
    }

    return angle;
	
}
