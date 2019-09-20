#include<stdlib.h>
#include<stdio.h>
#define s 30000
void mul_cpx_separatefile(double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im)
{
    for(int i=0;i<s;++i)
    {
        c_re[i] = (a_re[i])*(b_re[i]) - (a_im[i])*(b_im[i]);
        c_im[i] = (a_re[i])*(b_im[i]) + (a_im[i])*(b_re[i]);
    }
    
}