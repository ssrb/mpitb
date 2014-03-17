/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Alltoallv
/*
 * ----------------------------------------------------
 * Sends data from all to all processes, with a displacement
 * info = MPI_Alltoallv (svar,scnts,sdisps, rvar,rcnts,rdisps, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_SSSRRRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Alltoallv          Sends data from all to all processes with displacements\n\
\n\
  info = MPI_Alltoallv (svar,scnts,sdisps, rvar,rcnts,rdisps, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
 scnts (intvec) number of elements to send to each processor (length commsize)\n\
sdisps (intvec) i-th entry:C-style displacement in svar from which to take the\n\
                outgoing data to rank i                      (length commsize)\n\
\n\
  rvar  Octave variable used as msg dst (at each rank) (room for all incoming)\n\
 rcnts (intvec) number of elements to receive from each rank (length commsize)\n\
rdisps (intvec) i-th entry:C-style displacement in rvar at which to place the\n\
                incoming data from rank i                    (length commsize)\n\
\n\
  comm (int)   communicator (handle)\n\
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
  MPI_Alltoallv is a collective operation on comm (all ranks must call it)\n\
\n\
")

	PATN_SSSRRRC (NAME)	// SSSRRRC->svar/cnt/dsp,rvar/cnt/dsp,comm

	/* info = MPI_Alltoallv (svar,scnts,sdisps, rvar,rcnts,rdisps, comm) */
