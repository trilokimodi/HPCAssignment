#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include <time.h>


void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1, int * length1, int * width1);


MPI_Status status;

main(int argc, char **argv)
{
  
  int num_mpi_proc , mpi_rank , numworkers;
  struct timespec tg1, tg2;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_mpi_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  numworkers = num_mpi_proc-1;

  /*---------------------------- master ----------------------------*/
  if (mpi_rank == 0) 
  {
    int num_iterations,length,width;
    float diffusion_constant;
    double * temperatureold, * temperaturenew;
    #define d diffusion_constant
    analyse_parsing(argc , argv , &num_iterations , &diffusion_constant, &length, &width);  

    /* Open file, read number of characters */
    FILE *fptr;
    char finumele2changeame[100];
    sprintf(finumele2changeame,"diffusion_%d_%d",length,width);
    fptr = fopen(finumele2changeame,"r");
    if (!fptr)
    {
      printf("can't open diffusion file\n");
      exit(0);
    }
    fseek(fptr,0,SEEK_END);
    int last_character = (ftell(fptr));
    fseek(fptr,0,SEEK_SET);
    fscanf(fptr,"%d  %d",&length,&width);
    length+=2;
    width+=2;
  
    /* Assign all doubles with initial value as 0 and boundary too*/  
    temperatureold = (double *)malloc(sizeof(double) * length * width);
    temperaturenew = (double *)malloc(sizeof(double) * length * width);

    for (int i = 0; i < length*width ; ++i)
    {
      temperatureold[i] = 0;
      temperaturenew[i] = 0;
    }

    int x_position,y_position;

    // Initialize the temperature values from file
    do
    {
      fscanf(fptr ,"%d %d ", &x_position, &y_position);
      fscanf(fptr ,"%lf", &temperatureold[(x_position+1)*width + (y_position+1)]);
    }while(ftell(fptr)<last_character);

    fclose(fptr);  

    #define told temperatureold
    #define tnew temperaturenew

    timespec_get(&tg1,TIME_UTC);
    int numele2change = (length-2)*(width-2)/(numworkers+1);
    int numlines2copy = numele2change/(width-2);
    int lastpositiontoworker;
    int flag=0,dest;
    int numelems2copymaster = numele2change + 2*numlines2copy;

    /* send matrix data to the worker tasks */
    for(int iter = 0; iter<num_iterations/2;++iter)
    {
      printf("iteration no %d\n",iter);
      for (dest=1; dest <= numworkers; ++dest)
      {
        flag = 1;
        MPI_Send(&numele2change, 1 , MPI_INT , dest , 1 , MPI_COMM_WORLD);
        MPI_Send(&length, 1 , MPI_INT , dest , 2 , MPI_COMM_WORLD);
        MPI_Send(&width, 1 , MPI_INT , dest , 3 , MPI_COMM_WORLD);
        int startingposition = (dest-1)*numele2change + width;
        MPI_Send(&startingposition, 1 , MPI_INT , dest , 4 , MPI_COMM_WORLD);
        MPI_Send(&d, 1 , MPI_FLOAT , dest , 5 , MPI_COMM_WORLD);
        MPI_Send(&told[0], length*width , MPI_DOUBLE , dest , 6 , MPI_COMM_WORLD);
        MPI_Send(&numlines2copy, 1 , MPI_INT , dest , 7 , MPI_COMM_WORLD);
      }

      if(flag > 0)
      {
        lastpositiontoworker = (dest-1)*numele2change+width + width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          tnew[rem_x + rem_y] = told[rem_x + rem_y] + (d*(((told[rem_x + rem_y-1] + told[rem_x + rem_y+1] + told[rem_x + rem_y - width] + told[rem_x + rem_y + width])/4)-told[rem_x + rem_y]));
      }
      else
      {
        lastpositiontoworker = width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          tnew[rem_x + rem_y] = told[rem_x + rem_y] + (d*(((told[rem_x + rem_y-1] + told[rem_x + rem_y+1] + told[rem_x + rem_y - width] + told[rem_x + rem_y + width])/4)-told[rem_x + rem_y]));
      }

      /* wait for results from all worker tasks */
      for (int source=1; source<=numworkers; ++source)
      {
        int startingposition;
        MPI_Recv(&startingposition, 1 , MPI_INT, source, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(&tnew[startingposition], numelems2copymaster, MPI_DOUBLE, source, 9, MPI_COMM_WORLD, &status);
      }

int counts;
for(int i=0;i<length;++i)
{
  for(int j=0 ; j<width; ++j)
  {
    if(tnew[i*width+j]!=0)
    //printf("%lf ",tnew[i*width + j]);
    counts++;
  }
}
printf("Actual ans for -n2 -d0.01 100*100 is 486,1699. Our counts = %d\n",counts);

      //Exchange of memories
      for (dest=1; dest <= numworkers; ++dest)
      {
        flag = 1;
        MPI_Send(&numele2change, 1 , MPI_INT , dest , 1 , MPI_COMM_WORLD);
        printf("Sending message\n");
        MPI_Send(&length, 1 , MPI_INT , dest , 2 , MPI_COMM_WORLD);
        MPI_Send(&width, 1 , MPI_INT , dest , 3 , MPI_COMM_WORLD);
        int startingposition = (dest-1)*numele2change + width;
        MPI_Send(&startingposition, 1 , MPI_INT , dest , 4 , MPI_COMM_WORLD);
        MPI_Send(&d, 1 , MPI_FLOAT , dest , 5 , MPI_COMM_WORLD);
        printf("Sending message\n");
        MPI_Send(&tnew[0], length*width , MPI_DOUBLE , dest , 6 , MPI_COMM_WORLD);
        printf("Sending message\n");
        MPI_Send(&numlines2copy, 1 , MPI_INT , dest , 7 , MPI_COMM_WORLD);
        
      }

      if(flag > 0)
      {
        lastpositiontoworker = (dest-1)*numele2change+width + width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          told[rem_x + rem_y] = tnew[rem_x + rem_y] + (d*(((tnew[rem_x + rem_y-1] + tnew[rem_x + rem_y+1] + tnew[rem_x + rem_y - width] + tnew[rem_x + rem_y + width])/4)-tnew[rem_x + rem_y]));
      }
      else
      {
        lastpositiontoworker = width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          told[rem_x + rem_y] = tnew[rem_x + rem_y] + (d*(((tnew[rem_x + rem_y-1] + tnew[rem_x + rem_y+1] + tnew[rem_x + rem_y - width] + tnew[rem_x + rem_y + width])/4)-tnew[rem_x + rem_y]));
      }

      /* wait for results from all worker tasks */
      for (int source=1; source<=numworkers; ++source)
      {
        int startingposition;
        MPI_Recv(&startingposition, 1 , MPI_INT, source, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(&told[startingposition], numelems2copymaster, MPI_DOUBLE, source, 9, MPI_COMM_WORLD, &status);
      }
    }

    if(num_iterations % 2 !=0)
    {
      for (dest=1; dest <= numworkers; ++dest)
      {
        flag = 1;
        MPI_Send(&numele2change, 1 , MPI_INT , dest , 1 , MPI_COMM_WORLD);
        MPI_Send(&length, 1 , MPI_INT , dest , 2 , MPI_COMM_WORLD);
        MPI_Send(&width, 1 , MPI_INT , dest , 3 , MPI_COMM_WORLD);
        int startingposition = (dest-1)*numele2change + width;
        MPI_Send(&startingposition, 1 , MPI_INT , dest , 4 , MPI_COMM_WORLD);
        MPI_Send(&d, 1 , MPI_FLOAT , dest , 5 , MPI_COMM_WORLD);
        MPI_Send(&told[0], length*width , MPI_DOUBLE , dest , 6 , MPI_COMM_WORLD);
        MPI_Send(&numlines2copy, 1 , MPI_INT , dest , 7 , MPI_COMM_WORLD);
      }

      if(flag > 0)
      {
        lastpositiontoworker = (dest-1)*numele2change+width + width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          tnew[rem_x + rem_y] = told[rem_x + rem_y] + (d*(((told[rem_x + rem_y-1] + told[rem_x + rem_y+1] + told[rem_x + rem_y - width] + told[rem_x + rem_y + width])/4)-told[rem_x + rem_y]));
      }
      else
      {
        lastpositiontoworker = width;
        for(int rem_x = lastpositiontoworker+1; rem_x < (length - 1)*width; rem_x+=width)
        for(int rem_y = 0;rem_y < width - 2;++rem_y)
          tnew[rem_x + rem_y] = told[rem_x + rem_y] + (d*(((told[rem_x + rem_y-1] + told[rem_x + rem_y+1] + told[rem_x + rem_y - width] + told[rem_x + rem_y + width])/4)-told[rem_x + rem_y]));
      }

      /* wait for results from all worker tasks */
      for (int source=1; source<=numworkers; ++source)
      {
        int startingposition;
        MPI_Recv(&startingposition, 1 , MPI_INT, source, 8, MPI_COMM_WORLD, &status);
        MPI_Recv(&tnew[startingposition], numelems2copymaster, MPI_DOUBLE, source, 9, MPI_COMM_WORLD, &status);
      }
      
      // Calculate average after n iterations
      double average = 0,averageabsolute = 0, difference = 0;
      for(int i = width+1 ; i < ((length-1)*width)-1; ++i)  
        average += tnew[i];
      average = average/((length-2)*(width-2));

      // Calculate average absolute differences after n iterations
      for(int i = 1; i < length - 1; ++i)
      {
        for(int j = 1; j < width - 1; ++j)
        {
          difference = tnew[i*width + j] - average;
          averageabsolute += sqrtl(difference*difference);
        }
      } 
      averageabsolute = averageabsolute/((length-2)*(width-2));

      //Display result
      printf("%lf\n%lf\n",average,averageabsolute);
    }
    else
    {
      // Calculate average after n iterations
      double average = 0,averageabsolute = 0, difference = 0;
      for(int i = width+1 ; i < ((length-1)*width)-1; ++i)  
        average += told[i];
      average = average/((length-2)*(width-2));

      // Calculate average absolute differences after n iterations
      for(int i = 1; i < length - 1; ++i)
      {
        for(int j = 1; j < width - 1; ++j)
        {
          difference = told[i*width + j] - average;
          averageabsolute += sqrtl(difference*difference);
        }
      } 
      averageabsolute = averageabsolute/((length-2)*(width-2));

      //Display result
      printf("%lf\n%lf\n",average,averageabsolute);
    }
    
    timespec_get(&tg2,TIME_UTC);
    double timeelapsed = (double)((double)(tg2.tv_sec - tg1.tv_sec))+(double)((double)(tg2.tv_nsec - tg1.tv_nsec)/1000000000);
    printf("Time taken for one single iteration by timespec_get is %lf\n",timeelapsed);
  }


  /*---------------------------- worker----------------------------*/
  if (mpi_rank > 0)
  {
    fprintf(stdout,"Worker\n");
    int source = 0; int numele2change, length , width, startingposition, numlines2copy,err; 
    int numeletnew;
    float dc;
    MPI_Recv(&numele2change, 1 , MPI_INT , source , 1 , MPI_COMM_WORLD, &status);
    MPI_Recv(&length, 1 , MPI_INT , source , 2 , MPI_COMM_WORLD, &status);
    MPI_Recv(&width, 1 , MPI_INT , source , 3 , MPI_COMM_WORLD, &status);
    MPI_Recv(&startingposition, 1 , MPI_INT , source , 4 , MPI_COMM_WORLD, &status);
    MPI_Recv(&dc, 1 , MPI_FLOAT , source , 5 , MPI_COMM_WORLD, &status);
    double * told1 = (double *)malloc(sizeof(double) * length * width);
    MPI_Recv(&told1[0], length*width , MPI_DOUBLE, source, 6, MPI_COMM_WORLD, &status);
    MPI_Recv(&numlines2copy, 1 , MPI_INT , source , 7 , MPI_COMM_WORLD, &status);   
int counts = 0;
for(int i=0;i<length;++i)
for(int j=0;j<width;++j)
if(told1[i*width+j]>0)
counts++;
fprintf(stdout,"counts told warker = %d\n",counts);


    numeletnew = numele2change+2*numlines2copy;

    double * tnew1 = (double *)malloc(sizeof(double) * numeletnew);
    for(int i=0; i<numeletnew; ++i)
      tnew1[i] = 0;

    /* computation of heat temperatures*/
    int j,k = startingposition+1,count=0;
    for(int i=k,tnew_line=0; count < numele2change; i+=width,++tnew_line)
    {
      for(j=1; j<width-1; ++j)
      {        
        tnew1[tnew_line*width+j] = told1[tnew_line*width + k + j-1] + dc * (((told1[tnew_line*width+k+j] + told1[(tnew_line+1)*width+k+j-1] + told1[(tnew_line-1)*width + k+j-1] + told1[tnew_line*width+k-1+j-1])/4) - told1[tnew_line*width+k+j-1]);
        ++count;
      }
    }

    MPI_Send(&startingposition, 1, MPI_INT, 0, 8, MPI_COMM_WORLD);
    MPI_Send(&tnew1[0], numeletnew, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
    free(told1);
    free(tnew1);
  }
  MPI_Finalize();
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
