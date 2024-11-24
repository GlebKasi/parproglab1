/* основная задача без изменений */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv)
{
	double* a[ISIZE];
	int i, j;
	for (i=0; i<ISIZE; i++){
		a[i] = (double*) calloc((size_t) JSIZE, sizeof(double));
	}
	FILE *ff;
	for (i=0; i<ISIZE; i++){
		for (j=0; j<JSIZE; j++){
			a[i][j] = 10*i +j;
		}
	}
	//начало измерения времени
	for (i=0; i<ISIZE; i++){
		for (j = 0; j < JSIZE; j++){
			a[i][j] = std::sin(2*a[i][j]);
		}
	}
	//окончание измерения времени
	ff = fopen("e1.txt","w");
	for(i=0; i < ISIZE; i++){
		for (j=0; j < JSIZE; j++){
			fprintf(ff,"%f ",a[i][j]);
		}
		fprintf(ff,"\n");
	}
	fclose(ff);
}
