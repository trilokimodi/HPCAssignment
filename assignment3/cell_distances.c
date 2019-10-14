//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <getopt.h>

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_threads);

int main(int argc,const char * argv[])
{
  int num_thread;
  unsigned long counter[3465]={ 0 } ;
  analyse_parsing(argc,argv,&num_thread);

  /* Open file, read number of lines */
  FILE *fptr;
  fptr = fopen("cells","r");
  fseek(fptr,0,SEEK_END);
  int num_lines = (ftell(fptr))/24;
  fseek(fptr,0,SEEK_SET);
  
  /* Read coordinates from file and convert floats into ints by multiplying by 1000 */  
  int coordinate[num_lines][3];
  float floatvalues[3];
  for(int i=0 ; i<num_lines ; ++i)
  {
    fscanf(fptr,"%f %f %f",&floatvalues[0],&floatvalues[1],&floatvalues[2]);
    coordinate[i][0] = 1000*floatvalues[0];
    coordinate[i][1] = 1000*floatvalues[1];
    coordinate[i][2] = 1000*floatvalues[2];
  }
  fclose(fptr);

  /* Calculate distances using OpenMP threads */    
  omp_set_num_threads(num_thread);
  #pragma omp parallel shared(num_lines,counter)
  {
    int dif1,dif2,dif3;
    float dist= 0;
    unsigned int loc_counter[3465]= { 0 };

    /* Let each thread compute 50 iterations per run */ 
    #pragma omp for schedule(static, 50)
      for (size_t ix=0; ix < num_lines ;++ix)
	    {
	      for(size_t jx= ix+1; jx < num_lines; ++jx)
	      {
	        dif1 = coordinate[ix][0]-coordinate[jx][0];
	        dif2 = coordinate[ix][1]-coordinate[jx][1];
	        dif3 = coordinate[ix][2]-coordinate[jx][2];
	        dist = sqrtf((float)(dif1*dif1 + dif2*dif2 + dif3*dif3));
	        loc_counter[(int)(dist*0.1)]++;
	      }
	    }

    /* Adding thread local counts to global counter */  
    #pragma omp critical (combine_local_counters)
	  {
      for (size_t ix =0; ix<3465; ++ix)
	      counter[ix]+=loc_counter[ix];
    }
  }

  /* Print distances and frequency */
  for (int ix =0 ; ix < 3465; ++ix)
	  printf("%05.2f %d \n", ix*0.01f, counter[ix]);

  return 0;
}

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_threads)
{
  int index, iter=0; 
  char * args;
  while((index=getopt(argc1,(char* const*)argv1,"-t:"))!=-1)
    {
      ++iter;
      if (index=='t')
	    {
	      args = optarg;
        *num_threads = strtol(args,(char ** restrict)argv1,10);
      }
    }
  if(iter==0)
    printf("No values is passed for threads\n");	  
}