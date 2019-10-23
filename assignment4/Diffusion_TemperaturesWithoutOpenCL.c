//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <getopt.h>

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations, float * diffusion_constant);

int main(int argc,const char * argv[])
{
  int num_iterations,length,width;
  float diffusion_constant;
  #define d diffusion_constant
  analyse_parsing(argc , argv , &num_iterations , &diffusion_constant);

  /* Open file, read number of characters */

  FILE *fptr;
  fptr = fopen("diffusion_100_100","r");
  fseek(fptr,0,SEEK_END);
  int last_character = (ftell(fptr));
  fseek(fptr,0,SEEK_SET);
  fscanf(fptr,"%d  %d",&length,&width);

  /* Assign all long doubles with initial value as 0 and boundary too*/  
  long double temperatureold[length+2][width+2],temperaturenew[length+2][width+2];
  int x_position,y_position;
  for(int i=0 ; i<length+2; ++i)
  {
    for(int j=0 ; j<width+2 ; ++j)
    {
      temperatureold[i][j] = 0;
      temperaturenew[i][j] = 0;
    }
  }

  /* Read temperatureolds from file and store long doubles from file into array */  
  do
  {
    //fscanf(fptr,"%d %d %Lf",&x_position,&y_position,&temperatureold[x_position][y_position]);
    fscanf(fptr,"%d %d ",&x_position,&y_position);
    fscanf(fptr,"%Lf",&temperatureold[x_position+1][y_position+1]);
  }while(ftell(fptr)<last_character);

  fclose(fptr);

  for(int i=1; i<length+1 ; ++i)
  {
    for(int j=1 ; j<width+1 ; ++j)
    {
      temperaturenew[i][j] = temperatureold[i][j];
    }
  } 

  #define told temperatureold
  #define tnew temperaturenew
  
  /* Calculate heat diffusion for n - iterations */
  for(int k=0 ; k < num_iterations ; ++k)
  {
    for(int i=1 ; i < length + 1 ; ++i)
    {
      for(int j=1 ; j < width + 1 ; ++j)
      tnew[i][j] = told[i][j] + ((long double)d * ((told[i-1][j] + told[i+1][j] + told[i][j-1] + told[i][j+1])/4 - told[i][j]));
    }
    for(int i=1 ; i < length + 1 ; ++i)
    {
      for(int j=1 ; j < width + 1 ; ++j)
      told[i][j] = tnew[i][j];
    }
  }

  long double average = 0,averageabsolute = 0, difference = 0;

  for(int i=1; i<length+1; ++i)
  {
    for(int j=1; j<width+1; ++j)
    {
      average += told[i][j];
    }
  }
  average = average/(length*width);

  for(int i=1; i<length+1; ++i)
  {
    for(int j=1; j<width+1; ++j)
    {
      difference = told[i][j] - average;
      averageabsolute += sqrtl(difference*difference);
    }
  }
  averageabsolute = averageabsolute/(length*width);

  printf("%Lf\n%Lf\n",average,averageabsolute);
  return 0;
}

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations, float * diffusion_constant)
{
  int index, iter=0; 
  char * args;
  while((index=getopt(argc1,(char* const*)argv1,"-n:-d:"))!=-1)
    {
      ++iter;
      if (index == 'n')
	    {
	      args = optarg;
        *num_iterations = strtol(args,(char ** restrict)argv1,10);
      }
      else if(index == 'd')
      {
        args = optarg;
        *diffusion_constant = strtof(args,(char ** restrict)argv1);
      }
    }
  if(iter==0)
  {
    printf("No values is passed for iterations and diffusion constant\n");	  
    exit(0);
  }
}