#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[]){
	FILE* ourresult = fopen("./output.mtx","r");
	FILE* realresult = fopen("./correct_output.mtx","r");

	for(int row=0; row<1024; row++){
		for(int col=0; col<1024; col++){
			double left;
			double right;
			fscanf(ourresult, "%lf ", &left);
			fscanf(realresult, "%lf ", &right);
			if(abs(left-right) >= .00002) printf("WRONG");
		}
	}
}
