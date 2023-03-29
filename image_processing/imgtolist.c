#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"
#include <gtk/gtk.h>

float* transform(SDL_Surface *image)
{
	
	Uint32 pixel;
	Uint8 r,g,b;
	int width = image->w;
	int height = image->h;
    	float* result =  calloc(784, sizeof(float) );
    	
	int nb=0;
    	int i=0;
	for(int x = 0; x <width ; x++)
    	{
		for(int y = 0; y < height; y++)
		{
		
				pixel = getpixel(image,x,y);
				SDL_GetRGB(pixel,image->format, &r, &g, &b);
				
				if(r==255)
				{
					result[i]=0.01f;
					nb+=1;
				}
				else if (r==0)
				{
					result[i]=0.99f;
				}
				i++;
		}
	}
	if(nb>740)
		return NULL;
	return result;
}

SDL_Surface* resize (SDL_Surface *image)
{
	SDL_Surface *new=NULL;
	Uint32 p;
	Uint8 r,g,blue;

	new=SDL_CreateRGBSurface(SDL_SWSURFACE,28,28,32,0,0,0,0);
	int n=image->h/28;
	for(int a=0;a<image->w;a+=n)
	{
		for(int b=0;b<image->h;b+=n)
		{
			int average=0;
			for(int x=0;x<n;x++)
			{
				for(int y=0;y<n;y++)
				{
					p=getpixel(image,a+x,b+y);
					SDL_GetRGB(p,image->format,&r , &g , &blue);
					average+=r;
				}


			}
			average/=n*n;
			putpixel(new,a/n,b/n,SDL_MapRGB(new->format,average,average,average));

		}
			
	}
	return new;

}
