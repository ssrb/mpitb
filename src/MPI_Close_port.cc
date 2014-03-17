/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Close_port
/*
 * ----------------------------------------------------
 * Close a (previously opened) server port
 * info = MPI_Close_port (port)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_F_STR

DEFUN_DLD(NAME, args, nargout,
"MPI_Close_port		Close a server port\n\
\n\
  info = MPI_Close_port (port)\n\
\n\
  port (str)	port name (returned from a previous MPI_Open_port)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument\n\
     16 MPI_ERR_OTHER	Other error - use MPI_Error_string for more info\n\
     17 MPI_ERR_INTERN	This is fatal - send a bug report to LAM\n\
\n\
  SEE ALSO: MPI_Open_port, MPI_Comm_accept, MPI_Comm_connect,\n\
            MPI_Comm_disconnect,\n\
            process\n\
\n\
")

	PATN_F_STR (NAME)

	/* info = MPI_Close_port (port) */
