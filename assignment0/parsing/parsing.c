#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<string.h>
#include <unistd.h>
void main(int argc,const char *argv[])
{
    int index,iter=0;
    char * args;
    long int val;
    while((index=getopt(argc,argv,"-a:-b:"))!=-1)
    {
      // To get a better understanding of iterations, optarg and argv values. 
      /*printf("\nIteration no %d, argc = %d\n",++iter,argc);
        for(int i=0;i<argc;++i)
        {
            printf("\noptarg is %s \t argv is %s",optarg,argv[i]);
        }*/
      
        ++iter;
        if(index=='?')
            printf("Invalid arguement/No value was passed for %c\n",optopt);
        else if(index=='a')
        {
            //printf("\nThe value of optargs is %s",optarg);
            if(optarg[0]=='-')
            {
                //index = ':';
                printf("No value is passed for a\n");
                //continue;
            }
            else
            {
                args = optarg;
                //printf("\nThe value of args is %s",args);
                val = strtol(args,argv,10);
                printf("The value of a is %d\n",val);
            }
        }
        else if(index=='b')
        {
            if(optarg[0]=='-')
            {
                //index = ':';
                printf("No value is passed for b\n");
            }
            else
            {
                args = optarg;
                //printf("\nThe value of args is %s",args);
                val = strtol(args,argv,10);
                printf("The value of b is %d\n",val);
            }
        }
    }
    if(iter==0)
    {
        printf("No value is passed for both a and b\n");
    }
}
