/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_size
/*
 * ----------------------------------------------------
 * Returns the size of a group
 * [info size] = MPI_Group_size (grp)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_INT_F_GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_size		Returns the size of a group\n\
\n\
  [info size] =  MPI_Group_size (grp)\n\
\n\
  grp  (int)	group (handle). MPI_GROUP_NULL is not valid\n\
  size (int)	number of processes in the group\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      9 MPI_ERR_GROUP	Null group passed to function\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Group_rank, MPI_Comm_size, MPI_Comm_rank\n\
            comms, groups\n\
\n\
")

	PATN_INT_F_GRP (NAME)

	/* [info size] = MPI_Group_size (grp) */
