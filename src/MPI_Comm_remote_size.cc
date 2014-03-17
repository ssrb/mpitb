/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_remote_size
/*
 * ----------------------------------------------------
 * Determines size of the remote group associated with an inter-communicator
 * [info size] = MPI_Comm_remote_size (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_remote_size	Determines size of remote group assoc. with intercomm\n\
\n\
  [info size] = MPI_Comm_remote_size (comm)\n\
\n\
  comm (int)	communicator (handle - must be intercommunicator)\n\
  size (int)	number of processes in the remote group of comm\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL? intra?)\n\
     13 MPI_ERR_ARG    Invalid argument (NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Comm_remote_group, MPI_Comm_group, MPI_Comm_size\n\
            comms, groups\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info size] = MPI_Comm_remote_size (comm) */
