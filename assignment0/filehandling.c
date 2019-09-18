#include<stdio.h>
#include<stdlib.h>
void main()
{
  FILE *fptr;
  fptr = fopen("textfileassignment0.txt","w");
 printf("\nsave nos\n");
 int i,n[5];
 n[0]=5;
 for(i=0;i<5;++i)
   {
     n[i]=i+3;
     fprintf(fptr,"%d",n[i]);
   }
 fptr = freopen("textfileassignment0.txt","r",fptr);
 do{
   n[--i]=fgetc(fptr);
   if(feof(fptr))
     break;
   printf("%c\t",n[i]);
 }while(i>0);
 int j = fclose(fptr);
 printf("%d\t%c",j,j);
}
