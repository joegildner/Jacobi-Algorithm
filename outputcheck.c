#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char* argv[]){
	FILE* ourresult = fopen("./output.mtx","r");
	FILE* realresult = fopen("./correct_output.mtx","r");
	int totalitems = 1024*1024;
	int totalcorrect = 1024*1024;

	printf("Analyzing...\n");

	for(int row=0; row<1024; row++){
		for(int col=0; col<1024; col++){
			double left;
			double right;
			fscanf(ourresult, "%lf ", &left);
			fscanf(realresult, "%lf ", &right);
			if(abs(left-right) >= .00002) totalcorrect--;
		}
	}

	double percentcorrect = 100 * (double)totalcorrect / (double)totalitems;
	printf("Output is %.2f%% Correct\n", percentcorrect);

	fclose(ourresult);
	fclose(realresult);
}
