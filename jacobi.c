/* Joe Gildner
* Joey Xiong
* CSCI 347
* 12/02/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

#include "barrier.h"

#define THREAD_COUNT 64
#define MATRIX_SIZE 1024

//Temporary barrier
barrier* threadbar;

double threshold = .001;
bool steadystate = false;

pthread_t thread_ids[THREAD_COUNT];
int thread_nums[THREAD_COUNT];
double thread_maxchange[THREAD_COUNT];

double matrix_left[MATRIX_SIZE][MATRIX_SIZE];
double matrix_right[MATRIX_SIZE][MATRIX_SIZE];
double (*currentmat)[MATRIX_SIZE][MATRIX_SIZE];
double (*updatedmat)[MATRIX_SIZE][MATRIX_SIZE];

void* jacobi_algorithm(void* args);

int main(int argc, char* argv[]){
  FILE* inputmatrix = fopen("./input.mtx","r");
  currentmat = &matrix_left;
  updatedmat = &matrix_right;

  for(int row=0; row<MATRIX_SIZE; row++){
    for(int col=0; col<MATRIX_SIZE; col++){
      fscanf(inputmatrix, "%lf ", &((*currentmat)[row][col]));
      (*updatedmat)[row][col] = (*currentmat)[row][col];
    }
  }

  threadbar = barrier_new(THREAD_COUNT);

  for(int i=0; i<THREAD_COUNT; ++i){
    thread_nums[i] = i;
    if(pthread_create(&(thread_ids[i]), NULL, &jacobi_algorithm, (void*)(&thread_nums[i])) != 0){
      perror("pthread");
    }else{
      printf("Created thread %d successfully\n",i);
    }
  }

  for(int i=0; i<THREAD_COUNT; i++){
    pthread_join(thread_ids[i],NULL);
  }

  for(int row=0; row<MATRIX_SIZE; row++){
    for(int col=0; col<MATRIX_SIZE; col++){
      printf("%lf ",(*updatedmat)[row][col]);
    }
    printf("\n");
  }
}

void* jacobi_algorithm(void* args){
  int thd = *((int*) args);

  while(!steadystate){
    thread_maxchange[thd] = 0.0;
    for(int row = thd+1; row<MATRIX_SIZE-1; row += THREAD_COUNT){
      for(int col = 1; col<MATRIX_SIZE-1; col++){
        double above = (*currentmat)[row-1][col];
        double below = (*currentmat)[row+1][col];
        double left = (*currentmat)[row][col-1];
        double right = (*currentmat)[row][col+1];
        double newval = (above + below + left +right)/4;
        (*updatedmat)[row][col] = newval;

        double chg = (newval - (*currentmat)[row][col]);

        if(chg > thread_maxchange[thd]){
          thread_maxchange[thd] = chg;
        }
      }
    }

    barrier_enter(threadbar, thd);
  }

  return NULL;
}

barrier* barrier_new(int count)
{
  barrier* b = malloc(sizeof(barrier));
  if(b!=NULL)
  {
    pthread_mutex_init(b->m,NULL);
    pthread_cond_init(&(b->allHere),NULL);
    pthread_cond_init(&(b->allGone),NULL);
    b->total = count;
    b->here = 0;
    b->leaving = 0;
  }
  return b;
}

void barrier_enter(barrier* b, int thd){
  pthread_mutex_lock(b->m);
  while(b->leaving) {
    pthread_cond_wait(&(b->allGone),b->m);
    pthread_cond_signal(&b->allGone);
  }
  b->here++;
  while(b->here != b->total && !b->leaving){
    pthread_cond_wait(&(b->allHere), b->m);
  }
  // EVERYONE HERE
  double maxchange = 0.0;
  for(int i=0; i<THREAD_COUNT; i++){
    if(thread_maxchange[i] > maxchange) maxchange = thread_maxchange[i];
  }
  steadystate = maxchange < threshold;
  printf("Max change: %lf \n", maxchange);

  double (*swap)[MATRIX_SIZE][MATRIX_SIZE];
  swap = currentmat;
  currentmat = updatedmat;
  updatedmat = swap;
  // TIME TO LEAVE

  b->leaving = 1;
  pthread_cond_signal(&(b->allHere));
  b->here--;
  if(b->here == 0){
    b->leaving = 0;
    pthread_cond_signal(&(b->allGone));
  }
  pthread_mutex_unlock(b->m);
}
