/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_free
/*
 * ----------------------------------------------------
 * Marks the communicator object for deallocation
 * info = MPI_Comm_free (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_F_COMREF

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_free		Marks the communicator object for deallocation\n\
\n\
  info = MPI_Comm_free (comm)\n\
\n\
  comm (int)	communicator to be destroyed (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL? WORLD? SELF?)\n\
\n\
  SEE ALSO: MPI_Comm_create, MPI_Comm_dup, MPI_Comm_split\n\
            comms\n\
\n\
  MPI_COMM_SELF, MPI_COMM_WORLD, MPI_COMM_NULL, are not valid arguments\n\
\n\
  comm will be set to MPI_COMM_NULL upon successful return\n\
\n\
")

	PATN_F_COMREF (NAME)

	/* info = MPI_Comm_free (comm) */
