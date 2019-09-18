#include<stdio.h>
#include<stdlib.h>
#define s 5
void mul_cpx_separatefile(double * a_re, double * a_im, double * b_re, double * b_im, double * c_re, double * c_im);
void main()
{
    double * as_re, * as_im, * bs_re, * bs_im, * cs_re, * cs_im;
    as_re = (double*)malloc(sizeof(double)*s);
    as_im = (double*)malloc(sizeof(double)*s);
    bs_re = (double*)malloc(sizeof(double)*s);
    bs_im = (double*)malloc(sizeof(double)*s);
    cs_re = (double*)malloc(sizeof(double)*s);
    cs_im = (double*)malloc(sizeof(double)*s);
    for(int i=0;i<s;++i)
    {
        as_re[i] = rand() % 100 + 1;
        as_im[i] = rand() % 100 + 1;
        bs_re[i] = rand() % 100 + 1;
        bs_im[i] = rand() % 100 + 1;
    }
    mul_cpx_separatefile(as_re,as_im,bs_re,bs_im,cs_re,cs_im);
    for(int i=0;i<s;++i)
        printf("A(%d) = %.2lf + %.2lf i\tB(%d) = %.2lf + %.2lf i\tC(%d) = %.2lf + %.2lf i\n",i,as_re[i],as_im[i],i,bs_re[i],bs_im[i],i,cs_re[i],cs_im[i]);
}