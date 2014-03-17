#include <stdio.h>
#include <mpi.h>
int main(int argc, char*argv[]){
	int		size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("\nmy MPI_COMM_WORLD is size==%d, i'm rank==%d\n\n",size,rank);
MPI_Finalize();
	return(0);
}

