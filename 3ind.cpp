/* 2-индививидуальное задание у меня вариант г */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define ISIZE 5000
#define JSIZE 5000
#define NUMTHREADS 1

int main(int argc, char **argv)
{
	omp_set_num_threads(NUMTHREADS);
	omp_set_dynamic(1);
	double* a[ISIZE];
	double* b[ISIZE];
	int i, j;
	for (i=0; i<ISIZE; i++){
		a[i] = (double*) calloc((size_t) JSIZE, sizeof(double));
		b[i] = (double*) calloc((size_t) JSIZE, sizeof(double));
	}
	FILE *ff;
	for (i=0; i<ISIZE; i++){
		for (j=0; j<JSIZE; j++){
			a[i][j] = 10*i +j;
			b[i][j] = 0;
		}
	}
	double start = omp_get_wtime();
	//начало измерения времени
	#pragma omp parallel for num_threads(NUMTHREADS) 
	for (i=0; i<ISIZE; i++){
		for (j = 0; j < JSIZE; j++){
			a[i][j] = sin(0.005*a[i][j]);
		}
	}
	#pragma omp parallel for num_threads(NUMTHREADS) 
	for (i=5; i<ISIZE; i++){
		for (j = 0; j < JSIZE-2; j++){
			b[i][j] = a[i-5][j+2]*1.5;
		}
	}
	//окончание измерения времени
	double end = omp_get_wtime();
	printf("%f\n", end - start);
	ff = fopen("3indomp.txt","w");
	for(i=0; i < ISIZE; i++){
		for (j= 0; j < JSIZE; j++){
			fprintf(ff,"%f ",b[i][j]);
		}
		fprintf(ff,"\n");
	}
	fclose(ff);
}
