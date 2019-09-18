#include<stdio.h>
#include<stdlib.h>
#define SIZE 2
/*
void main()
{
  int ** arr = (int**) malloc((sizeof(int**)*SIZE));
  for(int i=0;i<SIZE;++i)
    {
      arr[i]=(int*) malloc(sizeof(int)*SIZE);
    }
  for(int i=0;i<SIZE;++i)
    {
      for(int j=0;j<SIZE;++j)
	{
	  arr[i][j]=0;
	}
    }
  for(int i=0;i<SIZE;++i)
    {
      free(arr[i]);
    }
  free(arr);
}
*/
void main()
{
  int * as = (int*) malloc(sizeof(int) * SIZE*SIZE);
  int ** arr = (int**) malloc(sizeof(int*) * SIZE);
  for(int i = 0, j = 0; i < SIZE; ++i, j+=SIZE )
    {
      arr[i] = as + j;
      printf("Print value of arr[%d],as[%d] = %d||%d\n",i,i,&arr[i],as[i]);
    }

  for(int i = 0; i < SIZE; ++i)
    {
      printf("\n");
      for(int j = 0; j < SIZE; ++j)
	{
	  arr[i][j]+=i+j;
	  printf("%d||%d\t",arr[i][j],&arr[i][j]);
	}
    }

      // sprintf("%d\n", arr[0][0]);

  free(arr);
  free(as);
  printf("\n%d\n",arr[9][9]);
} 
