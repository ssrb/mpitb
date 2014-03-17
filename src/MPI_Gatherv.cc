/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Gatherv
/*
 * ----------------------------------------------------
 * Gathers into specified locations from all processes in a group
 * Gather variant with displacements
 * info = MPI_Gatherv (svar, rvar, cnts, disps, root, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_SRCDRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Gatherv            Gathers into specified locations from all ranks in comm\n\
\n\
  info = MPI_Gatherv (svar, rvar, cnts, disps, root, comm)\n\
\n\
  svar  Octave variable used as msg src\n\
\n\
  rvar  Octave variable used as msg dst (at root), or meaningless (all others)\n\
  cnts (intvec) number of elements to receive from each rank (length commsize)\n\
  disps(intvec) i-th entry:C-style displacement in rvar at which to place the\n\
                incoming data from rank i                    (length commsize)\n\
\n\
  root (int)   rank of receiving process\n\
  comm (int)   communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
      8 MPI_ERR_ROOT   Invalid root rank (0..Comm_size-1)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Gatherv is a collective operation on comm (all ranks must call it)\n\
\n\
  it is enough that meaningless arguments be the same type at non-root ranks\n\
  for instance, MPI_Gatherv(S,[],[],[],0,NEWORLD) if scattering doubles\n\
\n\
")

	PATN_SRCDRC (NAME)	// SRCDRC->s/rvar,cnts,disps,root,comm

	/* info = MPI_Gatherv (svar, rvar, cnts, disps, root, comm) */
