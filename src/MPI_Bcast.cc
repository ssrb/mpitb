/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Bcast
/*
 * ----------------------------------------------------
 * Broadcasts a message from the process with rank "root" to
 * 	all other processes of the group
 * info = MPI_Bcast (var, root, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_F_VRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Bcast      Broadcasts a msg from process `root' to all others of the group\n\
\n\
  info = MPI_Bcast (var, root, comm)\n\
\n\
  var        Octave variable used as msg src (at root) or msg dst (all others)\n\
  root (int) rank of broadcast root\n\
  comm (ptr) communicator (handle)\n\
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
  SEE ALSO: MPI_Barrier, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Bcast is a collective operation on comm (all ranks must call it)\n\
\n\
  For  4  or  less  ranks, a linear algorithm is used, where rank 0 loops\n\
  over sending the message to each other rank.\n\
\n\
  If more than 4 ranks are involved, a tree-based algorithm  is  used  to\n\
  send the messages from rank 0.\n\
\n\
")

	PATN_F_VRC (NAME)	// VRC -> var, root, comm

	/* info = MPI_Bcast (var, root, comm) */
