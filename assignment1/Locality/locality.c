#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define s1 1000
#define s2 1000
#define B 1000000000
#define iter 10000
double* row_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs);
double* col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs);

void main()
{
    struct timespec tg1, tg2;
    double * rsums,* csums;
    double ** matrix = (double**)malloc(sizeof(double*)*s1);
    double *cmem = (double*)malloc(sizeof(double)*s1*s2);
    for(int i=0,j=0;i<s1;++i,j+=s2)
        matrix[i] = cmem + j;               //keeping contiguous memory

    rsums = (double*)(malloc(sizeof(double)*s1));
    csums = (double*)(malloc(sizeof(double)*s1));
    
    for(int i=0;i<s1;++i)
    {
        for(int j=0;j<s2;++j)
        {
            matrix[i][j]=rand()%100 + 1;
        }
    }
    
    timespec_get(&tg1,TIME_UTC);                                 // start of time
    for(int i=0;i<iter;++i)
        rsums = row_sums(rsums,(const double**)matrix,s1,s2);   //row sum called and iterated 10 times
    timespec_get(&tg2,TIME_UTC);                                //end of time
    double timeelapsed = (double)((double)(tg2.tv_sec - tg1.tv_sec)/iter)+(double)((double)(tg2.tv_nsec - tg1.tv_nsec)/B/iter);
    printf("Time taken for row sums by timespec_get is %lf\n",timeelapsed);

    timespec_get(&tg1,TIME_UTC);                                //start of time
    for(int i=0; i<iter;++i)
        csums = col_sums(csums,(const double**)matrix,s1,s2);   //col sum called and iterated 10 times.
    timespec_get(&tg2,TIME_UTC);                                //end of time
    timeelapsed = (double)((double)(tg2.tv_sec - tg1.tv_sec)/iter)+(double)((double)(tg2.tv_nsec - tg1.tv_nsec)/B/iter);
    printf("Time taken for col sums by  timespec_get is %lf\n",timeelapsed);

    printf("first 5 Row sums array\n");
    for(int i=0;i<5;++i)
        printf("%.2lf\t",rsums[i]);
    printf("\n");

    printf("first 5 column sum array\n");
    for(int i=0;i<5;++i)
        printf("%.2lf\t",csums[i]);
    printf("\n");

    free(matrix);
    free(rsums);
    free(csums);
    free(cmem);
}

double* row_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
    for(size_t i = 0; i < nrs ; ++i)
    {
        double sum = 0;
        for(size_t j = 0; j < ncs; ++j)
            sum += matrix[i][j];
        sums[i] = sum;
    }
    return sums;
}

double* col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
    for(size_t i = 0; i < ncs ; ++i)
    {
        double sum = 0;
        for(size_t j = 0; j < nrs; ++j)
            sum += matrix[j][i];
        sums[i] = sum;
    }
    return sums;
}
//fastest
/*
double* col_sums(double * sums, const double ** matrix, size_t nrs, size_t ncs)
{
    for(int i=0;i<ncs;++i)
    sums[i] = 0;
    for(size_t i = 0; i < nrs ; ++i)
    {
        for(size_t j = 0; j < ncs; ++j)
            sums[j] += matrix[i][j];
    }
    return sums;
}*/
