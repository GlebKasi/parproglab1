/* 1-индививидуальное задание у меня вариант г */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 5000
#define JSIZE 5000

//Задача 1г (MPI)

int main(int argc, char **argv)
{
	remove("1indmpi.txt");
	//double a[ISIZE][JSIZE];
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
	
	int commsize, my_rank;
	MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Status status;
  int i_start, i_end;
  int goal;
  /*
  if((JSIZE - 2) % commsize > my_rank)
  {
  	i_start = ((JSIZE - 2) / commsize) * my_rank + my_rank;
  	i_end = ((JSIZE - 2) / commsize) * (my_rank + 1) + my_rank + 1;
  }
  else
  {
  	i_start = ((JSIZE - 2) / commsize) * my_rank + ((JSIZE - 2) % commsize);
  	i_end = ((JSIZE - 2) / commsize) * (my_rank + 1) + ((JSIZE - 2) % commsize);
  }
  */
  int* b[ISIZE];
	for (i=0; i<ISIZE; i++){
		b[i] = (int*) calloc((size_t) JSIZE, sizeof(int));
	}
	for (i=0; i<ISIZE; i++){
		for (j=0; j<JSIZE; j++){
			b[i][j] = 0;
		}
	}
	for (i=3; i<6; i++){
		for (j=0; j<JSIZE - 2; j++){
			b[i][j] = (i * (JSIZE - 2) + j) % commsize;
		}
	}
	for (i=6; i<ISIZE; i++)
	{
		for (j = JSIZE - 4; j < JSIZE - 2; j++)
		{
			b[i][j] = (i * 2 + j) % commsize;
		}
	}
	for (i=6; i<ISIZE; i++){
		for (j=0; j<JSIZE - 4; j++){
			b[i][j] = b[i - 3][j + 2];
		}
	}
	//начало измерения времени
	/*for (i=3; i<ISIZE; i++){
		for (j = 0; j < JSIZE - 2; j++){
			a[i][j] = std::sin(3*a[i -3][j+2]);
		}
	}*/
	//printf("%d, %d, %d\n", my_rank, i_start, i_end);
	MPI_Barrier(MPI_COMM_WORLD);
	double t_start = MPI_Wtime();
	/*if(my_rank < 3)
	{
  for(i = 3 + my_rank; i < ISIZE; i+=3)
  {
  	for (j = 0; j < JSIZE - 2; j++)
  	{
			a[i][j] = std::sin(3*a[i -3][j+2]);
		}
  }
  }*/
	
	for (i=3; i<ISIZE; i++){
		for (j=0; j<JSIZE - 2; j++){
			if(b[i][j] == my_rank)
			{
				a[i][j] = std::sin(3*a[i -3][j+2]);
			}
		}
	}
	
	//окончание измерения времени
	MPI_Barrier(MPI_COMM_WORLD);
	double t_end = MPI_Wtime();
	ff = fopen("1indmpi.txt","a");
	double working_time = t_end - t_start;
	printf("%d, %f\n", my_rank, working_time);
	double buff;
	if(my_rank == 0){
	for(i= 0; i < ISIZE; i++){
		for (j= 0; j < JSIZE; j++){
			if(b[i][j] == 0){
				fprintf(ff,"%f ",a[i][j]);
			}
			else{
				MPI_Recv(&buff, 1, MPI_DOUBLE, b[i][j], b[i][j], MPI_COMM_WORLD, &status);
				fprintf(ff,"%f ",buff);
			}
		}
		fprintf(ff,"\n");
	}
	}
	else
	{
	for(i= 0; i < ISIZE; i++){
		for (j= 0; j < JSIZE; j++){
			if(b[i][j] == my_rank){
				MPI_Send(&a[i][j], 1, MPI_DOUBLE, 0, my_rank, MPI_COMM_WORLD);
				//fprintf(ff,"%d ",b[i][j]);
			}
		}
	}
	}
  MPI_Barrier(MPI_COMM_WORLD);
  
  
	fclose(ff);
	MPI_Finalize();
	return 0;
}
