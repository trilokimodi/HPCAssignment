#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include <sys/time.h>   
#include <sys/resource.h>
#define iter 1000
#define B 1000000000
#define n 1000000
#define m 1000
void benchmark1(int* p);
void benchmark2(int* p);
void benchmark1(int* p)
{
    int i=0;
    for(int j=0;j<m;++j)
    {
        for(int k=0;k<m;++k)
            p[j+m*k] = i++;
    }
}
void benchmark2(int *p)
{
    for(int i=0;i<n;++i)
        p[i]=i;
}

void main()
{
    int *x,*y,*p,a,i=0,j=0,k=0;   
    clock_t b1[iter],b2[iter],b3[iter],bavg1=0,bavg2=0,bavg3=0;
    struct timespec b1start[iter],b1end[iter],b2start[iter],b2end[iter],b3start[iter],b3end[iter];
    double timeb1,timeb2,timeb3,b1avg,b2avg,b3avg;
    x = (int*)malloc(sizeof(int)*n);
    y = (int*)malloc(sizeof(int)*n);
    p = (int*)malloc(sizeof(int)*n);
    for(i=0;i<n;++i)
    {
        x[i] = i;
    }
    i=0;
    a = 10;
    for(k=0;k<iter;++k)
    {
        b1[k]=clock();       
        timespec_get(&b1start[k],TIME_UTC);
        benchmark1(p);
        for(i=0;i<n;++i)
        {
            j = p[i];
            y[j] += a*x[j];
        }
        timespec_get(&b1end[k],TIME_UTC);
        b1[k]=clock() - b1[k];

        i=0;j=0;
        
        b2[k]=clock();
        timespec_get(&b2start[k],TIME_UTC);
        benchmark2(p);
        for(i=0;i<n;++i)
        {
            j = p[i];
            y[j] += a*x[j];
        }
        timespec_get(&b2end[k],TIME_UTC);
        b2[k]=clock() - b2[k];
        
        i=0;j=0;

        b3[k]=clock();       
        timespec_get(&b3start[k],TIME_UTC);
        benchmark2(p);
        for(i=0;i<n;++i)
        {
            y[i] += a*x[i];
        }
        timespec_get(&b3end[k],TIME_UTC);
        b3[k]=clock() - b3[k];
        i=0;j=0;
    }
    for(k=0;k<iter;++k)
    {
        bavg1+=b1[k];
        bavg2+=b2[k];
        bavg3+=b3[k];
        timeb1+= (double)(double)(b1end[k].tv_sec - b1start[k].tv_sec)+(double)((double)(b1end[k].tv_nsec - b1start[k].tv_nsec)/B);
        timeb2+= (double)(double)(b2end[k].tv_sec - b2start[k].tv_sec)+(double)((double)(b2end[k].tv_nsec - b2start[k].tv_nsec)/B);
        timeb3+= (double)(double)(b3end[k].tv_sec - b3start[k].tv_sec)+(double)((double)(b3end[k].tv_nsec - b3start[k].tv_nsec)/B);
    }
    b1avg = timeb1/iter;
    b2avg = timeb2/iter;
    b3avg = timeb3/iter;
    double timebc1 = (double)(bavg1)/CLOCKS_PER_SEC;
    double timebc2 = (double)(bavg2)/CLOCKS_PER_SEC;
    double timebc3 = (double)(bavg3)/CLOCKS_PER_SEC;
    //printf("\nTime taken by benchmark-1 by clock is %lf miliseconds\n",timebc1);
    printf("\nTime taken by benchmark-1 by timespec_get is %lf\n",b1avg);
    //printf("\nTime taken by benchmark-2 by clock is %lf miliseconds\n",timebc2);
    printf("\nTime taken by benchmark-2 by timespec_get is %lf\n",b2avg);
    //printf("\nTime taken by benchmark-3 is by clock %lf miliseconds\n",timebc3);
    printf("\nTime taken by benchmark-3 by timespec_get is %lf\n",b3avg);
}