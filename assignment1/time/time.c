#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>   
#include <sys/resource.h>
#define B 1000000000
#define SIZE 10
void main()
{
    struct timespec tg1, tg2;
    unsigned int i=1,j=0;
    unsigned long sum=0;
    timespec_get(&tg1,TIME_UTC);
    //clock_gettime(CLOCK_REALTIME, &start);
    do
    {
        i=1;
        sum=0;
        do
        {
            sum+=i;
        } while (++i<=B);
    } while (++j<SIZE);
    timespec_get(&tg2,TIME_UTC);
    //clock_gettime(CLOCK_REALTIME, &end);
    double timeelapsed = (double)((double)(tg2.tv_sec - tg1.tv_sec)/SIZE)+(double)((double)(tg2.tv_nsec - tg1.tv_nsec)/B/SIZE);
    printf("The sum of billion integers = %lu\n",sum);
    printf("Time taken for one single iteration by timespec_get is %lf\n",timeelapsed);
}