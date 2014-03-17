/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Allgatherv
/*
 * ----------------------------------------------------
 * Gathers data from all tasks and deliver it to all
 * The  block of data sent from the jth process is received by every
 * 	process and placed in the jth block of the buffer recvbuf
 * Allgather variant, with displacements
 * info = MPI_Allgatherv (svar, rvar, cnts, disps, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_SRCDC

DEFUN_DLD(NAME, args, nargout,
"MPI_Allgatherv         Gathers data from all tasks and deliver it to all\n\
\n\
  info = MPI_Allgatherv (svar, rvar, cnts, disps, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
\n\
  rvar  Octave variable used as msg dst (at each rank) (room for all incoming)\n\
  cnts (intvec) number of elements to receive from each rank (length commsize)\n\
  disps(intvec) i-th entry:C-style displacement in rvar at which to place the\n\
                incoming data from rank i                    (length commsize)\n\
\n\
  comm  (int)  communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Allgatherv is a collective operation on comm (all ranks must call it)\n\
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

	PATN_SRCDC (NAME)	// SRCDC -> src/rcvar, cnts, disps, comm

	/* info = MPI_Allgatherv (svar, rvar, cnts, disps, comm) */
