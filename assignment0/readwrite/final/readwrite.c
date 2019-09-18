#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define s 10
void main()
{
    
    clock_t t;
    t=clock();
    int ** matrix = (int**) malloc(sizeof(int*)*s);
    int * array;   
           
    //Non Contiguous Memory
    
    /*for(int i=0;i<s;++i)
        matrix[i]=(int*) malloc(sizeof(int)*s);
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
            matrix[i][j]=i+j;
   */ 

    //Contiguous memory
    array = (int*) malloc(sizeof(int)*s*s);
    for(int i=0,j=0;i<s;++i,j+=s)
    matrix[i]=array+j;
    for(int i=0;i<s;++i)
    for(int j=0;j<s;++j)
    matrix[i][j]=i+j;
    
    FILE * fptr;
    fptr=fopen("readwrite.bin","w");
    
    int count=0;
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
        {
            fwrite(&matrix[i][j],sizeof(matrix[i][j]),1,fptr);
        }
    fclose(fptr);
    t=clock() - t;
    double time = (double)(t)/CLOCKS_PER_SEC;
    printf("\nTime taken by to write in the file is %lf miliseconds\n\n",time*1000);
    
    //reading the file
    
    fptr=fopen("readwrite.bin","r");
    printf("Reading matrix from file\nMatrix || File \n");
    int arr[s][s];
    for(int i=0;i<s;++i)
        for(int j=0;j<s;++j)
            fread(&arr[i][j],sizeof(int),1,fptr);
    count=0;
    for(int i=0;i<s;++i)
        {
            for(int j=0;j<s;++j)
            {
                printf("%d||%d\t",matrix[i][j],arr[i][j]);
                if(arr[i][j]!=matrix[i][j])
                    ++count;
            }
            printf("\n");
        }
    if(count==0)
    printf("Both the matrix and file content are same\n");
    fclose(fptr);
    free(matrix);
    free(array);
}
