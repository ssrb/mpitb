/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_rank
/*
 * ----------------------------------------------------
 * Returns the rank of this process in the given group
 * [info rank] = MPI_Group_rank (grp)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_INT_F_GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_rank		Returns the rank of this process in the given group\n\
\n\
  [info rank] =  MPI_Group_rank (grp)\n\
\n\
  grp  (int)	group (handle). MPI_GROUP_NULL is not valid\n\
  rank (int)	rank of the calling process in group,\n\
		or MPI_UNDEFINED if not a member\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      9 MPI_ERR_GROUP	Null group passed to function\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Group_size, MPI_Comm_size, MPI_Comm_rank\n\
            comms, groups\n\
\n\
")

	PATN_INT_F_GRP (NAME)

	/* [info rank] = MPI_Group_rank (grp) */
