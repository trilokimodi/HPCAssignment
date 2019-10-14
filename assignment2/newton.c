#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<stddef.h>
#include<time.h>
#include<math.h>

void static inline analyse_parsing(int argc1,const char *argv1[]);
void perform_iteration(double * x0re, double * x0im, double xsquare);
void calc_roots(double** roots, int d);
void* newton_main(void* args);
void* write_main( void * args);

static int degree, n_threads, lines;
pthread_mutex_t row_done_mutex;
double ** roots;
char ** convergences;
char ** attractors;
char * row_done;

#define d degree
#define l lines

void main(int argc,const char *argv[])
{
    analyse_parsing(argc,argv);
    pthread_t *compute_threads = (pthread_t *)malloc(sizeof(pthread_t) * n_threads);
    pthread_t write_thread;

    // Allocate memory for global attactors and convergences
    convergences = (char**) malloc(sizeof(char* ) *l);
    attractors= (char**) malloc(sizeof(char* )* l);
    row_done = (char*) calloc(l,sizeof(char));

    // Allocate memory for all exact roots
    double * roots_entries = (double*) malloc(sizeof(double) * (d*2));
    roots = (double**) malloc(sizeof(double*) * d);
    for ( size_t ix = 0, jx = 0; ix < d; ++ix, jx+=2)
        roots[ix] = roots_entries + jx;
    
    // Calculate exact roots
    calc_roots(roots, d);

    // Create threads
    for (size_t i = 0; i < n_threads; ++i)
    {
        size_t* args = malloc(sizeof(size_t));
        *args = i;
        pthread_create(&compute_threads[i], NULL, newton_main, (void*) args);
    }

    pthread_create(&write_thread, NULL, write_main, NULL);

    
    // Join threads
    for (size_t i = 0; i < n_threads; ++i)
        pthread_join(compute_threads[i], NULL);

    pthread_join(write_thread,NULL);

    // Finish up, go home
    pthread_mutex_destroy(&row_done_mutex);
  
    // Free everything on the heap
    free(roots);
    free(roots_entries);
    free(compute_threads);
    free(convergences);
    free(attractors);
    free(row_done);
}

// Performs computing
void* newton_main(void* args)
{
    if (d==1)
    {
        for (int row=0;row<l;++row)
	    {
            char * attractor = (char*) calloc(l, sizeof(char)); //it is not globally allocated now
            char * convergence= (char*) malloc (sizeof(char)*l); //it is not globally allocated now  	
            for (int ix=0; ix<l; ++ix)
                convergence[ix]=1;
        
            convergences[row] = convergence;
            attractors[row]= attractor;
        
            pthread_mutex_lock(&row_done_mutex);
            row_done[row] = 1;
            pthread_mutex_unlock(&row_done_mutex);
	    }
    }

    else 
    {
        size_t offset = *((size_t*) args);
        free(args);

        // Allocate memory for all initial values of x, this should happen once within every thread      
        double * x0_line_entries = (double*) malloc(sizeof(double)*(l*2));
        double ** x0_line = (double**) malloc(sizeof(double*)*l); // Contains x0 values for one row, all columns
        for ( size_t ix = 0, jx = 0; ix < l; ++ix, jx+=2)
            x0_line[ix] = x0_line_entries + jx;
        
        // For each row
        for ( size_t row = offset; row < l   ; row += n_threads )
        {
            double imagpart = -2 + 4 * (double)row / l;
            char maxiter = 50;

            // Initialise x0_line
            for (size_t col = 0; col < l; ++col)
            {
                x0_line[col][0] = -2 + 4 * (double)col / l;
                x0_line[col][1] = imagpart;
            }

            // Allocate memory for attractor/convergence
            char * attractor =(char*) calloc(l, sizeof(char)); //it is globally allocated now
            char * convergence=(char*) calloc(l, sizeof(char)); //it is globally allocated now  

            // For x0 in row
            for (size_t jx=0; jx < l; ++jx)
            {
                attractor[jx] = -1;
                unsigned short int k = 0;
            
                // For iteration step k
                for (; ; ++k) // add limit to k for debugging
                {
                    double xsquare = x0_line[jx][0]*x0_line[jx][0] + x0_line[jx][1]*x0_line[jx][1]; // useful computattion
                
                    // If x_k really far away
                    if (xsquare < 0.000001 || x0_line[jx][0] > 10000000000 || x0_line[jx][0] < -10000000000 || x0_line[jx][1] > 10000000000 || x0_line[jx][1] < -10000000000)   
	                {
	                    convergence[jx] = maxiter; // assign maximum no if iterations 
	                    attractor[jx]=(char)d;
	                    break;
	                }
	
	                // If x_k^2 close enough to the origin, we bother checking convergence
	                if (xsquare < 1.0021) //1.002001 is minimal value)
	                {
	                    // For each exact root, with index ix
	                    for (int ix=0; ix < d; ++ix)
	                    {
	                        // If distance from root < tol, we are done
	                        if ((roots[ix][0]-x0_line[jx][0])*(roots[ix][0]-x0_line[jx][0]) + ((roots[ix][1]-x0_line[jx][1])*(roots[ix][1]-x0_line[jx][1])) < 0.000001)
	                        {
	                            attractor[jx] = (char)ix;
	                            break;
	                        }
	                    }
	                }

           	    // If convergence was reached, stop iteration
           	    if (attractor[jx] != -1)
                    break;
           	     
           	    // Calculates x_(k+1)
           	    perform_iteration(&x0_line[jx][0],&x0_line[jx][1],xsquare); 
           	    } // End end iteration for x0

                // If convergence isn't met before 50, say that it converged at 50
                if (k > 50) 
                    k = 50;	  

                convergence[jx] = (char)k;    
            } // End row
        
            // Set convergence and attractor value for row
            convergences[row] = convergence;
            attractors[row]= attractor;

            pthread_mutex_lock(&row_done_mutex);
            row_done[row] = 1;
            pthread_mutex_unlock(&row_done_mutex);
        } // End for all rows
        free(x0_line);
        free(x0_line_entries);
    } // else ending
    return NULL;
} // End function newton_main

// Calculates x_(k+1)
void perform_iteration(double * x0re, double * x0im, double xsquare)
{    
    if (degree == 5)
    { 
        double realp= *x0re;
        double resq = (*x0re) * (*x0re);
        double imsq = (*x0im) * (*x0im);
        double invsquare= (double)1.0/(xsquare);
        double divisor2 = (double)0.2*invsquare*invsquare*invsquare*invsquare;
        *x0im = (*x0im *((double)0.8))- ((double)4*(realp)* (*x0im)* (resq-imsq) * divisor2);
        *x0re = (*x0re *((double)0.8))+ ((xsquare * xsquare - (double)8 * resq * imsq) * divisor2);
    }

    else if(degree == 7)
    {
        double resq = *x0re*(*x0re);
        double imsq = *x0im*(*x0im);
        double divisor= 1.0/7;
        double invsquare= 1.0/(resq+ imsq);
        double divisor2 = divisor * invsquare * invsquare * invsquare * invsquare * invsquare * invsquare;
        *x0im = *x0im*(1-divisor) - (2*(*x0re)*(*x0im)*(3*resq*resq - 10*resq*imsq + 3*imsq*imsq))*divisor2;
        *x0re = *x0re*(1-divisor) + (resq*resq*resq + 15*resq*imsq*(imsq-resq) - imsq*imsq*imsq)*divisor2;
    }

    else if (degree == 2)
    {
        *x0re = *x0re*(0.5+0.5/xsquare);
	    *x0im = *x0im*(0.5-0.5/xsquare);
    }

}

void calc_roots(double** roots, int d)
{
    double twopi=6.2831853;
    roots[0][0]=1.0; // always one real root                                                                                                                                         
    roots[0][1]=0.0;
  
    for (size_t ix=1; ix < d; ++ix)
    {
        roots[ix][0]= cos ((double)ix*twopi/d);
        roots[ix][1]= sin ((double)ix*twopi/d);
    }
}

// Write to ppm files
void* write_main( void * args)
{
    struct timespec sleep_timespec;
    sleep_timespec.tv_sec = 0;
    sleep_timespec.tv_nsec = 250;
    int flag = degree, colorgrey, len;
    char filename[100];
    char colorstrgrey[51][10];
    char* greystring = (char*)malloc(sizeof(char)*lines*10);
    char* colorstring = (char*)malloc(sizeof(char)*lines*7);
    FILE *fcolor, *fgrey;

    // Colors used for attractors
    char colorstr[8][7] = 
    {
        "4 0 0 ","0 4 0 ","0 0 4 ","3 3 0 ","0 5 5 ","3 0 5 ","4 2 0 ","4 0 2 "
    };

    // Creates array of greyscale triplets of size 50 used for convergence
    for(int i=0 ; i <= 50 ; ++i)
    { 
        colorgrey = i;
        sprintf(colorstrgrey[i],"%2d %2d %2d ",colorgrey,colorgrey,colorgrey);
    }

    char * attractor; 
    char * convergence;
    char * row_done_loc = (char*)calloc(l, sizeof(char));
    
    // Open color ppm file for writing
    sprintf(filename,"newton_attractors_x%d.ppm",degree);
    fcolor = fopen(filename,"w");
    fprintf(fcolor, "P3\n%d %d\n7\n", lines, lines);

    // Open greyscale ppm file for writing
    sprintf(filename,"newton_convergence_x%d.ppm",degree);
    fgrey=fopen(filename,"w");
    fprintf(fgrey, "P3\n%d %d\n50\n", lines, lines);
    
    // Write to files line by line
    for ( size_t ix = 0; ix < l; )
    {
        pthread_mutex_lock(&row_done_mutex);
        if ( row_done[ix] != 0 )
	        memcpy(row_done_loc, row_done, l*sizeof(char));
        
        pthread_mutex_unlock(&row_done_mutex);
      
        if ( row_done_loc[ix] == 0 ) {
            nanosleep(&sleep_timespec, NULL);
            continue;
        }
       
        for ( ; ix < l && row_done_loc[ix] != 0; ++ix )
	    {
            convergence = convergences[ix]; 
	        attractor = attractors[ix];
            
            // For each x0, add convergence and attractor color triplet to array
            for(int i = 0;i<lines;++i)
            {
                memcpy(colorstring + i * 6,colorstr+attractor[i],6*sizeof(char));
                memcpy(greystring+(i*9),colorstrgrey+convergence[i],9*sizeof(char));
            }

            // Write array of color triplets to file
            fwrite(colorstring, strlen(colorstring), 1 , fcolor);
            fwrite("\n",1,1, fcolor);

            // Write array of greyscale triplets to file
            fwrite(greystring, strlen(greystring), 1 , fgrey);
            fwrite("\n",1,1,fgrey);

            // Freedom
            free(attractor);
            free(convergence);
        } 
    }
    free(row_done_loc);
    free(colorstring);
    free(greystring);
    
    // Close ppm files
    fclose(fcolor);
    fclose(fgrey);
}

// Take command line input
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
                printf("No value is passed for threads\n");

            else
            {
                args = optarg;
                n_threads = strtol(args,(char ** restrict)argv1,10);
            }
        }

        else if(index=='l')
        {
            if(optarg[0]=='-')
                printf("No value is passed for lines\n");

            else
            {
                args = optarg;
                lines = strtol(args,(char ** restrict)argv1,10);
            }
        }

        else
        {
            
            if(!optarg[0])
                printf("No value is passed for degree\n");

            else
            {
                args = optarg;
                degree = strtol(args,(char ** restrict)argv1,10);
            }
        }
    }
    if(iter==0)
        printf("No value is passed for threads, degree and lines\n");
}