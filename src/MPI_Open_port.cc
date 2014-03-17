/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Open_port
/*
 * ----------------------------------------------------
 * Establish a server port for client MPI connections
 * [info port] = MPI_Open_port (inf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_STR_F_INF

DEFUN_DLD(NAME, args, nargout,
"MPI_Open_port		Establish a server port for client MPI connections\n\
\n\
  [info port] = MPI_Open_port (inf)\n\
\n\
  inf  (int)	extra information in MPI_Info object (handle)\n\
\n\
  port (str)	newly established port (name)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     16 MPI_ERR_OTHER	Other error - use MPI_Error_string for more info\n\
     17 MPI_ERR_INTERN	This is fatal - send a bug report to LAM\n\
\n\
  SEE ALSO: MPI_Close_port, MPI_Comm_accept, MPI_Comm_connect,\n\
            MPI_Comm_disconnect,\n\
            process\n\
\n\
")

	PATN_STR_F_INF (NAME)

	/* [info port] = MPI_Open_port (inf) */
