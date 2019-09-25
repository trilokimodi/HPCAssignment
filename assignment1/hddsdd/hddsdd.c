#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define iter 1000
void main()
{
    FILE *fp;
    int count = 0;
    clock_t thdd[iter],tavg;
    for(int j=0;j<iter;++j)
    {
        thdd[j] = clock();
        fp = fopen("HDDSDD.bin","w");
        for(int i=1;i<=pow(2,20);++i)
            fwrite(&i,sizeof(int),1,fp);
        fclose(fp);
        fp = fopen("HDDSDD.bin","r");
        for(int i=1;i<=pow(2,20);)
            {
                fread(&count,sizeof(int),1,fp);
                ++i;
            }
        fclose(fp);
        thdd[j]=clock() - thdd[j];
    }
    for(int i=0;i<iter;++i)
        tavg += thdd[i];
    tavg = tavg/iter;
    double time = (double)(tavg)/CLOCKS_PER_SEC;
    printf("\nTime taken to write/read in hdd is %lf miliseconds\n",time*1000);
}