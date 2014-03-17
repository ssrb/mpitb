/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Intercomm_merge
/*
 * ----------------------------------------------------
 * Creates an intracommunicator from both groups in an intercommunicator
 * [info intracom] = MPI_Intercomm_merge (intercom, high)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_COM_F_COM_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Intercomm_merge    Creates intracomm (1 group) from an intercomm (2 grps)\n\
\n\
  [info intracomm] = MPI_Intercomm_merge (intercom, high)\n\
\n\
  intercom(int) Inter-communicator (returned from MPI_Comm_spawn, for instace)\n\
  high    (int) whether this group comes first in new intracomm (0) o not (1)\n\
                all ranks in same group should use same high value\n\
\n\
  info    (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
     13 MPI_ERR_ARG    Invalid argument\n\
     16 MPI_ERR_OTHER  LAM unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Intercomm_create, MPI_Comm_free\n\
            comms, groups\n\
\n\
  MPI_Intercomm_merge is a collective operation on intercom\n\
\n\
")

	PATN_COM_F_COM_INT (NAME,high)

	/* [info intracom] = MPI_Intercomm_merge (intercom, high) */
