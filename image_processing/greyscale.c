#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"

void greyscale(SDL_Surface *image)
{
	Uint32 pixel;
	Uint8 r,g,b;
	Uint8 average;
	
	int width = image->w;
	int height = image->h;
    
	for(int x = 0; x <width ; x++)
    {
        for(int y = 0; y < height; y++)
        {
			pixel = getpixel(image,x,y);
			SDL_GetRGB(pixel,image->format, &r, &g, &b);

			average = 0.3*r+0.59*g+0.11*b;
			r=g=b=average;

			pixel = SDL_MapRGB(image->format,r,g,b);
			putpixel(image,x,y,pixel);
		}
    }
}

