#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stddef.h>
#include<time.h>
#include<math.h>
int degree, n_threads, lines;
void calc_roots(float** cplx_roots, int degree);
void fun_polar(double * x0re, double * x0im, int  d);
void static inline analyse_parsing(int argc1,const char *argv1[]);
void writingfile(char * conval2,char * attr2);
//void calc_roots(double** cplx_roots, int degree);
char * convergences;
char * attractors;
char * item_done;
#define re lines
#define im lines
//#define threads n_threads
//#define block_size (im / n_threads)
//pthread_mutex_t mutex_sum;
//double a[re], b[re], sum;          //Not to be used
//void* dotprod(void* arg);      //Not to be used

void main(int argc,const char *argv[])
{
    analyse_parsing(argc,argv);
    float *  ptr_roots= (float*) malloc(sizeof(float) * (degree*2));
    float ** cplx_roots = (float**) malloc(sizeof(float*) * degree);
    for( size_t ix = 0, jx = 0; ix < degree; ++ix, jx+=2)
        {
            cplx_roots[ix] = ptr_roots + jx;     
        }
    calc_roots(cplx_roots,degree);
    for (size_t ix=0; ix<degree; ++ix)
        {
            printf("real %f,im  %f \n", cplx_roots[ix][0], cplx_roots[ix][1]);
        }
    //int l=10; // number of rows and columns;
  
    /* allocate memory for all exact roots, should not be in newtonmethod later */    
    /*double *  ptr_roots= (double*) malloc(sizeof(double) * (degree*2));
    double ** cplx_roots = (double**) malloc(sizeof(double*) * degree);
    for ( size_t ix = 0, jx = 0; ix < degree; ++ix, jx+=2)
        {
           cplx_roots[ix] = ptr_roots + jx;
        }*/
     /*the true roots */
    calc_roots(cplx_roots, degree);
  
    /* allocate memory for all initialvalues of x, this should happen within every thread?*/ 
 
    double * ptr_xitr= (double*) malloc(sizeof(double)*(lines*2));
    double ** init_row = (double**) malloc(sizeof(double*)*lines);
    
        /*the actual newtonmethod  */
        // initialize how complex values should be stored //
    for ( size_t ix = 0, jx = 0; ix < lines; ++ix, jx+=2)
        {
          init_row[ix]=ptr_xitr+jx;
        }
    //  for (size_t kx=start; kx<end(maybe(lines)); kx+=numthread){  an idea for  
      /* initial values of x0*/
    for (size_t ix=0; ix < lines; ++ix)
        {
            init_row[ix][0]=-2 + 4*(double)ix/lines;
            init_row[ix][1]=0.03;
            //  printf("re:%f im: %f\n", init_row[ix][0], init_row[ix][1]);
        }   
  
    /* the newton iteration */
  
    char maxiter = 50;  
    char* attr =(char*) malloc( sizeof(char)*lines);
    char* conval=(char*) calloc(lines, sizeof(char));  
    for (size_t jx=0; jx < lines; ++jx)
        {
            //int conval = 0;
            attr[jx]=-1;
            unsigned short int conv_loc = 0;
            for( conv_loc=0; ; ++conv_loc)
	        {
	            double xsquare = init_row[jx][0]*init_row[jx][0] + init_row[jx][1]*init_row[jx][1]; // useful computattion
	            if (xsquare < 10e-6 || init_row[jx][0] > 10e10 || init_row[jx][0] <-1*10e10 || init_row[jx][1] > 10e10 || init_row[jx][1]<-1*10e10)   
	                {
	                    conval[jx]= maxiter; // assign maximum no if iterations 
	                    attr[jx]=(char)degree;
	                    printf("No convergence, conval set to %hhud , attr set to %hhud \n ", conval[jx] , attr[jx]);
	                    break;
	                }
	            if (xsquare < 1.003) //1.002001 is minimal value)
	                {
	                for (int ix=0; ix < degree; ++ix)
		                {
		                    if ((cplx_roots[ix][0]-init_row[jx][0])*(cplx_roots[ix][0]-init_row[jx][0]) + ((cplx_roots[ix][1]-init_row[jx][1])*(cplx_roots[ix][1]-init_row[jx][1])) < 10e-6)
		                        {
		                            attr[jx]=(char)ix; 
		                            // conval[jx]=(char)conv_loc;
		                            printf("conv_loc %d ", conv_loc);// poses problems if conval_loc >250
		                            break;
		                        }
		                }   
	                }
	            if (attr[jx] != -1) 
                    {
	                    printf("hey conv: %hhud , attr: %hhud ,real:%f, im: %f \n ",conval[jx], attr[jx] ,init_row[jx][0],init_row[jx][1]);
	                    break;
	                }
	            fun_polar(&init_row[jx][0],&init_row[jx][1],degree); 
	        }
        if (conv_loc > 50) // or maxiter
	        {
                conv_loc=50;	  
	        }
        conval[jx] = (char)conv_loc;
    }
  //  FOR LATER attractors[kx]=attr;
  //  /* conversion table /*
    for (size_t ix=0; ix<lines ; ++ix)
    {
        printf("conv %hhi , attr %hhi \n", conval[ix], attr[ix]);
    }
    writingfile(conval,attr);
    free(conval);
    free(attr);
    free(init_row);
    free(cplx_roots);
    free(ptr_xitr);
    free(ptr_roots);
}

void fun_polar(double * x0re, double * x0im, int  d)
{    
    // printf(" real=%f, imaginary=%f\n",*x0re,*x0im);
      //  new idea! polar for large d
    if (d==1)
	    {
	    *x0re=1;
	    *x0im=0;
	    }
    else if (d==2)
	    {
	        double square= *x0re**x0re + *x0im**x0im;
	        *x0re  =*x0re*(0.5+0.5/square);
	        *x0im  =*x0im*(0.5-0.5/square);
	    }
    else if (d > 2 && d%2 == 1)
	    {      // allt det svåra handlar om termen 1/x0 . tänk i polära koordinater
	        double angle =-1*atan2(*x0im,*x0re)*(d-1); // ger vinkeln av nya komplexa talet i radianer, negativ pga är komplexkojugatet
	        double invsquare= 1.0f/(*x0re**x0re+ *x0im**x0im); //
	        double divisor= 1.0f/d;
	        for (size_t ix=1; ix<d; ix+=2) 
	            {
	                divisor *= invsquare;   // ger talet man ska dela på. för udda tal e d chill ingen squareroot
	            }	    
	        *x0re  =*x0re *(1-1.0/d) + cos(angle)*divisor;
            *x0im  =*x0im *(1-1.0/d) + sin(angle)*divisor;
	    }
      
    else // note we only need for d=4, maybe skip forloop later just write d=4
	    {
	        double angle=-1*atan2(*x0im,*x0re)*(d-1); // ger vinkeln av nya komplexa talet i radianer, negativ pga är komplexkojugatet
	        double invsquare= 1.0f/(*x0re**x0re+ *x0im**x0im); //
	        double divisor= sqrt(invsquare)/d; // tråkigt med en squareroot men kom ej undan :(
	        for (size_t ix = 2; ix < d; ix+=2) 
	            {
	                divisor *= invsquare;   // ger talet man ska dela på. för udda tal e d chill ingen squareroot
	            }	    
	        *x0re  =*x0re *(1-1.0/d) + cos(angle)*divisor;
	        *x0im  =*x0im *(1-1.0/d) + sin(angle)*divisor;
	    }
}
  

/*void calc_roots(double** cplx_roots, int d){                                                                                                                                             
  double twopi=6.283185;
  cplx_roots[0][0]=1.0; // always one real root                                                                                                                                         
  cplx_roots[0][1]=0.0;
  
  for (size_t ix=1; ix < d; ++ix){
    cplx_roots[ix][0]= cos ((double)ix*twopi/d);
    cplx_roots[ix][1]= sin ((double)ix*twopi/d);
  }
}*/

void calc_roots(float** cplx_roots, int degree)
{                                                                                                                                                                      float twopi=6.283185;
  cplx_roots[0][0]=1.0; // always one real root
  cplx_roots[0][1]=0.0;
  for (size_t ix=1; ix < degree; ++ix)
  {
    cplx_roots[ix][0]= cos ((float)ix*twopi/degree);
    cplx_roots[ix][1]= sin ((float)ix*twopi/degree);
  }
}


  // if d is 1, then newton always converges to 1+ 0i in one iteration, super simple
  //all roots are on unit circle, can we use this in determining if more iterations are necessary? how?

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
void writingfile(char * conval2, char * attr2)
{
    printf("begin write\n");
    int count,count2,flag = degree;
    int color[3];
    char colorstr[flag][7];
    FILE * fptr;
    fptr=fopen("pixel.ppm","w");
    fprintf(fptr, "P3\n%d %d\n%d\n", re, lines,degree);
    //int attr[7] = {1,2,3,4,5,6,7};
    //get 1000 attr from transfer
    count = 0;count2=0;
    for(int i=0 ; i<degree ; ++i)
    { 
        //color[0] = 3;
        //color[1] = 3;
        //color[2] = 0;
        color[0]=(i) % degree;
        color[1]=(i+2) % degree;
        color[2]=(i+1) % degree;
        sprintf(colorstr[i],"%d %d %d ",color[0],color[1],color[2]);
        printf("%s\n",colorstr[i]);
    }
    printf("size of colorstr %d\n",strlen(colorstr[1]));
    do
    {
      count2=1;
      do
      {
          flag = attr2[count2]; 
          fwrite(colorstr[flag], 6 , 1 , fptr);
      }while(++count2 < re);
      fwrite("\n",1,1,fptr);
    }while (++count < lines);
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
