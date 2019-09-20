#include<stdio.h>
#include<stdlib.h>
#define s1 5
#define s2 5


void main()
{
    double * sums = (double*)malloc(sizeof(double)*s1*s2);
    double ** matrix, ** matrix2;
    double matrix1[s1][s2];
    matrix = (double **)(malloc(sizeof(double *)*s2));
    matrix2 = (double **)(malloc(sizeof(double *)*s2));
    for(size_t i = 0,j=0; i < s1 ; ++i,j+=s2)
        {
            matrix[i] = (double*)malloc(sizeof(double)*s1);
            matrix2 = sums + j;
        }
    printf("Ignoring mem frag\n");
    for(int i =0; i<s1;++i)
        {
            for(int j = 0;j<s2;++j)
                {
                    matrix[i][j]=i+j;
                    printf("%p\t",&matrix[i][j]);
                }
            printf("\n");
        }
    printf("with mem frag\n");
    for(int i =0; i<s1;++i)
        {
            /*for(int j = 0;j<s2;++j)
                {
                    matrix2[i][j]=i+j;
                    printf("%p\t",&matrix2[i][j]);
                }
            printf("\n");*/
            printf("%p\t",&matrix2[i]);   
        }
    printf("\nAddress of sums is %p\n",&sums);    
    printf("Normal 2D array\n");
    for(int i =0; i<s1;++i)
        {
            for(int j = 0;j<s2;++j)
                {
                    matrix1[i][j]=i+j;  
                    printf("%p\t",&matrix1[i][j]);
                }
            printf("\n");
        }
}