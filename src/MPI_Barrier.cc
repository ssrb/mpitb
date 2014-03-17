/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Barrier
/*
 * ----------------------------------------------------
 * Blocks until all process have reached this routine
 * info = MPI_Barrier(comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Barrier            Blocks until all process have reached this routine\n\
\n\
  info = MPI_Barrier (comm)\n\
\n\
  comm (int)   communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
\n\
  SEE ALSO: MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Barrier is a collective operation on comm (all ranks must call it)\n\
\n\
  For 4 or less ranks, a linear algorithm is used,  where  rank  0  first\n\
  receives  synchronization  message  from  each other rank.  Rank 0 then\n\
  sends out a synchronization message to each other rank.\n\
\n\
  If more than 4 ranks are involved, a tree‐based algorithm  is  used  to\n\
  receive  and then send the synchronization messages to and from rank 0.\n\
\n\
")

	PATN_F_COM (NAME)

	/* info = MPI_Barrier(comm) */
