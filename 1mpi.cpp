/* основная задача с распараллеливанием через mpi*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ISIZE 5000
#define JSIZE 5000

int main(int argc, char **argv)
{
	remove("1mpi.txt");
	double* a[ISIZE];
	int i, j;
	for (i=0; i<ISIZE; i++){
		a[i] = (double*) calloc((size_t) JSIZE, sizeof(double));
	}
	FILE* ff;
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
  
  if(ISIZE % commsize > my_rank)
  {
  	i_start = (ISIZE / commsize) * my_rank + my_rank;
  	i_end = (ISIZE / commsize) * (my_rank + 1) + my_rank + 1;
  }
  else
  {
  	i_start = (ISIZE / commsize) * my_rank + (ISIZE % commsize);
  	i_end = (ISIZE / commsize) * (my_rank + 1) + (ISIZE % commsize);
  }
  double t_start = MPI_Wtime();
	//начало измерения времени
	for (i=i_start; i<i_end; i++){
		for (j = 0; j < JSIZE; j++){
			a[i][j] = sin(2*a[i][j]);
		}
	}
	//окончание измерения времени
	double t_end = MPI_Wtime();
	ff = fopen("1mpi.txt","a");
	//MPI_File_open(MPI_COMM_WORLD, "1mpi.txt", MPI_MODE_WRONLY, MPI_INFO_NULL, &ff);
	
	double working_time = t_end - t_start;
	printf("%f\n", working_time);
	/*for(i=i_start; i < i_end; i++){
		for (j=0; j < JSIZE; j++){
			fprintf(ff,"%f ",a[i][j]);
		}
		fprintf(ff,"\n");
	}*/
	if(my_rank == 0){
		
  	for(i=i_start; i < i_end; i++){
			for (j=0; j < JSIZE; j++){
				fprintf(ff,"%f ",a[i][j]);
			}
			fprintf(ff,"\n");
		}
		MPI_Send(&t_end, 1, MPI_DOUBLE, my_rank + 1, commsize + 1, MPI_COMM_WORLD);
  }
  else{
  if(my_rank == commsize - 1){
  	MPI_Recv(&t_end, 1, MPI_DOUBLE, my_rank - 1, commsize + 1, MPI_COMM_WORLD, &status);
  	for(i=i_start; i < i_end; i++){
			for (j=0; j < JSIZE; j++){
				fprintf(ff,"%f ",a[i][j]);
			}
			fprintf(ff,"\n");
		}
  }
  else{
  	MPI_Recv(&t_end, 1, MPI_DOUBLE, my_rank - 1, commsize + 1, MPI_COMM_WORLD, &status);
  	for(i=i_start; i < i_end; i++){
			for (j=0; j < JSIZE; j++){
				fprintf(ff,"%f ",a[i][j]);
			}
			fprintf(ff,"\n");
		}
		MPI_Send(&t_end, 1, MPI_DOUBLE, my_rank + 1, commsize + 1, MPI_COMM_WORLD);
  }
  }
	fclose(ff);
	//MPI_File_close(&ff);
	MPI_Finalize();
	return 0;
}
