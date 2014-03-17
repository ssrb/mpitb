/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Allgather
/*
 * ----------------------------------------------------
 * Gathers data from all tasks and distribute it to all
 * The block of data sent from the jth process is received by every
 * 	process and placed in the jth block of the buffer recvbuf
 * info = MPI_Allgather (svar, rvar, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_F_SRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Allgather          Gathers data from all tasks and distribute it to all\n\
\n\
  info = MPI_Allgather (svar, rvar, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
  rvar  Octave variable used as msg dst (at each rank)\n\
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
  MPI_Allgather is a collective operation on comm (all ranks must call it)\n\
\n\
  The MPI standard (1.0 and 1.1) says\n\
  `The jth block of data sent from each process is received by every process\n\
   and placed in the jth block of the buffer recvbuf.'\n\
\n\
  This is misleading; a better description is\n\
  `The block of data sent from the jth process is received by every process\n\
   and placed in the jth block of the buffer recvbuf.'\n\
\n\
  This text was suggested by Rajeev Thakur.\n\
\n\
")

	PATN_F_SRC (NAME)	// SRC -> srcvar,rcvvar, comm

	/* info = MPI_Allgather (svar, rvar, comm) */
