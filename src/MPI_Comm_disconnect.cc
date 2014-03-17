/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_disconnect
/*
 * ----------------------------------------------------
 * Disconnect a client and server (MPI_Comm_accept/connect)
 * info = MPI_Comm_disconnect (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_F_COMREF

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_disconnect	Disconnect a client and server\n\
\n\
  info = MPI_Comm_disconnect (comm)\n\
\n\
  comm (int)	communicator handle (from MPI_Comm_[accept|connect])\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER	Other error - use MPI_Error_string for more info\n\
\n\
  SEE ALSO: MPI_Open_port, MPI_Comm_accept, MPI_Comm_connect, MPI_Close_port,\n\
            process\n\
\n\
  Waits for pending communications to take place\n\
  On return, comm set to MPI_COMM_NULL\n\
  Must disconnect (not MPI_Comm_free), or server/client will remain connected\n\
  Can't use MPI_COMM_NULL as argument\n\
\n\
")

	PATN_F_COMREF (NAME)

	/* info = MPI_Comm_disconnect (comm) */
