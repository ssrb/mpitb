/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_compare
/*
 * ----------------------------------------------------
 * Compares two groups
 * [info result] = MPI_Group_compare (grp1, grp2)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_INT_F_2GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_compare	Compares two groups\n\
\n\
  [info result] = MPI_Group_compare (grp1, grp2)\n\
\n\
  grp[n] (int)	groups (handles)\n\
  result (int)	comparison code\n\
      1 MPI_IDENT	order and members are the same\n\
      3 MPI_SIMILAR	different order but same members\n\
      4 MPI_UNEQUAL	otherwise\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS	No error\n\
      9 MPI_ERR_GROUP	Null group passed to function\n\
     13 MPI_ERR_ARG	Invalid argument (typically  a  NULL pointer)\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Comm_compare, MPI_Group_size, MPI_Group_rank,\n\
            MPI_GROUP_EMPTY, MPI_GROUP_NULL,\n\
            comms, groups\n\
\n\
")

	PATN_INT_F_2GRP (NAME)

	/* [info result] = MPI_Group_compare (grp1, grp2) */
