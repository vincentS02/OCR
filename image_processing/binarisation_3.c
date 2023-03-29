#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>

int CLAMP(int x,int lo, int hi)
{
        if(x >= hi)
                return hi;
        if (x <= lo)
                return lo;
        return x;
}


void binarisation_3 (SDL_Surface *image)
{
	int width = image->w;
	int height = image->h;


//	int fenetre = 25;
//	int threshold = ((height/fenetre)+(width/fenetre))/2;
	int threshold = 60;
	printf("threshold : ");
	printf("%d",threshold);
	printf("\n");

	SDL_Surface *imagefinale = SDL_CreateRGBSurface (0,width,height,32,0,0,0,0);
	Uint8 r,g,b;

	int somme_couleur = 0;
	int somme_div = 0;

	int Ecart_type² = 0;

	for (int x=0;x<width;x++)
	{
		int borne_inf = CLAMP(x-threshold,0,width);
		int borne_sup = CLAMP(x+threshold,0,width);
		

		//traitement en y=<threshold;
		somme_couleur=0;
		somme_div=0;
		Ecart_type² = 0;

		//recherche moyenne de la fenetre
		for (int rep = 0;rep<threshold;rep++)
		{
			for (int inf = borne_inf;inf<borne_sup;inf++)
			{
				Uint32 pixel = getpixel(image,inf,rep);
                                SDL_GetRGB(pixel,image->format,&r,&g,&b);
				somme_couleur+=r;
				somme_div+=1;
				

			}
		}
		int moy = somme_couleur/somme_div;
		
		//recherche ecart type de la fenetre
		for (int rep = 0;rep<threshold;rep++)
		{
			for (int inf = borne_inf;inf<borne_sup;inf++)
			{
				Uint32 pixel = getpixel(image,inf,rep);
                                SDL_GetRGB(pixel,image->format,&r,&g,&b);
				Ecart_type² += ((r-moy)*(r-moy));
			}
		}



		Uint32 pixel = getpixel(image,x,0);
                SDL_GetRGB(pixel,image->format,&r,&g,&b);
		int ECART_TYPE = sqrt(Ecart_type²/somme_div);
		int seuil = moy*(1+(0.2*((ECART_TYPE/128)-1)));
		if (r>=seuil)
			r=255;
		else
			r=0;
		pixel = SDL_MapRGB(imagefinale->format,r,r,r);
                putpixel(imagefinale,x,0,pixel);
		


		//parcours pour un y normal 
		for (int y=1;y<height;y++)
		{
			//calcul de la moyenne

			
			if (y-threshold >0)
			{
				for (int inf = borne_inf;inf<borne_sup;inf++)
				{
					Uint32 pixel = getpixel(image,inf,y-threshold-1);
                	                SDL_GetRGB(pixel,image->format,&r,&g,&b);
					somme_couleur-=r;
					somme_div-=1;
				}
			}

			if (y+threshold <height)
			{
				for (int inf = borne_inf;inf<borne_sup;inf++)
				{
					Uint32 pixel = getpixel(image,inf,y+threshold+1);
                	                SDL_GetRGB(pixel,image->format,&r,&g,&b);
					somme_couleur+=r;
					somme_div+=1;
				}


			}
			
			int moy = somme_couleur/somme_div;
			
			if (y-threshold >0)
			{
				for (int inf = borne_inf;inf<borne_sup;inf++)
				{
					Uint32 pixel = getpixel(image,inf,y-threshold-1);
                	                SDL_GetRGB(pixel,image->format,&r,&g,&b);
					Ecart_type² -= ((r-moy)*(r-moy));


				}
			}

			if (y+threshold <height)
			{
				for (int inf = borne_inf;inf<borne_sup;inf++)
				{
					Uint32 pixel = getpixel(image,inf,y+threshold+1);
                	                SDL_GetRGB(pixel,image->format,&r,&g,&b);
					Ecart_type² += ((r-moy)*(r-moy));
				}


			}


			Uint32 pixel = getpixel(image,x,y);
        	        SDL_GetRGB(pixel,image->format,&r,&g,&b);
//			int ancien = r;
			int ECART_TYPE = sqrt(Ecart_type²/somme_div);
			int seuil = moy*(1+(0.2*((ECART_TYPE/128)-1)));

			if (r>=seuil)
				r=255;
			else
				r=0;
			pixel = SDL_MapRGB(imagefinale->format,r,r,r);
                	putpixel(imagefinale,x,y,pixel);			
/*
			printf("\n");
			printf ("somme_couleur : ");
                       printf("%d", somme_couleur);
                       printf("\n");
                       printf("somme_diviseur: ");
                       printf("%d",somme_div);
	       	       printf ("\n");
                       printf("moy : ");
                       printf("%d",moy);
                       printf("\n");
                       printf("Ecart_type : ");
                       printf("%d",ECART_TYPE);
                       printf("\n");
                       printf("SEUIL : ");
                       printf("%d",seuil);
                       printf("\n");
			printf("valeur du pixel : ");
			printf("%d",x);
			printf(" ");
			printf("%d",y);
			printf(" ");
                       printf("%d",ancien);
                       printf("\n");

*/


		}

//	printf("____________________________________________________________");	

	}
	for (int x=0;x<width;x++)
        {
                for (int y=0;y<height;y++)
                {
                        Uint32 pixel = getpixel(imagefinale,x,y);
                        putpixel(image,x,y,pixel);

                }
        }
}

