/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_union
/*
 * ----------------------------------------------------
 * Produces a group by combining two groups
 * [info grp] = MPI_Group_union (grp1, grp2)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_2GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_union	Produces a group by combining two groups\n\
\n\
  [info grp] = MPI_Group_union (grp1, grp2)\n\
\n\
  grp[n](int)	groups (handles)\n\
\n\
  info  (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
      9 MPI_ERR_GROUP  Null group passed to function\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
     13 MPI_ERR_ARG    Invalid argument (typically  a  NULL pointer)\n\
\n\
  SEE ALSO: MPI_Group_difference, MPI_Group_intersection, MPI_Comm_group\n\
            comms, groups\n\
\n\
  When a group is no longer being used, it should be freed with MPI_Group_free\n\
\n\
")

	PATN_GRP_F_2GRP (NAME)

	/* [info grp] = MPI_Group_union (grp1, grp2) */
