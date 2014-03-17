/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_dup
/*
 * ----------------------------------------------------
 * Duplicates an existing communicator with all its cached information
 * [info comm2] = MPI_Comm_dup (comm1)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_COM_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_dup		Duplicates an existing comm. with all its cached info\n\
\n\
  [info comm2] = MPI_Comm_dup (comm1)\n\
\n\
  commX (int)	communicators (handles). MPI_COMM_NULL is not valid\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
     17 MPI_ERR_INTERN	This is  fatal. Send a bug report to the LAM team.\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Comm_create, MPI_Comm_free, MPI_Comm_split\n\
            comms, groups\n\
\n\
	MPI_Comm_dup is a collective operation on comm1\n\
\n\
")

	PATN_COM_F_COM (NAME)

	/* [info comm2] = MPI_Comm_dup (comm1) */
