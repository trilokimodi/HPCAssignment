#include<stdlib.h>
#include<stdio.h>

void mul_cpx_separatefile(double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im)
{
    *c_re = (*a_re)*(*b_re) - (*a_im)*(*b_im);
    *c_im = (*a_re)*(*b_im) + (*a_im)*(*b_re);
}