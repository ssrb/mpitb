/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_free
/*
 * ----------------------------------------------------
 * Frees a group
 * info = MPI_Group_free (grp)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_F_GRP

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_free		Frees a group\n\
\n\
  info = MPI_Group_free (grp)\n\
\n\
  grp  (int)	group (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
      9 MPI_ERR_GROUP  Null group passed to function\n\
     13 MPI_ERR_ARG    Invalid argument (typically  a  NULL pointer)\n\
\n\
  SEE ALSO: MPI_Comm_group, MPI_Group_union, MPI_Group_difference\n\
            comms, groups\n\
\n\
  On output, grp is set to MPI_GROUP_NULL\n\
\n\
")

	PATN_F_GRP( NAME)

	/* info = MPI_Group_free (grp) */
