#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void calc_roots(float** cplx_roots, int d);
int main()
{
  int d=5;
  float *  ptr_roots= (float*) malloc(sizeof(float) * (d*2));
  float ** cplx_roots = (float**) malloc(sizeof(float*) * d);
  for ( size_t ix = 0, jx = 0; ix < d; ++ix, jx+=2){
    cplx_roots[ix] = ptr_roots + jx;      
  }
  calc_roots(cplx_roots,d);
  // void calc_roots(float** cplx_roots, int d){
  /*float twopi=6.283185;
  cplx_roots[0][0]=1.0;
  cplx_roots[0][1]=0.0;
  for (size_t ix=1; ix < d; ++ix)
    {
      cplx_roots[ix][0]= cos ((float)ix*twopi/d);
      cplx_roots[ix][1]= sin ((float)ix*twopi/d);
    }
  */
  for (size_t ix=0; ix<d; ++ix){
    printf("real %f,im  %f \n", cplx_roots[ix][0], cplx_roots[ix][1]);
   }
  return 0;
}

void calc_roots(float** cplx_roots, int d){                                                                                                                                                                      float twopi=6.283185;
  cplx_roots[0][0]=1.0; // always one real root
  cplx_roots[0][1]=0.0;
  for (size_t ix=1; ix < d; ++ix){
    cplx_roots[ix][0]= cos ((float)ix*twopi/d);
    cplx_roots[ix][1]= sin ((float)ix*twopi/d);
  }
}
