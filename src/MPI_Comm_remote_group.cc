/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_remote_group
/*
 * ----------------------------------------------------
 * Accesses the remote group associated with the given inter‐communicator
 * [info grp] = MPI_Comm_remote_group (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_remote_group	Accesses the remote group assoc. with given intercomm\n\
\n\
  [info grp] = MPI_Comm_remote_group (comm)\n\
\n\
  comm (int)	Communicator (must be intercommunicator)\n\
  grp  (int)	remote group of communicator (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (null? intra?)\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Comm_remote_size, MPI_Comm_size, MPI_Comm_group\n\
		comms, groups\n\
\n\
")

	PATN_GRP_F_COM (NAME)

	/* [info grp] = MPI_Comm_remote_group (comm) */
