#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stddef.h>
#include<time.h>
#include<math.h>
static int degree, n_threads, lines;
void static inline analyse_parsing(int argc1,const char *argv1[]);
void fun_polar(float * x0re, float * x0im, int  d);
void calc_roots(float** roots, int d);
void* newton_main(void* args);
void* write_main( void * args);
void writingfile(char * conval2,char * attr2);
char ** convergences;
char ** attractors;
char * item_done;
float ** roots;
char * item_done;
int n_x0;
pthread_mutex_t item_done_mutex;
#define d degree
#define l lines
#define re lines
#define im lines
#define B 1000000000
#define SIZE 1000

void main(int argc,const char *argv[])
{
    analyse_parsing(argc,argv);
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
    pthread_t write_thread; //  = (pthread_t*)malloc(sizeof(pthread_t)); no need to malloc
    n_x0 = l * l;
    
    // Allocate memory for global attactors and convergences
    convergences = (char**) malloc(sizeof(char* ) *l);
    attractors= (char**) malloc(sizeof(char* )* l);
    item_done = (char*) calloc(l,sizeof(char));

    // Allocate memory for all exact roots
    float * roots_entries = (float*) malloc(sizeof(float) * (d*2));
    roots = (float**) malloc(sizeof(float*) * d);
    for ( size_t ix = 0, jx = 0; ix < d; ++ix, jx+=2)
        roots[ix] = roots_entries + jx;
    
    // Calculate exact roots
    calc_roots(roots, d);

    // Create threads
    for (size_t i = 0; i < n_threads; ++i)
    {
        size_t* args = malloc(sizeof(size_t));
        *args = i;
        pthread_create(&threads[i], NULL, newton_main, (void*) args);
    }

    pthread_create(&write_thread, NULL, write_main, NULL);

    
    // Join threads
    for (size_t i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);

    pthread_join(write_thread,NULL);

    // Finish up, go home
    pthread_mutex_destroy(&item_done_mutex);
   
    free(roots);
    free(roots_entries);
    free(threads);
    free(convergences);
    free(attractors);
    free(item_done);
}
//char * attractor; // it is typecasted again in newton_main and introduced in write_main.
//char * convergence; // it is typecasted again in newton_main and introduced in write_main.

void* write_main( void * args)
{
    struct timespec sleep_timespec;
    sleep_timespec.tv_sec = 0;
    sleep_timespec.tv_nsec = 250;
    int flag = degree, colorgrey,len;
    char filename[100];
    char colorstrgrey[51][13];
    char* greystring = (char*)malloc(sizeof(char)*lines*10);
    char* colorstring = (char*)malloc(sizeof(char)*lines*7);
    FILE *fcolor, *fgrey;
    char colorstr[12][7] = 
    {
    "4 0 0 ","0 4 0 ","0 0 4 ","3 3 0 ","0 5 5 ","3 0 5 ","4 2 0 ","4 0 2 ","2 0 4 ","2 4 0 "
    ,"0 2 4 ","0 4 2 "
    }; 
    for(int i=50,j=0 ; j<=50 ; --i,++j)
    { 
        colorgrey = 50-i;
        sprintf(colorstrgrey[j],"%d %d %d ",colorgrey,colorgrey,colorgrey);
    }
    struct timespec tbegin, tend;
    double timeelapsed;
    int count = 1;
    char * attractor; 
    char * convergence;
    char * item_done_loc = (char*)calloc(l, sizeof(char));
    sprintf(filename,"newton_convergence_x%d.ppm",degree);
    fcolor=fopen(filename,"w");
    fprintf(fcolor, "P3\n%d %d\n7\n", re, lines);
    sprintf(filename,"newton_attractors_x%d.ppm",degree);
    fgrey=fopen(filename,"w");
    fprintf(fgrey, "P3\n%d %d\n50\n", re, lines);
    for ( size_t ix = 0; ix < l; )
    {
        pthread_mutex_lock(&item_done_mutex);
        if ( item_done[ix] != 0 )
	    {
	        memcpy(item_done_loc, item_done, l*sizeof(char));
	    }
        pthread_mutex_unlock(&item_done_mutex);
      
        if ( item_done_loc[ix] == 0 ) {
	    nanosleep(&sleep_timespec, NULL);
	    continue;
        }
        //Method 1

        //timespec_get(&tbegin,TIME_UTC);
        //do{
        
        count = 1;
        //Method 1

        //timespec_get(&tbegin,TIME_UTC);
        //do{
        //for(int i=0;i<lines;++i)
        //{
      
        //}
     
      
        for ( ; ix < l && item_done_loc[ix] != 0; ++ix )
	    {
            printf("%d\n",ix);
            convergence = convergences[ix]; // this is input for trilo
	        attractor = attractors[ix]; // same here
	        // time for triloki to write result

            for(int i=0,j=0,k=0;i<lines;++i)
            {
                flag = attractor[i];
                k=0;
                do
                {
                colorstring[j++] = colorstr[flag][k++];
                }while(colorstr[flag][k]!='\0');
            }
            len=strlen(colorstring);
            printf("%s\n",colorstring);
            fwrite(colorstring, len , 1 , fcolor);
            fwrite("\n",sizeof("\n"),1,fcolor);
            for(int j=0;j<lines;++j)
            {
                flag = convergence[j];
                fwrite(colorstrgrey[flag], strlen(colorstrgrey[flag]) , 1 , fgrey);
            }
            fwrite("\n",sizeof("\n"),1,fgrey);
            //pthread_mutex_lock(&item_done_mutex);
	        //writingfile(convergence,attractor);	
	  
	        //for(size_t i = 0; i < l; ++i)
	        // printf("Convergences[%zu] = %hhu, attractors[%zu] = %hhu\n", i, convergence[i], i, attractor[i]);
	    }
        free(attractor);
	    free(convergence);
        fclose(fcolor);
        fclose(fgrey); 
    }

    free(item_done_loc);
}

void* newton_main(void* args)
{
  size_t offset = *((size_t*) args);
  free(args);
 // Allocate memory for all initialvalues of x, this should happen oncewithin every thread      
        float * x0_col_entries= (float*) malloc(sizeof(float)*(l*2));
        float ** x0_col = (float**) malloc(sizeof(float*)*l); // Contains x0 values for one row, all columns
        for ( size_t ix = 0, jx = 0; ix < l; ++ix, jx+=2)
        {
            x0_col[ix] = x0_col_entries + jx;
        }  
        
  /* loops over every row */
    for ( size_t row = offset; row < l   ; row += n_threads )
    {
        // Allocate memory for all initialvalues of x, this should happen within every thread      
        //  float * x0_col_entries= (float*) malloc(sizeof(float)*(l*2));
        // float ** x0_col = (float**) malloc(sizeof(float*)*l); // Contains x0 values for one row, all columns
        //for ( size_t ix = 0, jx = 0; ix < l; ++ix, jx+=2)
        //{
         //   x0_col[ix] = x0_col_entries + jx;
        //}  
        
        // Initialise x0_col
        for (size_t col = 0; col < l; ++col)
        {
            x0_col[col][0] = -2 + 4 * (float)col / (l - 1);
            x0_col[col][1] = -2 + 4 * (float)row / (l - 1);
        }
            
        // The newton iteration
        char maxiter = 50;
        // Allocate memory for attractor/convergence
	char * attractor =(char*) calloc(l, sizeof(char)); //it is globally allocated now
	char * convergence=(char*) calloc(l, sizeof(char)); //it is globally allocated now  

        // For x0 in row
        for (size_t jx=0; jx < l; ++jx)
        {
            attractor[jx] = -1;
            unsigned short int k = 0;
            
            // For iteration step k
            for (; ; ++k)
            {
                float xsquare = x0_col[jx][0]*x0_col[jx][0] + x0_col[jx][1]*x0_col[jx][1]; // useful computattion
                
                // If x_k really far away
                if (xsquare < 10e-6 || x0_col[jx][0] > 10e10 || x0_col[jx][0] <-1*10e10 || x0_col[jx][1] > 10e10 || x0_col[jx][1]<-1*10e10)   
                {
                    convergence[jx]= maxiter; // assign maximum no if iterations 
                    attractor[jx]=(char)d;
                    //printf("No convergence, convergence set to %hhud , attractor set to %hhud \n ", convergence[jx] , attractor[jx]);
                    break;
                }

                // If x_k^2 close enough to the origin, we bother checking convergence
                if (xsquare < 1.003) //1.002001 is minimal value)
                {
                    // For each exact root, with index ix
                    for (int ix=0; ix < d; ++ix)
                    {
                        // If distance from root < tol, we are done
                        if ((roots[ix][0]-x0_col[jx][0])*(roots[ix][0]-x0_col[jx][0]) + ((roots[ix][1]-x0_col[jx][1])*(roots[ix][1]-x0_col[jx][1])) < 10e-6)
                        {
                            attractor[jx] = (char)ix;
                            // printf("k %d ", k);//  no longer poses problems if convergence_loc >250
                            break;
                        }
                    }
                }

                // If convergence was reached, stop iteration
                if (attractor[jx] != -1) {
                    //	printf("hey conv: %hhud , attractor: %hhud ,real:%f, im: %f \n ",convergence[jx], attractor[jx] ,x0_col[jx][0],x0_col[jx][1]);
                    break;
                }
                
                // Calculates x_(k+1)
                fun_polar(&x0_col[jx][0],&x0_col[jx][1],d); 
            } // End end iteration for x0

            if (k > 50) // maxiter = 50, but wrong type
                k = 50;	  

            convergence[jx] = (char)k;    
        } // End row
        
        // Debugging
        // for (size_t ix=0; ix<l ; ++ix)
        // {
        //     //  printf("conv %hhi , attractor %hhi \n", convergence[ix], attractor[ix]);
        // }

        // Set convergence and attractor value for row
        convergences[row] = convergence;
        attractors[row]= attractor;

        pthread_mutex_lock(&item_done_mutex);
        item_done[row] = 1;
        pthread_mutex_unlock(&item_done_mutex);

       // free(x0_col);
       // free(x0_col_entries);
    } // End for all rows
    free(x0_col);
    free(x0_col_entries);
    return NULL;
} // End function newton_main

// Calculates x_(k+1)
void fun_polar(float * x0re, float * x0im, int  d)
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
	float square= *x0re * *x0re + *x0im * *x0im;
	  *x0re  =*x0re*(0.5+0.5/square);
	  *x0im  =*x0im*(0.5-0.5/square);
	}
      else if (d > 2 && d%2 == 1)
	{      // allt det svåra handlar om termen 1/x0 . tänk i polära koordinater
	  float angle =-1*atan2(*x0im,*x0re)*(d-1); // ger vinkeln av nya komplexa talet i radianer, negativ pga är komplexkojugatet
	  float invsquare= 1.0f/(*x0re**x0re+ *x0im**x0im); //
	  float divisor= 1.0f/d;
	  for (size_t ix=1; ix<d; ix+=2) 
	    {
	      divisor *= invsquare;   // ger talet man ska dela på. för udda tal e d chill ingen squareroot
	    }	    
	  *x0re  =*x0re *(1-1.0/d) + cos(angle)*divisor;
	  *x0im  =*x0im *(1-1.0/d) + sin(angle)*divisor;
	}
    else // note we only need for d=4, maybe skip forloop later just write d=4
	{
	  float angle=-1*atan2(*x0im,*x0re)*(d-1); // ger vinkeln av nya komplexa talet i radianer, negativ pga är komplexkojugatet
	  float invsquare= 1.0f/(*x0re**x0re+ *x0im**x0im); //
	  float divisor= sqrt(invsquare)/d; // tråkigt med en squareroot men kom ej undan :(
	  for (size_t ix = 2; ix < d; ix+=2) 
	    {
	      divisor *= invsquare;   // ger talet man ska dela på. för udda tal e d chill ingen squareroot
	    }	    
	  *x0re  =*x0re *(1-1.0/d) + cos(angle)*divisor;
	  *x0im  =*x0im *(1-1.0/d) + sin(angle)*divisor;
	}
  }

  void calc_roots(float** roots, int d){                                                                                                                                             
  float twopi=6.283185;
  roots[0][0]=1.0; // always one real root                                                                                                                                         
  roots[0][1]=0.0;
  
  for (size_t ix=1; ix < d; ++ix){
    roots[ix][0]= cos ((float)ix*twopi/d);
    roots[ix][1]= sin ((float)ix*twopi/d);
    }
}

void writingfile(char * conval2, char * attr2)
{
    int flag = degree, colorgrey,len;
    char filename[100];
    char colorstrgrey[51][13];
    char* greystring = (char*)malloc(sizeof(char)*lines*10);
    char* colorstring = (char*)malloc(sizeof(char)*lines*7);
    FILE *fcolor, *fgrey;
    char colorstr[12][7] = 
    {
        "4 0 0 ","0 4 0 ","0 0 4 ","3 3 0 ","0 5 5 ","3 0 5 ","4 2 0 ","4 0 2 ","2 0 4 ","2 4 0 "
        ,"0 2 4 ","0 4 2 "
    }; 
    for(int i=50,j=0 ; j<=50 ; --i,++j)
    { 
        colorgrey = 50-i;
        sprintf(colorstrgrey[j],"%d %d %d ",colorgrey,colorgrey,colorgrey);
    }
    sprintf(filename,"newton_convergence_x%d.ppm",degree);
    
    struct timespec tbegin, tend;
    double timeelapsed;
    int count = 1;
    printf("BEGIN\n");
    //Method 1

    //timespec_get(&tbegin,TIME_UTC);
    //do{
    fcolor=fopen(filename,"w");
    fprintf(fcolor, "P3\n%d %d\n7\n", re, lines);
    for(int i=0,j=0,k=0;i<lines;++i)
    {
        flag = attr2[i];
        k=0;
        do
        {
            colorstring[j++] = colorstr[flag][k++];
        }while(colorstr[flag][k]!='\0');
    }
    len=strlen(colorstring);
    //for(int j=0;j<lines;++j)
    //{
        printf("%s\n",colorstring);
        fwrite(colorstring, len , 1 , fcolor);
        fwrite("\n",sizeof("\n"),1,fcolor);
    //}
    fclose(fcolor);
    //}while(++count<= SIZE);
    //timespec_get(&tend,TIME_UTC);
    //timeelapsed = (double)(tend.tv_sec - tbegin.tv_sec)+(double)(tend.tv_nsec - tbegin.tv_nsec)/B;
    //printf("Time taken for method 1 attractors is %.9lf\n",timeelapsed);

    count = 1;
    //Method 2
    /*
    timespec_get(&tbegin,TIME_UTC);
    do{
    fcolor=fopen(filename,"w");
    fprintf(fcolor, "P3\n%d %d\n7\n", re, lines);
    for(int i=0;i<lines;++i)
    {
      for(int j=0;j<lines;++j)
      {
        flag = attr2[j];
        //fwrite(colorstr[flag], strlen(colorstr[flag]) , 1 , fcolor);
        fwrite(colorstr[flag], 6 , 1 , fcolor);
      }
      fwrite("\n",sizeof("\n"),1,fcolor);
    }
    fclose(fcolor);
    }while(++count<= SIZE);
    timespec_get(&tend,TIME_UTC);
    timeelapsed = (double)(tend.tv_sec - tbegin.tv_sec)+(double)(tend.tv_nsec - tbegin.tv_nsec)/B;
    printf("Time taken for method 2 attractors is %.9lf\n",timeelapsed);
    */
    sprintf(filename,"newton_attractors_x%d.ppm",degree);
    
    count = 1;
    //Method 1

    //timespec_get(&tbegin,TIME_UTC);
    //do{
    fgrey=fopen(filename,"w");
    fprintf(fgrey, "P3\n%d %d\n50\n", re, lines);
    //for(int i=0;i<lines;++i)
    //{
      for(int j=0;j<lines;++j)
      {
          flag = conval2[j];
          fwrite(colorstrgrey[flag], strlen(colorstrgrey[flag]) , 1 , fgrey);
      }
      fwrite("\n",sizeof("\n"),1,fgrey);
    //}
    fclose(fgrey);
    //}while(++count<=SIZE);
    //timespec_get(&tend,TIME_UTC);
    //timeelapsed = (double)(tend.tv_sec - tbegin.tv_sec)+(double)(tend.tv_nsec - tbegin.tv_nsec)/B;
    //printf("Time taken for method 1 convergence is %.9lf\n",timeelapsed);

    count =1;
    //Method 2   
    /*
    timespec_get(&tbegin,TIME_UTC);
    do{
    for(int i=0,j=0,k=0;i<lines;++i)
    {
        flag = conval2[i];
        k=0;
        do
        {
            greystring[j++] = colorstrgrey[flag][k++];
        }while(colorstrgrey[flag][k]!='\0');
    }
    l = strlen(greystring);
    //greystring[l+1] = '\n';
    fgrey=fopen(filename,"w");
    fprintf(fgrey, "P3\n%d %d\n50\n", re, lines);
    for(int j=0;j<lines;++j)
    {
        fwrite(greystring, l , 1 , fgrey);
        fwrite("\n",sizeof("\n"),1,fgrey);
    }
    fclose(fgrey);
    }while(++count<= SIZE);
    timespec_get(&tend,TIME_UTC);
    timeelapsed = (double)(tend.tv_sec - tbegin.tv_sec)+(double)(tend.tv_nsec - tbegin.tv_nsec)/B;
    printf("Time taken for method 2 convergence is %.9lf\n",timeelapsed);

    count = 1;
    //Method 3

    timespec_get(&tbegin,TIME_UTC);
    do{
    for(int i=0,j=0;i<lines;++i)
    {
        greystring[j++] = conval2[i];
        greystring[j++] = ' ';
        greystring[j++] = conval2[i];
        greystring[j++] = ' ';
        greystring[j++] = conval2[i];
        greystring[j++] = ' ';
    }
    fgrey=fopen(filename,"w");
    fprintf(fgrey, "P3\n%d %d\n50\n", re, lines);
    l=strlen(greystring);
    for(int j=0;j<lines;++j)
    {
        fwrite(greystring, l , 1 , fgrey);
        fwrite("\n",sizeof("\n"),1,fgrey);
    }
    fclose(fgrey);
    }while(++count<=SIZE);
    timespec_get(&tend,TIME_UTC);
    timeelapsed = (double)(tend.tv_sec - tbegin.tv_sec)+(double)(tend.tv_nsec - tbegin.tv_nsec)/B;
    printf("Time taken for method 3 convergence is %.9lf\n",timeelapsed);
    */
   pthread_mutex_unlock(&item_done_mutex);
}

void static inline analyse_parsing(int argc1,const char *argv1[])
{
    int index,iter=0;
    char * args;
    while((index=getopt(argc1,(char* const*)argv1,"-t:-l::"))!=-1)
    {      
        ++iter;
        if(index=='?')
            printf("Invalid arguement/No value was passed for %c\n",optopt);
        else if(index=='t')
        {
            if(optarg[0]=='-')
            {
                printf("No value is passed for threads\n");
            }
            else
            {
                args = optarg;
                n_threads = strtol(args,(char ** restrict)argv1,10);
                //printf("The number of threads is %d\n",n_threads);
            }
        }
        else if(index=='l')
        {
            if(optarg[0]=='-')
            {
                printf("No value is passed for lines\n");
            }
            else
            {
                args = optarg;
                lines = strtol(args,(char ** restrict)argv1,10);
                //printf("The no of lines is %d\n",lines);
            }
        }
        else
        {
            
            if(!optarg[0])
            {
                printf("No value is passed for degree\n");
            }
            else
            {
                args = optarg;
                degree = strtol(args,(char ** restrict)argv1,10);
                //printf("The value of degree is %d\n",degree);
            }
        }
    }
    if(iter==0)
    {
        printf("No value is passed for threads, degree and lines\n");
    }
}

