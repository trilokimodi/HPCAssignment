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
char * convergences;
char * attractors;
char * item_done;
#define re lines
#define im lines

void main(int argc,const char *argv[])
{
    analyse_parsing(argc,argv);
    float *  ptr_roots= (float*) malloc(sizeof(float) * (degree*2));
    float ** cplx_roots = (float**) malloc(sizeof(float*) * degree);
    for( size_t ix = 0, jx = 0; ix < degree; ++ix, jx+=2)
        {
            cplx_roots[ix] = ptr_roots + jx;     
        }
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
	                    //printf("No convergence, conval set to %hhud , attr set to %hhud \n ", conval[jx] , attr[jx]);
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
		                            //printf("conv_loc %d ", conv_loc);// poses problems if conval_loc >250
		                            break;
		                        }
		                }   
	                }
	            if (attr[jx] != -1) 
                    {
	                    //printf("hey conv: %hhud , attr: %hhud ,real:%f, im: %f \n ",conval[jx], attr[jx] ,init_row[jx][0],init_row[jx][1]);
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
        //printf("conv %hhi , attr %hhi \n", conval[ix], attr[ix]);
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

void writingfile(char * conval2, char * attr2)
{
    int flag = degree, colorgrey;
    char filename[100];
    char colorstrgrey[50][13];
    char* colorstring = (char*)malloc(sizeof(char)*lines*9);
    FILE *fcolor, *fgrey;
    char colorstr[12][13] = 
    {
        "4 0 0 ","0 4 0 ","0 0 4 ","3 3 0 ","0 5 5 ","3 0 5 ","4 2 0 ","4 0 2 ","2 0 4 ","2 4 0 "
        ,"0 2 4 ","0 4 2 "
    }; 
    for(int i=50,j=0 ; j<50 ; --i,++j)
    { 
        colorgrey = 50-i;
        sprintf(colorstrgrey[j],"%d %d %d ",colorgrey,colorgrey,colorgrey);
    }
    for(int i=0,j=0;i<lines;++i)
    {
        flag = conval2[i];
        colorstring[j] = *(colorstrgrey[flag]);
        if(flag<10)
        j+=6;
        else
        j+=9;        
    }
    int l = strlen(colorstring);
    colorstring[l+1] = '\n';
    sprintf(filename,"newton_convergence_x%d.ppm",degree);
    fcolor=fopen(filename,"w");
    //fprintf(fcolor, "P3\n%d %d\n255\n", re, lines);
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
    sprintf(filename,"newton_attractors_x%d.ppm",degree);
    fgrey=fopen(filename,"w");
    /*fprintf(fgrey, "P3\n%d %d\n255\n", re, lines);
    for(int i=0;i<lines;++i)
    {
      for(int j=0;j<lines;++j)
      {
          flag = conval2[j];
          fwrite(colorstrgrey[flag], strlen(colorstrgrey[flag]) , 1 , fgrey);
      }
      fwrite("\n",sizeof("\n"),1,fgrey);
    }*/
    fprintf(fgrey, "P3\n%d %d\n50\n", re, lines);
    for(int j=0;j<lines;++j)
    {
        fwrite(colorstrgrey, l+2 , 1 , fgrey);
    }
    fclose(fgrey);

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
