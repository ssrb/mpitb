#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

main(int argc, char **argv)
{
  register double width;
  double sum, lsum;
  register int intervals, i; 
  int nproc, iproc;
  MPI_Status status;

  double t,e;		/* time & error */
  double PI25DT = 3.141592653589793238462643;

  t=MPI_Wtime();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) exit(1);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &iproc);
  intervals = 1E8;
  width = 1.0 / intervals;
  lsum = 0;
  for (i=iproc; i<intervals; i+=nproc) {
    register double x = (i + 0.5) * width;
    lsum += 4.0 / (1.0 + x * x);
  }
  lsum *= width;
  if (iproc != 0) {
    MPI_Send(&lsum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  } else {
    sum = lsum;
    for (i=1; i<nproc; ++i) {
      MPI_Recv(&lsum, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
               MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      sum += lsum;
    }
  }
  MPI_Finalize();

  if (!iproc) {
    t=MPI_Wtime()-t;
    e=sum-PI25DT;
    printf("Estimation of pi is %f after %d iterations\n", sum,intervals);
    printf("PCs\tTime (s)\tError\n");
    printf(" %d\t %8.6f\t %8.6E\n", nproc,t,e);
  }

  return(0);
}
