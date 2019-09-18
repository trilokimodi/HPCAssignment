#include<stdlib.h>
#include<stdio.h>
#include<time.h>
int matrix(int x, int y);
#define s 10
void main()
{
    FILE *f;
    int i=0,j,flag=0,check=0;
    clock_t t;
    t=clock();
    f = fopen("matrix.txt","r");
    do
    {
        j=0;
        do
        {
            fscanf(f,"%d",&check);
            flag=matrix(i,j);
            if(flag!=check)
            printf("\nMismatch found at location %d,%d. Original value = %d. Value in matrix.txt file = %d\n",i,j,flag,check);
        } while (++j<s);
    } while (++i<s);
    fclose(f);
    t=clock() - t;
    double time = (double)(t)/CLOCKS_PER_SEC;
    printf("The time taken by do while loop to read the file is %f mili seconds\n",time*1000);
}