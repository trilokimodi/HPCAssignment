#include<stdio.h>
#include<stdlib.h>
void main()
{
    int *as = NULL;
    int sum = 0;
    for(int i=0;i<10;++i)
        as[i]=i;
    for(int i=0;i<10;++i)
        sum+=as[i];
    free(as);
}