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

#define THREAD_COUNT 13

//Temporary barrier
pthread_barrier_t threadbar;
double threshold = .001;
bool steadystate = false;

pthread_t thread_ids[THREAD_COUNT];
int thread_nums[THREAD_COUNT];
double thread_maxchange[THREAD_COUNT];

double matrix_left[1024][1024];
double matrix_right[1024][1024];
double* currentmat;
double* updatedmat;

void* jacobi_algorithm(void* args);

int main(int argc, char* argv[]){
  FILE* inputmatrix = fopen("./input.mtx","r");
  currentmat = &(matrix_left[0][0]);
  updatedmat = &(matrix_right[0][0]);

  for(int row=0; row<1024; row++){
    for(int col=0; col<1024; col++){
      fscanf(inputmatrix, "%lf ", &(currentmat[row][col]));
      updatedmat[row][col] = -1.0;
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

  for(int row = 1; row<1023; row++){
    for(int col = 1; col<1023; col++){
      if(updatedmat[row][col] == -1.0)
      printf("ERROR %d,%d\n", row, col);
    }
  }
}

void* jacobi_algorithm(void* args){
  int thd = *((int*) args);

  while(!steadystate){
    for(int row = 1; row<1023; row++){
      for(int col = thd+1; col<1023; col += THREAD_COUNT){

        double above = currentmat[row-1][col];
        double below = currentmat[row+1][col];
        double left = currentmat[row][col-1];
        double right = currentmat[row][col+1];
        updatedmat[row][col] = (above + below + left +right)/4;

        double chg = abs(updatedmat[row][col] - currentmat[row][col]);
        if(chg > thread_maxchange[thd]) thread_maxchange[thd] = chg;
      }
    }


    if(pthread_barrier_wait(&threadbar)){
      double maxchange = 0.0;
      for(int i=0; i<THREAD_COUNT; i++){
        if(thread_maxchange[i] > maxchange)maxchange = thread_maxchange[i];
      }
      steadystate = maxchange < threshold;
      currentmat = updatedmat;
    }
    pthread_barrier_wait(&threadbar);
  }



  return NULL;
}
