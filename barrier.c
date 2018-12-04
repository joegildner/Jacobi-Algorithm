#include <stdlib.h>
#include <pthread.h>
#include "barrier.h"
int main()
{
  return 0;
}
barrier* barrier_new(int count)
{
  barrier* b = malloc(sizeof(barrier));
  if(b!=NULL)
  {
    pthread_mutex_init(b->m,NULL);
    pthread_cond_init(&(b->allHere),NULL);
    pthread_cond_init(&(b->allGone),NULL);
    b->total = 0;
    b->here = 0;
    b->leaving = 0;
  }
  return b;
}
void barrier_enter(barrier* b, int thd)
{
  pthread_mutex_lock(b->m);
  while(b->leaving)
  {
    pthread_cond_wait(&(b->allGone),b->m);
    pthread_cond_signal(&b->allGone);
  }
  b->here++;
  while(b->here != b->total && b->here != b->leaving)
  {
    pthread_cond_wait(&(b->allHere), b->m);
  }
  b->leaving = 1;
  pthread_cond_signal(&(b->allHere));
  b->here--;
  if(b->here == 0)
  {
    b->leaving = 0;
    pthread_cond_signal(&(b->allGone));
  }
  pthread_mutex_unlock(b->m);
}
