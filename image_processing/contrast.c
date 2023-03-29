#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"

 Uint8 f(Uint8 c, double n)
{
    if(c <= 255 / 2)
        return (Uint8)( (255/2) * pow((double) 2 * c / 255, n));
    else
        return 255 - f(255 - c, n);
}

void contrast(SDL_Surface *image, double n){
    Uint32 pixel;
    Uint8 r,g,b;
    for (int x =0; x< image->w;x++){
        for (int y=0;y<image->h;y++){
            pixel = getpixel(image,x,y);
            SDL_GetRGB(pixel,image->format, &r, &g, &b);
            
            r = f(r,n);
            g = f(g,n);
            b = f(b,n);
            pixel = SDL_MapRGB(image->format,r,g,b);
            putpixel(image,x,y,pixel);
        }
    }
}

void accentuation(SDL_Surface *image)
{
	Uint32 pixel;
	Uint8 r,g,b;
	int width = image->w;
	int height = image->h;

	for (int x=0;x<width;x++)
	{
		for (int y = 0;y<height;y++)
		{
			pixel = getpixel(image,x,y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);

			if (r<127)
			{
				r+=50; 
			}
			else
				r-=50;
			pixel = SDL_MapRGB(image->format,r,r,r);
			putpixel(image,x,y,pixel);
		}
	}


}
