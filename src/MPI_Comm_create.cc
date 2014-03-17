/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_create
/*
 * ----------------------------------------------------
 * Creates a new communicator
 * [info comm2] = MPI_Comm_create (comm1, grp)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_COM_F_COM_GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_create	Creates a new communicator (changing group)\n\
\n\
  [info comm2] = MPI_Comm_create (comm1, grp)\n\
\n\
  commX (int)	communicators (handles). MPI_COMM_NULL is not valid\n\
  grp   (int)	desired group, which is a subset of the group of comm1 (handle)\n\
\n\
  info  (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
      9 MPI_ERR_GROUP	Null group passed to function\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
     17 MPI_ERR_INTERN	This is  fatal. Send a bug report to the LAM team.\n\
\n\
  SEE ALSO: MPI_Comm_dup, MPI_Comm_split, MPI_Comm_free\n\
            comms, groups\n\
\n\
  MPI_Comm_create is a collective operation on comm1 ?!? comm2 ?!?\n\
\n\
  When a comm. is no longer being used, it should be freed with MPI_Comm_free\n\
\n\
")

	PATN_COM_F_COM_GRP (NAME)

	/* [info comm2] = MPI_Comm_create (comm1, grp) */
