/***************************************************************
% BW: Bandwidth test & SSI rpi modules
%
%  Designed for LAM/MPI
%  Usage: mpirun -c 2 -ssi rpi <rpi> BW
%		where <rpi> can be lamd, tcp, sysv, usysv, gm (Myrinet)
%		man lamssi_rpi for details
%
%  You can also play setting the LAM_MPI_SSI_rpi environment variable
%  before running this program with & w/out -ssi rpi switches
%
%  1MB is sent&recv 16 times and the global latency is printed onscreen
%  together with the LAM/MPI environment vars, in order to decide
%  which ones provide better performance
 ***************************************************************/

#include <stdio.h>	/* printf, fflush */
#include <stdlib.h>	/* atoi, getenv, calloc */
#include <mpi.h>	/* MPI_* */

#define TAG  7
#define NTIMES 16
#define SIZE   1<<20	/* 2^20 == 1MB */

int main(int argc, char*argv[]){
  int i;				/* loop index */
  char*buffer;				/* send/recv buffer */
  double T,Tot;				/* time stamps */
  int rank,size;			/* MPI stuff */

  Tot = MPI_Wtime();			/* measuring elapsed time */

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  if (size<2){
    printf("2 computers required\n");
    exit(1);
  }

  if (!rank){
    printf("_____________________________________\n");
    system("env | egrep \"LAM|MPI\"");
    printf("_____________________________________\n");
    printf("Sending 1MB..."); fflush(stdout);
  }

  buffer=calloc(SIZE,sizeof(char));
//buffer[0]='A'; buffer[SIZE-1]='Z';

  MPI_Barrier(MPI_COMM_WORLD);
  T=MPI_Wtime();			/* measuring xmit time */
  if (!rank)	for (i=0; i<NTIMES; i++){
    MPI_Send(buffer,SIZE,MPI_BYTE, 1,TAG,MPI_COMM_WORLD);
    MPI_Recv(buffer,SIZE,MPI_BYTE, 1,TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);}

  if (rank==1)	for (i=0; i<NTIMES; i++){
    MPI_Recv(buffer,SIZE,MPI_BYTE, 0,TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(buffer,SIZE,MPI_BYTE, 0,TAG,MPI_COMM_WORLD);
  }

  T=MPI_Wtime()-T;			/* xmit time measured */

  free(buffer);
  if (!rank){
    printf("\t...done\n");
  }
  MPI_Finalize();
  Tot=MPI_Wtime()-Tot;			/* elapsed time measured */

  if (!rank){
    printf("Message time: %8.3f s\n",T);
    printf("BandWidth   : %8.3f MB/s\n",NTIMES*2*(SIZE)/(1<<20)/T);
    printf("Elapsed time: %8.3f s\n",Tot);
  }
  exit(0);

}

