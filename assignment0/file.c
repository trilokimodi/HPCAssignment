#include<stdio.h>
#include<stdlib.h>
struct aboutyou
{
    char name[20];
    int score;
    float perc;
};

void main()
{
    FILE *fptr;
    fptr = fopen("file.txt","w");
    struct aboutyou a[3];
    int i=0;
    printf("Enter the details of students:\n");
    do{
        printf("\nEnter name || score\n");
        fprintf(fptr,"%s\t%d\t%f\n",(scanf("%s",&a[i].name),a[i].name),(scanf("%d",&a[i].score),a[i].score),a[i].score/100);
        printf("%s\t%d",a[i].name,a[i].score);
    }while(++i<3);
    fptr=freopen("file.txt","r",fptr);
    for(i=0;i<3;++i)
    {
        fscanf(fptr,"%d",&a[i].score);
        printf("\npercentage is",a[i].score);
    }
    fclose(fptr);
}