#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char file_to_arraystr(FILE * fd,char arr[9][9])
{
  int i = 0;
  int j = 0;

  char c;
  while ((c = fgetc(fd)) != EOF)
  {
  	if (i>=9)
      	{
	    i = 0;
	    j++;
	}
    	if (c == '.')
	{
 	   	arr[j][i] = '.';
	}
	if (c == '\n')
	{
	continue;	
	}
	if ( c == ' ')
	{
	continue;
	}
	else
	{
		arr[j][i] =   c ;
	}
	i++;
  }
  return 0;
}

char* printArraystr(FILE* fd, char str2[81])
{
  char str[9][9];
  file_to_arraystr(fd,str);
    int k =0;
  for(int i=0;i<9;i++) 
	{ 
		for(int j=0;j<9;j++) 
		{
			str2[k]=str[i][j]; 
			k++; 
		}
	}
  return str2;
}