#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define s 30000
#include <sys/time.h>   
#include <sys/resource.h>
void main()
{
    double * as_re, * as_im, * bs_re, * bs_im, * cs_re, * cs_im;
    as_re = (double*)malloc(sizeof(double)*s);
    as_im = (double*)malloc(sizeof(double)*s);
    bs_re = (double*)malloc(sizeof(double)*s);
    bs_im = (double*)malloc(sizeof(double)*s);
    cs_re = (double*)malloc(sizeof(double)*s);
    cs_im = (double*)malloc(sizeof(double)*s);
    struct timespec t1,t2;
    for(int i=0;i<s;++i)
    {
        as_re[i] = rand() % 100 + 1;
        as_im[i] = rand() % 100 + 1;
        bs_re[i] = rand() % 100 + 1;
        bs_im[i] = rand() % 100 + 1;
    }
    int count = 0;
    timespec_get(&t1,TIME_UTC);
    do
    {    
    for(int i=0;i<s;++i)
    {
        cs_re[i] = as_re[i]*bs_re[i] - as_im[i]*bs_im[i];
        cs_im[i] = as_re[i]*bs_im[i] + as_im[i]*bs_re[i];
        //printf("A(%d) = %.2lf + %.2lf i\tB(%d) = %.2lf + %.2lf i\tC(%d) = %.2lf + %.2lf i\n",i,as_re[i],as_im[i],i,bs_re[i],bs_im[i],i,cs_re[i],cs_im[i]);
    }
    }
    while(++count<10000);
    timespec_get(&t2,TIME_UTC);
    double time = ((double)(t2.tv_sec-t1.tv_sec) + (double)((double)(t2.tv_nsec - t1.tv_nsec)/1000000000)) / 10000;
    printf("Time involved by inline is %lf\n",time);
    //for(int i=0;i<5;++i)
    //printf("A(%d) = %.2lf + %.2lf i\tB(%d) = %.2lf + %.2lf i\tC(%d) = %.2lf + %.2lf i\n",i,as_re[i],as_im[i],i,bs_re[i],bs_im[i],i,cs_re[i],cs_im[i]);
}