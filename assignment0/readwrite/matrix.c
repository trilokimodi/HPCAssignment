#include<stdio.h>
#include<stdlib.h>
#define s 10
int matrix(int x,int y)
{
    if(x==0&&y==0)
    {
        
        int i=0,j;
        int** arr = (int**) malloc(sizeof(int*)*s);
        int * ar = (int *)malloc(sizeof(int)*s*s);
        //int arr2[10];
        for(int p=0,z=0;z<s;++z,p+=s)
        {
            //printf("\n as %d as %d arr2 %d", ar[p],&ar[p],arr2[z]);
            arr[z] = ar+p;
            //printf("\n arr\t%d, as\t%d\n",arr[z],&ar[p]);
        }
        //int arr[s][s];
        printf("\nOriginal array\n");
        do
        {
            j=0;
            do  
            {
                arr[i][j]=i+j;
                printf("%d\t",arr[i][j]);
            }while (++j<s);        
            printf("\n");
        }while(++i<s);
        /*for(int p=0,z=0;z<s;++z,p+=s)
        {
            arr[z] = ar+p;
            printf("\n arr\t%d, as\t%d\n",*arr[z],ar[p]);
        }*/
        //return arr[i][j];
        return x+y;
    }
    else
    {
        return x+y;
    }
}