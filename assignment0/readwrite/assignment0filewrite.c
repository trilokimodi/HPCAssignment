#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define s 10
void matrix(int** fptr);
void main()
{
    FILE *f;
    int i=0,j=0;
    clock_t t;
    t=clock();
    f = fopen("matrix.txt","w");
    do
    {
        j=0;
        do
        {
            if(i==j)
            fprintf(f,"%d ",i*j);
            else
            fprintf(f,"%d ",i+j);
        } while (++j<s);
        fprintf(f,"\n");       
    } while (++i<s);
    fclose(f);
    t=clock() - t;
    double time = (double)(t)/CLOCKS_PER_SEC;
    printf("The time taken by do while loop to write in the file is %f mili seconds\n",time*1000);
}