#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void fun_polar(double * x0re, double * x0im, int  d);
void calc_roots(double** cplx_roots, int d);

char * convergences;
char * attractors;
char * item_done;
int main(){

  int d=7;
  int l=10; // number of rows and columns;
  
   /* allocate memory for all exact roots, should not be in newtonmethod later */    
  double *  ptr_roots= (double*) malloc(sizeof(double) * (d*2));
  double ** cplx_roots = (double**) malloc(sizeof(double*) * d);
  for ( size_t ix = 0, jx = 0; ix < d; ++ix, jx+=2)
    {
      cplx_roots[ix] = ptr_roots + jx;
    }
  /* the true roots */
  calc_roots(cplx_roots, d);
  
  /* allocate memory for all initialvalues of x, this should happen within every thread?*/ 
 
  double * ptr_xitr= (double*) malloc(sizeof(double)*(l*2));
  double ** init_row = (double**) malloc(sizeof(double*)*l);
    
      /*the actual newtonmethod  */
  // initialize how complex values should be stored //
  for ( size_t ix = 0, jx = 0; ix < l; ++ix, jx+=2)
    {
      init_row[ix]=ptr_xitr+jx;
    }
  //  for (size_t kx=start; kx<end(maybe(lines)); kx+=numthread){  an idea for  
      /* initial values of x0*/
  for (size_t ix=0; ix < l; ++ix)
    {
      init_row[ix][0]=-2 + 4*(double)ix/l;
      init_row[ix][1]=0.03;
     //  printf("re:%f im: %f\n", init_row[ix][0], init_row[ix][1]);
    }
  
  /* the newton iteration */
  
  char maxiter = 50;  
  char* attr =(char*) malloc( sizeof(char)*l);
  char* conval=(char*) calloc(l, sizeof(char));  
  for (size_t jx=0; jx < l; ++jx)
    {
      //int conval = 0;
      attr[jx]=-1;
      unsigned short int conv_loc = 0;
      for ( conv_loc=0; ; ++conv_loc)
	{
	  double xsquare = init_row[jx][0]*init_row[jx][0] + init_row[jx][1]*init_row[jx][1]; // useful computattion
	  if (xsquare < 10e-6 || init_row[jx][0] > 10e10 || init_row[jx][0] <-1*10e10 || init_row[jx][1] > 10e10 || init_row[jx][1]<-1*10e10)   
	    {
	      conval[jx]= maxiter; // assign maximum no if iterations 
	      attr[jx]=(char)d;
	      printf("No convergence, conval set to %hhud , attr set to %hhud \n ", conval[jx] , attr[jx]);
	      break;
	    }
	  if (xsquare < 1.003) //1.002001 is minimal value)
	    {
	      for (int ix=0; ix < d; ++ix)
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
	   
	  if (attr[jx] != -1) {
	    printf("hey conv: %hhud , attr: %hhud ,real:%f, im: %f \n ",conval[jx], attr[jx] ,init_row[jx][0],init_row[jx][1]);
	    break;
	  }
	  
	  fun_polar(&init_row[jx][0],&init_row[jx][1],d); 
	}
      if (conv_loc > 50) // or maxiter
	{conv_loc=50;	  
	}
      conval[jx] = (char)conv_loc;
    }


  //  FOR LATER attractors[kx]=attr;
  //  /* conversion table /*

   for (size_t ix=0; ix<l ; ++ix)
   {
    printf("conv %hhi , attr %hhi \n", conval[ix], attr[ix]);
  }

  free(conval);
  free(attr);
  free(init_row);
  free(cplx_roots);
  free(ptr_xitr);
  free(ptr_roots);
  return 0;
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
  

void calc_roots(double** cplx_roots, int d){                                                                                                                                             
  double twopi=6.283185;
  cplx_roots[0][0]=1.0; // always one real root                                                                                                                                         
  cplx_roots[0][1]=0.0;
  
  for (size_t ix=1; ix < d; ++ix){
    cplx_roots[ix][0]= cos ((double)ix*twopi/d);
    cplx_roots[ix][1]= sin ((double)ix*twopi/d);
  }
}


  // if d is 1, then newton always converges to 1+ 0i in one iteration, super simple
  //all roots are on unit circle, can we use this in determining if more iterations are necessary? how?

      
  
