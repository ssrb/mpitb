/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Gather
/*
 * ----------------------------------------------------
 * Gathers together values from a group of processes
 * info = MPI_Gather (svar, rvar, root, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_F_SRRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Gather             Gathers together values from a group of processes\n\
\n\
  info = MPI_Gather (svar, rvar, root, comm)\n\
\n\
  svar  Octave variable used as msg src\n\
  rvar  Octave variable used as msg dst (at root), or meaningless (all others)\n\
  root  (int)  rank of receiving process\n\
  comm  (ptr)  communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
      8 MPI_ERR_ROOT   Invalid root rank (0..Comm_size-1)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Gather is a collective operation on comm (all ranks must call it)\n\
\n\
  it is enough that meaningless arguments be the same type at non-root ranks\n\
  for instance, MPI_Gather(S,[],0,NEWORLD) if scattering doubles\n\
\n\
")

	PATN_F_SRRC (NAME)	// SRRC -> src/rcvar,root,comm

	/* info = MPI_Gather (svar, rvar, root, comm) */
