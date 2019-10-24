//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1 , int * length1, int * width1);

int main(int argc,const char * argv[])
{
  int num_iterations,length,width;
  float diffusion_constant;
  #define d diffusion_constant
  analyse_parsing(argc , argv , &num_iterations , &diffusion_constant, &length, &width);
  char filename[100];
  sprintf(filename,"diffusion_%d_%d",length,width);

  /* Open file, read number of characters */

  FILE *fptr;
  fptr = fopen(filename,"r");
  fseek(fptr,0,SEEK_END);
  int last_character = (ftell(fptr));
  fseek(fptr,0,SEEK_SET);
  fscanf(fptr,"%d  %d",&length,&width);

  length += 2;
  width += 2;
  /* Assign all doubles with initial value as 0 and boundary too*/  
  double ** temperatureold = (double **)malloc(sizeof(double*) * length );
  double ** temperaturenew = (double **)malloc(sizeof(double*) * length );
  double * contiguousold = (double*)malloc(sizeof(double)*length*width);
  double * contiguousnew = (double*)malloc(sizeof(double)*length*width);
  for(int i=0,j=0;i<length;++i,j+=width)
  {
    temperaturenew[i] = contiguousnew + j;
    temperatureold[i] = contiguousold + j;
  }

  int x_position,y_position;
  for(int i=0 ; i<length; ++i)
  {
    for(int j=0 ; j<width ; ++j)
    {
      temperatureold[i][j] = 0;
      temperaturenew[i][j] = 0;
    }
  }

  /* Read temperatureolds from file and store doubles from file into array */  
  do
  {
    //fscanf(fptr,"%d %d %Lf",&x_position,&y_position,&temperatureold[x_position][y_position]);
    fscanf(fptr,"%d %d ",&x_position,&y_position);
    fscanf(fptr,"%lf",&temperatureold[x_position+1][y_position+1]);
  }while(ftell(fptr)<last_character);

  fclose(fptr);

  for(int i=1; i<length-1 ; ++i)
  {
    for(int j=1 ; j<width-1 ; ++j)
    {
      temperaturenew[i][j] = temperatureold[i][j];
    }
  } 

  #define told temperatureold
  #define tnew temperaturenew
  
  /* Calculate heat diffusion for n - iterations */
  for(int k=0 ; k < num_iterations ; ++k)
  {
    for(int i=1 ; i < length - 1 ; ++i)
    {
      for(int j=1 ; j < width - 1 ; ++j)
      tnew[i][j] = told[i][j] + ((double)d * ((told[i-1][j] + told[i+1][j] + told[i][j-1] + told[i][j+1])/4 - told[i][j]));
    }
    for(int i=1 ; i < length - 1 ; ++i)
    {
      for(int j=1 ; j < width - 1 ; ++j)
      told[i][j] = tnew[i][j];
    }
  }

// for(int i=length-1,iter=0;iter<5;--i)
// {
//   //printf("\n");
//   for(int j=width-1;iter<5;--j)
//   {
//     if(told[i][j]!=0)
//     {
//       printf("%lf\n",told[i][j]);
//       ++iter;
//     }
//   }
// }

  double average = 0,averageabsolute = 0, difference = 0;

  for(int i=1; i<length - 1; ++i)
  {
    for(int j=1; j<width - 1; ++j)
    {
      average += told[i][j];
    }
  }
  average = average/((length-2)*(width-2));

  for(int i=1; i<length - 1; ++i)
  {
    for(int j=1; j<width - 1; ++j)
    {
      difference = told[i][j] - average;
      averageabsolute += sqrtl(difference*difference);
    }
  }
  averageabsolute = averageabsolute/((length-2)*(width-2));

  printf("%lf\n%lf\n",average,averageabsolute);

  free(contiguousnew);
  free(contiguousold);
  free(temperaturenew);
  free(temperatureold);
  return 0;
}

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1, int * length1, int * width1)
{
  int index, iter=0; 
  char * args;
  while((index=getopt(argc1,(char* const*)argv1,"-n:-d:-l:-w:"))!=-1)
    {
      ++iter;
      if (index == 'n')
	    {
	      args = optarg;
        *num_iterations1 = strtol(args,(char ** restrict)argv1,10);
      }
      else if(index == 'd')
      {
        args = optarg;
        *diffusion_constant1 = strtof(args,(char ** restrict)argv1);
      }
      else if(index == 'l')
      {
        args = optarg;
        *length1 = strtof(args,(char ** restrict)argv1);
      }
      else if(index == 'w')
      {
        args = optarg;
        *width1 = strtof(args,(char ** restrict)argv1);
      }
    }
  if(iter!=4)
  {
    printf("No values is passed for iterations and diffusion constant\n");	  
    exit(0);
  }
}