//manual_rotation.c

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "pixel_operations.h"
#define M_PI       3.14159265358979323846

SDL_Surface * manual_rotation (SDL_Surface *image, double angle_degree)
{
	double theta = (angle_degree*M_PI)/180;
		
		
	int width = image->w;
	int height = image->h;
	int x_center = width/2;
	int y_center = height/2;
	Uint8 r,g,b;
	Uint32 pixel;
	SDL_Surface *image2=SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
	
	for (int y=0;y<height;y++)
	{
		for (int x=0;x<width;x++)
		{
			
			int rot_x = ceil(((x - x_center) * cos(theta) -
                        (y - y_center) * sin(theta)) + x_center);
            int rot_y = ceil(((x - x_center) * sin(theta) +
                        (y - y_center) * cos(theta)) + y_center);

			r=g=b=255;
			pixel = SDL_MapRGB(image->format,r,g,b);
			if ((0<=rot_x && rot_x<width) && (rot_y>=0 && rot_y<=height))
			{
				pixel = getpixel(image,rot_x,rot_y);
			}
			
			putpixel(image2,x,y,pixel);
		}
	}

	return image2;
}


SDL_Surface * manual_rotation_with_redim (SDL_Surface *image, double angle_dg)
{
	double theta = (angle_dg*M_PI)/180;
	int width = image->w;
    int height = image->h;
    int new_x =0;
    int new_y = 0;
    int x_center = width/2;
    int y_center = height/2;

	if (fmod(theta, M_PI) < (M_PI / 2.))
    {
	    new_x = fabs(ceil(2*(((width-1)-x_center)*cos(theta)+
    	    (y_center)*sin(theta))));
        new_y = fabs(ceil(2 * (((width - 1) - x_center) * sin(theta) +
			((height - 1) - y_center) * cos(theta))));
    }
    else 
    {
        new_x = fabs(ceil(2 * (((width - 1)- x_center) * cos(theta) -
            ((height - 1) - y_center) * cos(theta))));
        new_y = fabs(ceil(2 * (((width - 1) - x_center) * sin(theta) -
            (y_center) * cos(theta))));
    }

  	int new_x_center = new_x / 2;
	int new_y_center = new_y / 2;
    Uint8 r,g,b;
    Uint32 pixel;
    SDL_Surface *image2=SDL_CreateRGBSurface(0,new_x,new_y,32,0,0,0,0);
    for (int y=0;y<new_y;y++)
    {
    	for (int x=0;x<new_x;x++)
        {
        	int rot_x = ceil(((x - new_x_center) * cos(theta) -
                (y - new_y_center) * sin(theta)) + x_center);
        	int rot_y = ceil(((x - new_x_center) * sin(theta) +
                (y - new_y_center) * cos(theta)) + y_center);
			r=g=b=255;
            pixel = SDL_MapRGB(image->format,r,g,b);
            if ((0<=rot_x && rot_x<width) && (rot_y>=0 && rot_y<=height))
            {
                pixel = getpixel(image,rot_x,rot_y);
            }
            putpixel(image2,x,y,pixel);
        }
   	}
    return image2;
}

