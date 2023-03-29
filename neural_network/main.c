#include <stdio.h>
#include <stdlib.h>
#include "xor.h"
#include "matrix.h"
#include <time.h>

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

int main()
{

	struct neural_network network=NetworkInit();
	FILE *fichier=fopen("mnist_train.csv","r");
	float *csv=parse(fichier);
	while(csv[0]!=10)
	{
		TrainNeuralNetwork(csv,network);
		csv=parse(fichier);

	}
	free(csv);
	fclose(fichier);


	return 0;
}
