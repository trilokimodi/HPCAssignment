#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define iter 10
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
    int* x,*y,*p,a,i=0,j=0,k=0;
    clock_t b1[iter],b2[iter],bavg1,bavg2;
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
        benchmark1(p);
        for(i=0;i<n;++i)
        {
            j = p[i];
            y[j] += a*x[j];
        }
        i=0;j=0;
        b1[k]=clock() - b1[k];
        b2[k]=clock();
        benchmark2(p);
        for(i=0;i<n;++i)
        {
            j = p[i];
            y[j] += a*x[j];
        }
        b2[k]=clock() - b2[k];
    }
    for(k=0;k<iter;++k)
    {
        bavg1 += b1[k];
        bavg2 += b2[k];
    }
    bavg1 = bavg1/iter;
    bavg2 = bavg2/iter;
    double timeb1 = (double)(bavg1)/CLOCKS_PER_SEC;
    double timeb2 = (double)(bavg2)/CLOCKS_PER_SEC;
    printf("\nTime taken by benchmark-1 is %lf miliseconds\n",timeb1*1000);
    printf("\nTime taken by benchmark-2 is %lf miliseconds\n",timeb2*1000);
}