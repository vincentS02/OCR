#include <stdio.h>
#include <stdlib.h>
#include "../neural_network/xor.h"
#include "../neural_network/matrix.h"
#include "../image_processing/imgtolist.h"
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>

float *parse(FILE* pointeur)
{
	float *csv=calloc(785,sizeof(float));
	char c=fgetc(pointeur);
	if((int)c==EOF)
	{
		csv[0]=10;
	}
	else
	{
		while(c<'0' && c>'9')
			c=fgetc(pointeur);
		csv[0]=atoi(&c);
		fgetc(pointeur);
		for(int i=0;i<784;i++)
		{
			char car=fgetc(pointeur);
			while(car>='0'&& car<='9')
			{
				csv[i+1]=csv[i+1]*10+atoi(&car);
				car=fgetc(pointeur);

			}
			csv[i+1]=(csv[i+1]/255.0f*0.99f)+0.01f;

		}
	}
	return csv;
}

void tofile(char* directory,char* file,struct neural_network network)
{
	FILE *grid=fopen(file,"w+");
	char string[100];
	char c;
	float *parse;
	for(int i=1;i<82;i++)
	{
		sprintf(string,"%s%i.png",directory,i);
		parse=transform(IMG_Load(string));
		g_print(string);
		if(parse)
		{
			c=Query(parse,network);
		}
		else
			c='.';

		fputc(c,grid);
		if(i%9==0)
			fputc('\n',grid);
		else if(i%3==0)
			fputc(' ',grid);
		if(i%27==0)
			fputc('\n',grid);

	}
	free(parse);
	fclose(grid);
}