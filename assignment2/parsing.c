#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>
#include<time.h>
int degree, n_threads, lines;


#define re 1000
#define im lines
double a[re], b[re], sum;          //Not to be used

#define threads n_threads
pthread_mutex_t mutex_sum;

#define block_size (im / n_threads)
void static inline analyse_parsing(int argc1,const char *argv1[]);
void writingfile();

void* dotprod(void* arg);      //Not to be used


void main(int argc,const char *argv[])
{
    analyse_parsing(argc,argv);
    writingfile();
}

/*#define s 10
void main()
{
    
    clock_t t;
    t=clock();
    int ** matrix = (int**) malloc(sizeof(int*)*s);
    int * array;   
           
    //Non Contiguous Memory
    
    /*for(int i=0;i<s;++i)
        matrix[i]=(int*) malloc(sizeof(int)*s);
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
            matrix[i][j]=i+j;
   */ 

    /*Contiguous memory
    array = (int*) malloc(sizeof(int)*s*s);
    for(int i=0,j=0;i<s;++i,j+=s)
    matrix[i]=array+j;
    for(int i=0;i<s;++i)
    for(int j=0;j<s;++j)
    matrix[i][j]=i+j;
    
    FILE * fptr;
    fptr=fopen("readwrite.bin","w");
    
    int count=0;
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
        {
            fwrite(&matrix[i][j],sizeof(matrix[i][j]),1,fptr);
        }
    fclose(fptr);
    t=clock() - t;
    double time = (double)(t)/CLOCKS_PER_SEC;
    printf("\nTime taken by to write in the file is %lf miliseconds\n\n",time*1000);
    
    //reading the file
    
    fptr=fopen("readwrite.bin","r");
    printf("Reading matrix from file\nMatrix || File \n");
    int arr[s][s];
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
            fread(&arr[i][j],sizeof(int),1,fptr);
    count=0;
    for(int i=0;i<s;++i)
        {
            for(int j=0;j<s;++j)
            {
                printf("%d||%d\t",matrix[i][j],arr[i][j]);
                if(arr[i][j]!=matrix[i][j])
                    ++count;
            }
            printf("\n");
        }
    if(count==0)
    printf("Both the matrix and file content are same\n");
    fclose(fptr);
    free(matrix);
    free(array);
}*/
void writingfile()
{
    printf("begin write\n");
    int count,count2,flag;
    int color[3];
    char colorstr[flag][3];
    FILE * fptr;
    fptr=fopen("pixel.ppm","w");
    fprintf(fptr, "P3\n%d %d\n255\n", re, lines);
    int attr[7] = {1,2,3,4,5,6,7};
    //get 1000 attr from transfer
    count = 1;count2=1;
    for(int i=0 ; i<degree ; ++i)
    { 
        color[0]=(i*11) % 256;
        color[1]=((i+2)*11) % 256;
        color[2]=((i+1)*11) % 256;
        sprintf(colorstr[i],"%d %d %d",color[0],color[1],color[2]);
    }
    printf("assigned color\n");
    do
    {
      count2=0;
      do
      {
          flag = rand()%degree+1;  
          fwrite(colorstr[flag],sizeof(colorstr[flag]) , 1 , fptr);
      }while(++count2 <= re);
    }while (++count <= lines);
    fclose(fptr);
}
/*void threadsusage()
{
    int ret;
    size_t i, j;
    pthread_t n_threads[n_threads];

    

    for (i=0; i < re; ++i)
        a[i] = b[i] = 2;
    sum = 0;

  // thread creation and joining

    pthread_mutex_init(&mutex_sum, NULL);
    for (j=0, i=0; j < n_threads; ++j, i+=block_size) 
    {
        double ** arg = malloc(2*sizeof(double*));
        arg[0] = a+i; 
        arg[1] = b+i;
        if (ret = pthread_create(n_threads+j, NULL, dotprod, (void*)arg)) 
        {
            printf("Error creating thread: \n", ret);
            exit(1);
        }
    }

    for (j=0; j < n_threads; ++j) 
    {
        if (ret = pthread_join(n_threads[j], NULL)) 
        {
            printf("Error joining thread: %d\n", ret);
            exit(1);
        }
    }
    pthread_mutex_destroy(&mutex_sum);
    printf("sum = %f\n", sum);
}
void* dotprod(void *restrict arg) {
  double * a_loc = ((double**)arg)[0];
  double * b_loc = ((double**)arg)[1];
  free(arg);

  double sum_loc = 0;
  for (size_t i=0; i < block_size; ++i)
    sum_loc += a_loc[i]*b_loc[i];

  pthread_mutex_lock(&mutex_sum);
  sum += sum_loc;
  pthread_mutex_unlock(&mutex_sum);
}
*/


void static inline analyse_parsing(int argc1,const char *argv1[])
{
    int index,iter=0;
    char * args;
    while((index=getopt(argc1,(char* const*)argv1,"-t:-l::"))!=-1)
    {
      // To get a better understanding of iterations, optarg and argv values. 
      /*printf("\nIteration no %d, argc = %d\n",++iter,argc);
        for(int i=0;i<argc;++i)
        {
            printf("\noptarg is %s \t argv is %s",optarg,argv[i]);
        }*/
      
        ++iter;
        if(index=='?')
            printf("Invalid arguement/No value was passed for %c\n",optopt);
        else if(index=='t')
        {
            //printf("\nThe value of optargs is %s",optarg);
            if(optarg[0]=='-')
            {
                //index = ':';
                printf("No value is passed for threads\n");
                //continue;
            }
            else
            {
                args = optarg;
                //printf("\nThe value of args is %s",args);
                n_threads = strtol(args,(char ** restrict)argv1,10);
                printf("The number of threads is %d\n",n_threads);
            }
        }
        else if(index=='l')
        {
            if(optarg[0]=='-')
            {
                //index = ':';
                printf("No value is passed for lines\n");
            }
            else
            {
                args = optarg;
                //printf("\nThe value of args is %s",args);
                lines = strtol(args,(char ** restrict)argv1,10);
                printf("The no of lines is %d\n",lines);
            }
        }
        else
        {
            
            if(!optarg[0])
            {
                //index = ':';
                printf("No value is passed for degree\n");
            }
            else
            {
                args = optarg;
                degree = strtol(args,(char ** restrict)argv1,10);
                printf("The value of degree is %d\n",degree);
            }
        }
    }
    if(iter==0)
    {
        printf("No value is passed for both threads, degree and lines\n");
    }
}
