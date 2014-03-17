/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_test_inter
/*
 * ----------------------------------------------------
 * Test to see if a comm is an inter‐communicator
 * [info flag] = MPI_Comm_test_inter (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_test_inter	Tests to see if a comm is an inter-communicator\n\
\n\
  [info flag] = MPI_Comm_test_inter (comm)\n\
\n\
  comm (int)	communicator (handle)\n\
  flag (int)	boolean indicating whether comm is an intercomm (1) or not (0)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Intercomm_create, MPI_Intercomm_merge, MPI_Comm_remote_group\n\
            comms\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info flag] = MPI_Comm_test_inter (comm) */
