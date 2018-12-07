#ifndef BARRIER
#define BARRIER

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct barrier_t{
	//Barrier-Specific fields
  int thread_count;
  pthread_mutex_t* m;
  pthread_cond_t allHere;
  pthread_cond_t allGone;
  int total;
  int here;
  bool leaving;

  //Barrier processing data
  /*
  bool steadystate;
  double* thread_maxchange;
  double (*currentmat)[1024][1024];
	double (*updatedmat)[1024][1024];
	double* thread_maxchange;
	*/

}barrier;

barrier* barrier_new(int count);

void barrier_enter(barrier* b, int thd);

#endif
