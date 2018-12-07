#include "matrixio.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

void load_matrices(double (*current)[1024][1024], double (*update)[1024][1024]){
  FILE* inputmatrix = fopen("./input.mtx","r");

  for(int row=0; row<1024; row++){
    for(int col=0; col<1024; col++){
      fscanf(inputmatrix, "%lf ", &((*current)[row][col]));
      (*update)[row][col] = (*current)[row][col];
    }
  }

  fclose(inputmatrix);
}

void output_matrix(double (*current)[1024][1024]){
  FILE* outputmat = fopen("./output.mtx","w");

  for(int row = 0; row<1024; row++){
    for(int col = 0; col<1024; col++){
      fprintf(outputmat,"%.10f ", (*current)[row][col]);
    }
    fprintf(outputmat,"\n");
  }

  fclose(outputmat);
}