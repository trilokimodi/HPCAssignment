#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define num_lines 10000

void main()
    {
        int size=1000;
        int coordinate[num_lines][3] = { 1 };
        int dif1,dif2,dif3;
        float dist;
        struct timespec tg1, tg2;
        timespec_get(&tg1,TIME_UTC);
        do{
        for (size_t ix=0; ix < num_lines ;++ix)
	    {
	      int t1 = coordinate[ix][0];
          int t2 = coordinate[ix][2];
          int t3 = coordinate[ix][2];
          for(size_t jx= ix+1; jx < num_lines; ++jx)
	      {
	        //set 1 of calc
            /*dif1 = coordinate[ix][0]-coordinate[jx][0];
	        dif2 = coordinate[ix][1]-coordinate[jx][1];
	        dif3 = coordinate[ix][2]-coordinate[jx][2];*/
            //set 2 of calc
            dif1 = t1-coordinate[jx][0];
	        dif2 = t2-coordinate[jx][1];
	        dif3 = t3-coordinate[jx][2];
	        dist = sqrtf((float)(dif1*dif1 + dif2*dif2 + dif3*dif3));
	      }
	    }
        }while (--size>0);        
        timespec_get(&tg2,TIME_UTC);
        double timeelapsed = (double)((double)(tg2.tv_sec - tg1.tv_sec))+(double)((double)(tg2.tv_nsec - tg1.tv_nsec)/1000000000);
        printf("Time taken for one single iteration by timespec_get is %lf\n",timeelapsed);
    }