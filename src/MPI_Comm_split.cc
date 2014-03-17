/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_split
/*
 * ----------------------------------------------------
 * Create new communicators based on colors and keys
 * [info comm2] = MPI_Comm_split (comm1, color, key)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_COM_F_COM_2INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_split		Creates new communicators based on colors and keys\n\
\n\
  [info comm2] = MPI_Comm_split (comm1, color, key)\n\
\n\
  commX (int)	communicators (handles)\n\
  color (int)	control of subset assignment (nonnegative)\n\
		or MPI_UNDEFINED if a process doesn't want to belong to any\n\
  key   (int)	control of rank assigment\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
     17 MPI_ERR_INTERN	This is  fatal. Send a bug report to the LAM team.\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Comm_create, MPI_Comm_dup, MPI_Comm_free\n\
            comms, groups\n\
\n\
  MPI_Comm_split is a collective operation on comm1\n\
\n\
  When a comm. is no longer being used, it should be freed with MPI_Comm_free\n\
\n\
")

	PATN_COM_F_COM_2INT (NAME)

	/* [info comm2] = MPI_Comm_split (comm1, color, key) */
