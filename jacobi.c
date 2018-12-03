/* Joe Gildner
 * Joey Xiong
 * CSCI 347
 * 12/02/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_COUNT 13

pthread_t thread_ids[THREAD_COUNT];
int thread_nums[THREAD_COUNT];

double current_matrix[1024][1024];
double updated_matrix[1024][1024];

void* jacobi_algorithm(void* args);

int main(int argc, char* argv[]){
  FILE* inputmatrix = fopen("./input.mtx","r");

  for(int row=0; row<1024; row++){
    for(int col=0; col<1024; col++){
      fscanf(inputmatrix, "%lf ", &(current_matrix[row][col]));
      updated_matrix[row][col] = -1.0;
    }
  }


  
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
      if(updated_matrix[row][col] == -1.0)
        printf("ERROR %d,%d\n", row, col);
    }
  }


}

void* jacobi_algorithm(void* args){
  int thd = *((int*) args);

  for(int row = 1; row<1023; row++){
    for(int col = thd+1; col<1023; col += THREAD_COUNT){
      double above = current_matrix[row-1][col];
      double below = current_matrix[row+1][col];
      double left = current_matrix[row][col-1];
      double right = current_matrix[row][col+1];
      updated_matrix[row][col] = (above + below + left +right)/4;
    }
  }

  return NULL; 
}
