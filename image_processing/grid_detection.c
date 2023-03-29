//grid_detection.c

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"
#include<stdio.h>
#include<string.h>


void resize_surface(SDL_Surface* src,SDL_Surface* dest)
{
    int i,j;
    double rx,ry;
    rx = dest->w*1.0/src->w;
    ry = dest->h*1.0/src->h;
    for(i=0;i<dest->w;i++)
        for(j=0;j<dest->h;j++)
        {
                Uint32 pix;
                pix = getpixel(src,(int)(i/rx),(int)(j/ry));
                putpixel(dest,i,j,pix);
        }
}

int find_right(SDL_Surface *image,int x,int y)
{
	Uint32 pixel;
	Uint8 r,g,b;
	int largeur = 1;
	pixel = getpixel(image,x,y);
        SDL_GetRGB(pixel,image->format, &r, &g, &b);
	while (r!=255||g!=0)
	{
		largeur++;
		x++;
		pixel = getpixel(image,x,y);
		SDL_GetRGB(pixel,image->format, &r, &g, &b);

	}
	return largeur;
}

int is_column(SDL_Surface *image, int y)
{
	Uint32 pixel;
	Uint8 r,g,b;
	int width = image->w;
	for (int x=0;x<width;x++)
	{
		pixel = getpixel(image,x,y);
		SDL_GetRGB(pixel,image->format, &r, &g, &b);
		if (r == 255 && g==0)
			return 1;
	}
	return 0;

}	

int find_down(SDL_Surface *image,int x,int y)
{
	Uint32 pixel;
	Uint8 r,g,b;
	int hauteur = 1;
	pixel = getpixel(image,x,y);
        SDL_GetRGB(pixel,image->format, &r, &g, &b);
	while (r!=255||g!=0)
	{
		hauteur++;
		y++;
		pixel = getpixel(image,x,y);
		SDL_GetRGB(pixel,image->format, &r, &g, &b);

	}
	return hauteur;
}


void case_detection_4_4(SDL_Surface *image)
{
	
	
	int width = image->w;
	int height = image->h;
	Uint32 pixel;
	Uint8 r,g,b;
	int top_x = 0;
	int top_y = 0;

	

	for (int x=0;x<width;x++)
	{
		for (int y = 0;y<height;y++)
		{
			pixel = getpixel(image,x,y);
            		SDL_GetRGB(pixel,image->format, &r, &g, &b);
			if (r==255 && g == 0)
			{
				top_x = x;
				top_y = y;
				//sortie de boucle dégueu mais flemme
				x = width;
				y = height;
			}
		}
	}

	//recherche du premier carré, genre le point blanc en haut à gauche quoi
	
	while (r==255 && g==0)
	{
		top_x++;
		top_y++;
		pixel = getpixel(image,top_x,top_y);
		SDL_GetRGB(pixel,image->format, &r, &g, &b);
	}
	pixel = getpixel(image,top_x-1,top_y);
	SDL_GetRGB(pixel,image->format,&r,&g,&b);
	while (r!=255 || g!=0)
	{
		top_x-=1;
		pixel = getpixel(image,top_x-1,top_y);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
	}

	pixel = getpixel(image,top_x,top_y-1);
	SDL_GetRGB(pixel,image->format,&r,&g,&b);
	while (r!=255 || g!=0)
	{
		top_y-=1;
		pixel = getpixel(image,top_x,top_y-1);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
	}

	int index = 1;
	int parcoursX = 1;
	int parcoursY = 1;

	int largeur_case = find_right(image,top_x,top_y)/3;
	int hauteur_case = find_down(image,top_x,top_y)/3;
	int temp_y = 0;
	while (parcoursY <= 9)
	{
		int temp_x = 0;
		parcoursX = 1;
		while(parcoursX <=9)	
		{
			SDL_Surface *cell=SDL_CreateRGBSurface(0,largeur_case,hauteur_case,32,0,0,0,0);
			for (int C_Y=0;C_Y<largeur_case-1;C_Y++)
			{	
				for (int C_X = 0;C_X<hauteur_case-1;C_X++)
				{
					pixel = getpixel(image,temp_x+C_X+top_x,temp_y+top_y+C_Y);
					putpixel(cell,C_X,C_Y,pixel);
				}
			}
			temp_x +=largeur_case;
		

			char text[20] = "Result/";
			char b[22];
			sprintf(b,"%d",index);	
			strcat(text,b);						
			strcat(text,".png");						
			IMG_SavePNG(cell,text);
			parcoursX++;
	//		
			index++;


		}
		temp_y +=hauteur_case;
		parcoursY++;

	}







	//int modx = 1;
	//int mody = 1;
	/*
	while (parcoursY <=9)
	{
		int copy_top_x = top_x;
		parcoursX = 1;
		//mody = parcoursY%3;
		int hauteur = find_down(image,copy_top_x,top_y)/3;
		int largeur = find_right(image,copy_top_x,top_y)/3;

		while (parcoursX <=9)
		{
		//	int largeur = find_right(image,copy_top_x,top_y)/3;
		//	int hauteur = find_down(image,copy_top_x,top_y)/3;
	
			SDL_Surface *cell=SDL_CreateRGBSurface(0,largeur,hauteur,32,0,0,0,0);	
			
			for (int C_X=0;C_X<largeur-1;C_X++)
			{	
				for (int C_Y = 0;C_Y<hauteur-1;C_Y++)
				{
					pixel = getpixel(image,top_x+C_X,top_y+C_Y);
					putpixel(cell,C_X,C_Y,pixel);
				}
			}
			
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
			while (r!=255 || g!=0)
			{
				top_x+=1;
				pixel = getpixel(image,top_x,top_y);
				SDL_GetRGB(pixel,image->format,&r,&g,&b);
			}

			while (r==255 && g==0)
			{
				top_x+=1;
				pixel = getpixel(image,top_x,top_y);
				SDL_GetRGB(pixel,image->format,&r,&g,&b);
			}
			top_x +=largeur;
			

			char text[20] = "Result/";
			char b[22];
			sprintf(b,"%d",index);	
			strcat(text,b);						
			strcat(text,".png");						
			IMG_SavePNG(cell,text);
			parcoursX++;
	//		modx =parcoursX%3;
			index++;
		}
		pixel = getpixel(image,top_x,top_y);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
		while (r!=255 || g!=0)
		{
			top_y+=1;
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
		}
		while (r==255 && g==0)
		{
			top_y+=1;
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
		}
		top_y += hauteur;

		parcoursY++;
	}*/




}









void case_detection_10_10(SDL_Surface *image)
{
	
	
	int width = image->w;
	int height = image->h;
	Uint32 pixel;
	Uint8 r,g,b;
	int top_x = 0;
	int top_y = 0;

	

	for (int x=0;x<width;x++)
	{
		for (int y = 0;y<height;y++)
		{
			pixel = getpixel(image,x,y);
            		SDL_GetRGB(pixel,image->format, &r, &g, &b);
			if (r==255 && g == 0)
			{
				top_x = x;
				top_y = y;
				//sortie de boucle dégueu mais flemme
				x = width;
				y = height;
			}
		}
	}

	//recherche du premier carré, genre le point blanc en haut à gauche quoi
	
	while (r==255 && g==0)
	{
		top_x++;
		top_y++;
		pixel = getpixel(image,top_x,top_y);
		SDL_GetRGB(pixel,image->format, &r, &g, &b);
	}
	pixel = getpixel(image,top_x-1,top_y);
	SDL_GetRGB(pixel,image->format,&r,&g,&b);
	while (r!=255 || g!=0)
	{
		top_x-=1;
		pixel = getpixel(image,top_x-1,top_y);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
	}

	pixel = getpixel(image,top_x,top_y-1);
	SDL_GetRGB(pixel,image->format,&r,&g,&b);
	while (r!=255 || g!=0)
	{
		top_y-=1;
		pixel = getpixel(image,top_x,top_y-1);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
	}

	int index = 1;
	int parcoursX = 1;
	int parcoursY = 1;
	while (parcoursY <=9)
	{
		int copy_top_x = top_x;
		parcoursX = 1;
		while (parcoursX <=9)
		{
			int largeur = find_right(image,copy_top_x,top_y);
			int hauteur = find_down(image,copy_top_x,top_y);
	
			SDL_Surface *cell=SDL_CreateRGBSurface(0,largeur,hauteur,32,0,0,0,0);	
			
			for (int C_X=0;C_X<largeur-1;C_X++)
			{	
				for (int C_Y = 0;C_Y<hauteur-1;C_Y++)
				{
					pixel = getpixel(image,top_x+C_X,top_y+C_Y);
					putpixel(cell,C_X,C_Y,pixel);
				}
			}
			
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
			while (r!=255 || g!=0)
			{
				top_x+=1;
				pixel = getpixel(image,top_x,top_y);
				SDL_GetRGB(pixel,image->format,&r,&g,&b);
			}

			while (r==255 && g==0)
			{
				top_x+=1;
				pixel = getpixel(image,top_x,top_y);
				SDL_GetRGB(pixel,image->format,&r,&g,&b);
			}

			char text[20] = "Result/";
			char b[22];
			sprintf(b,"%d",index);	
			strcat(text,b);						
			strcat(text,".png");
			
			SDL_Surface* resized = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
			resize_surface(cell,resized);

			IMG_SavePNG(resized,text);
			parcoursX++;
			index++;
		}
		pixel = getpixel(image,top_x,top_y);
		SDL_GetRGB(pixel,image->format,&r,&g,&b);
		while (r!=255 || g!=0)
		{
			top_y+=1;
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
		}
		while (r==255 && g==0)
		{
			top_y+=1;
			pixel = getpixel(image,top_x,top_y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
		}


		parcoursY++;
	}




}


void grid_detection(SDL_Surface *image)
{
	int width = image->w;
	int height = image->h;
	Uint32 pixel;
	Uint8 r,g,b;
	int number_of_line = 0;
	int number_of_column = 0;

	int write_x_min = 0;
        int write_y_min = 0;
        int write_x_max = 0;
        int write_y_max = 0;

	//line_detection
	int anotherline = 0;
	
	for (int y=0;y<height;y++)
        {
                int numberof1together = 0;
                int max1together = 0;
		int begin = 0;
		int max_begin = 0;
		int max_end = 0;

                for (int x=0;x<width;x++)
                {
			pixel = getpixel(image,x,y);
			SDL_GetRGB(pixel,image->format,&r,&g,&b);
                        if (r==0)
                                numberof1together+=1;
                        else
                        {
                                if (numberof1together >max1together)
                                {
                                        max1together = numberof1together;
                                        max_begin = begin;
                                        max_end = x;
                                }
                                numberof1together = 0;
                                begin = x;
                        }
                }
                if (numberof1together >max1together)
                {
                        max1together = numberof1together;
                        max_begin = begin;
                        max_end = width-1;
                }

                if (max1together >width/3)
                {
                        if (anotherline == 0)
                        {
                                write_y_min = y;
                                write_y_max = y;
                                write_x_min = max_begin;
                                write_x_max = max_end;
                                anotherline = 1;
                        }
                        else
                        {
                                write_y_max = y;
                                if (max_begin <write_x_min)
                                        write_x_min = max_begin;
                                if (max_end > write_x_max)
                                        write_x_max = max_end;
				
                        }

                }
                else
                {
                        if (anotherline == 1)
                        {
				if (write_y_min >=1)
					write_y_min -=1;
			       	if (write_y_max <height-1)
					write_y_max ++;	

				for (;write_y_min<=write_y_max;write_y_min++)
                                {
                                        for (int min = write_x_min;min<=write_x_max;min++)
                                        {
                                                r = 255;
                                                g = 0;
                                                b = 0;
                                                pixel = SDL_MapRGB(image->format,r,g,b);
                                                putpixel(image,min,write_y_min,pixel);


                                        }

                                }
				number_of_line++;

				
                        }
			else
			{
				
				write_x_max = 0;
				write_x_min = 0;
				write_y_min = 0;
				write_y_max = 0;

			}

                        anotherline = 0;
                }
	//	printf("boucle");


        }

			if (anotherline == 1)
                        {
				if (write_y_min >=1)
					write_y_min -=1;
			       	if (write_y_max <height-1)
					write_y_max ++;	

				for (;write_y_min<=write_y_max;write_y_min++)
                                {
                                        for (int min = write_x_min;min<=write_x_max;min++)
                                        {
                                                r = 255;
                                                g = 0;
                                                b = 0;
                                                pixel = SDL_MapRGB(image->format,r,g,b);
                                                putpixel(image,min,write_y_min,pixel);


                                        }

                                }
				number_of_line++;

				
                        }



	//column_detection

	int anotherone = 0;//bool an other line of pixel is the same line
	write_x_min = 0;
	write_x_max = 0;
	write_y_min = 0;
	write_y_max = 0;
	
			
	for (int x=0;x<width;x++)
    	{
        	int numberof1together = 0;
        	int max1together = 0;
		int begin = 0;
		int max_begin = 0;
		int max_end = 0;

		for (int y = 0; y<height; y++)
        	{

            		pixel = getpixel(image,x,y);
            		SDL_GetRGB(pixel,image->format, &r, &g, &b);
 
 			if (r == 0 || (r==255 && g==0))
 			{
				numberof1together+=1;
			}
			else
			{
				if (numberof1together > max1together)
				{
					max1together = numberof1together;
					max_begin = begin;
					max_end = y;
				}
				begin = y;

				numberof1together = 0;
			}
		}
             
		if (numberof1together > max1together)
		{
			max1together = numberof1together;
			max_begin = begin;
			max_end = height-1;
		}
	
		if (max1together >height/3)
		{

			if (anotherone == 0)
			{
				write_x_min = x;
				write_x_max = x;
				write_y_min = max_begin;
				write_y_max = max_end;
				anotherone = 1;				
			}
			else
			{
				write_x_max = x;
				if (max_begin <write_y_min)
					write_y_min = max_begin;
				if (max_end >write_y_max)
					write_y_max = max_end;
			}
		}
		else
		{
			if (anotherone == 1)
			{
				if (write_x_min >=1)
					write_x_min -=1;
			       	if (write_x_max <width-1)
					write_x_max ++;	

				for (;write_x_min<=write_x_max;write_x_min++)
                                {
                                        for (int min = write_y_min;min<=write_y_max;min++)
                                        {
                                                r = 255;
                                                g = 0;
                                                b = 0;
                                                pixel = SDL_MapRGB(image->format,r,g,b);
                                                putpixel(image,write_x_min,min,pixel);


                                        }

                                }
                                number_of_column++;
			}
			else
			{
				write_x_max = 0;
                                write_x_min = 0;
                                write_y_min = 0;
                                write_y_max = 0;
			}
			
			anotherone = 0;
		}


    	}

			if (anotherone == 1)
			{
				if (write_x_min >=1)
					write_x_min -=1;
			       	if (write_x_max <width-1)
					write_x_max ++;	
				for (;write_x_min<=write_x_max;write_x_min++)
                                {
                                        for (int min = write_y_min;min<=write_y_max;min++)
                                        {
                                                r = 255;
                                                g = 0;
                                                b = 0;
                                                pixel = SDL_MapRGB(image->format,r,g,b);
                                                putpixel(image,write_x_min,min,pixel);


                                        }

                                }
                                number_of_column++;
			}

	
	printf("number of line : ");
	printf("%d",number_of_line);
	printf("\n");

	printf("number of column : ");
	printf("%d",number_of_column);
	printf("\n");
/*
	if ((number_of_line == 10) && (number_of_column == 10))
		case_detection(image);
	else
	{*/
				
		for (int y=0;y<height;y++)
		{
			for (int x = 0;x<width;x++)
			{
				pixel = getpixel(image,x,y);
        	    		SDL_GetRGB(pixel,image->format, &r, &g, &b);
				if (r==255 && g == 0)
				{
					int tempx = x;
					int tempy = y;
					while (r==255 && g==0)
					{
						tempx++;
						tempy++;
						pixel = getpixel(image,tempx,tempy);
        	    				SDL_GetRGB(pixel,image->format, &r, &g, &b);
					}
										
					if (is_column(image,tempy) == 0)
					{
						number_of_line-=1;
						for (int y__ = y;y__<=tempy;y__++)
						{
							int x__ = x;
							pixel = getpixel(image,x__,y__);
								SDL_GetRGB(pixel,image->format, &r, &g, &b);

							while (r == 255 && g ==0)
							{
								pixel = SDL_MapRGB(image->format, 0,0,0);
								putpixel(image,x__,y__,pixel);
								x__++;
								pixel = getpixel(image,x__,y__);
								SDL_GetRGB(pixel,image->format, &r, &g, &b);

							}
						
						}
						y = tempy;
					}
					else
					{
						x = width;
						y = height;
					}
				}
			}
		}
		
		printf("number of line : ");
		printf("%d",number_of_line);
		printf("\n");

		printf("number of column : ");
		printf("%d",number_of_column);
		printf("\n");

		if ((number_of_line == 10) && (number_of_column == 10))
			case_detection_10_10(image);
		if ((number_of_line == 4) && (number_of_column == 4))
			case_detection_4_4(image);




	//}

}



