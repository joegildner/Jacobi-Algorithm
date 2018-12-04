#include <stdlib.h>
#include <pthread.h>
typedef struct barrier{
  int thread_count;
  pthread_mutex_t* m;
  pthread_cond_t allHere;
  pthread_cond_t allGone;
  int total;
  int here;
  int leaving;
}barrier;

barrier* barrier_new(int count);

void barrier_enter(barrier* b, int thd);
