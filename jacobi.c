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

#define THREAD_COUNT 64
#define MATRIX_SIZE 1024

//Temporary barrier
pthread_barrier_t threadbar;
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

  pthread_barrier_init(&threadbar, NULL, THREAD_COUNT);

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


    if(pthread_barrier_wait(&threadbar)){

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

    }
    pthread_barrier_wait(&threadbar);



  }



  return NULL;
}
