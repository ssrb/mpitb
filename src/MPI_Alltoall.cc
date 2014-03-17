/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Alltoall
/*
 * ----------------------------------------------------
 * Sends data from all to all processes
 * info = MPI_Alltoall (svar, rvar, srcnt, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_F_SRCC

DEFUN_DLD(NAME, args, nargout,
"MPI_Alltoall           Sends data from all to all processes\n\
\n\
  info = MPI_Alltoall (svar, rvar, srcnt, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
  rvar  Octave variable used as msg dst (at each rank)\n\
 srcnt  (int)  number of elements to (send to/recv from) each process\n\
  comm  (int)  communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Alltoall is a collective operation on comm (all ranks must call it)\n\
\n\
")

	PATN_F_SRCC (NAME)	// SRCC -> src/rcvar,cnt,comm

	/* info = MPI_Alltoall (svar, rvar, srcnt, comm) */
