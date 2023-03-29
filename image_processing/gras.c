//gras.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"



void gras(SDL_Surface *image)
{
        Uint32 pixel;
	Uint8 r,g,b;
        int width = image->w;
        int height = image->h;
	SDL_Surface *image2 = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
        SDL_FillRect(image2, NULL, SDL_MapRGB(image2->format, 255,255, 255));
	for (int x=1;x<width-1;x++)
        {
                for (int y = 1;y<height-1;y++)
                {
                        pixel = getpixel(image,x,y);
                        SDL_GetRGB(pixel,image->format,&r,&g,&b);
		//	printf("%d",r);
		//	printf("\n");
                        if (r == 0)
			{
				for (int i = x-1;i<=x+1;i++)
				{
					for (int j = y-1;j<=y+1;j++)
					{
					
						pixel = SDL_MapRGB(image2->format,0,0,0);
        			        	putpixel(image2,i,j,pixel);
					}
				}

			}

		}
        }

	for (int x=0;x<width;x++)
        {
                for (int y=0;y<height;y++)
                {
                       pixel = getpixel(image2,x,y);
                       putpixel(image,x,y,pixel);

                }
        }
}
