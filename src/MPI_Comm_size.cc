/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_size
/*
 * ----------------------------------------------------
 * Determines  the  size of the group associated with a communicator
 * [info size] = MPI_Comm_size (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_size          Determines size of group associated with communicator\n\
\n\
  [info size] =  MPI_Comm_size (comm)\n\
\n\
  comm (int) (intra)-communicator handle. MPI_COMM_NULL not valid\n\
  size (int) number of processes in the group of comm\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator ('NULL'?)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Comm_rank\n\
            comms\n\
\n\
  MPI_COMM_NULL is not considered a valid argument to this function\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info size] = MPI_Comm_size (comm) */
