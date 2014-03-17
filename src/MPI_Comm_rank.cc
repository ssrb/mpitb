/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_rank
/*
 * ----------------------------------------------------
 * Determines the rank of the calling process in the communicator
 * [info rank] = MPI_Comm_rank (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_rank          Determines rank of calling process in communicator\n\
\n\
  [info rank] =  MPI_Comm_rank (comm)\n\
\n\
  comm (int) communicator handle. MPI_COMM_NULL not valid\n\
  rank (int) rank of the calling process in group of comm\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Comm_size\n\
            comms\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info rank] = MPI_Comm_rank (comm) */
