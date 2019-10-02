#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define i_m 1000
double a[i_m], b[i_m], sum;

#define n_threads 4
pthread_mutex_t mutex_sum;

// assuming that i_m is divisible by n_threads
#define block_size (i_m / n_threads)

void* dotprod(void* arg);

int
main()
{
    int ret;
    size_t i, j;
    pthread_t threads[n_threads];

    for (i=0; i < i_m; ++i)
        a[i] = b[i] = 2;
    sum = 0;

  // thread creation and joining

    pthread_mutex_init(&mutex_sum, NULL);
    for (j=0, i=0; j < n_threads; ++j, i+=block_size) 
    {
        double ** arg = malloc(2*sizeof(double*));
        arg[0] = a+i; 
        arg[1] = b+i;
        if (ret = pthread_create(threads+j, NULL, dotprod, (void*)arg)) 
        {
            printf("Error creating thread: \n", ret);
            exit(1);
        }
    }

    for (j=0; j < n_threads; ++j) 
    {
        if (ret = pthread_join(threads[j], NULL)) 
        {
            printf("Error joining thread: %d\n", ret);
            exit(1);
        }
    }
    pthread_mutex_destroy(&mutex_sum);
    printf("sum = %f\n", sum);
}
void* dotprod(void *restrict arg) {
  double * a_loc = ((double**)arg)[0];
  double * b_loc = ((double**)arg)[1];
  free(arg);

  double sum_loc = 0;
  for (size_t i=0; i < block_size; ++i)
    sum_loc += a_loc[i]*b_loc[i];

  pthread_mutex_lock(&mutex_sum);
  sum += sum_loc;
  pthread_mutex_unlock(&mutex_sum);

  return NULL;
}