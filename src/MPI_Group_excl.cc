/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_excl
/*
 * ----------------------------------------------------
 * Produces a group from an existing group taking only unlisted members
 * [info grp2] = MPI_Group_excl (grp, ranks)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_GRP_RNK

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_excl		Reorder existing group and take only unlisted members\n\
\n\
  [info grp2] = MPI_Group_excl (grp, ranks)\n\
\n\
  grp[x]  (int)	groups (handles)\n\
  ranks(intvec) ranks of processes in grp not to appear in grp2\n\
                grp2 preserves the order defined by grp\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      9 MPI_ERR_GROUP  Null group passed to function\n\
     13 MPI_ERR_ARG    Invalid argument (typically  a  NULL pointer)\n\
      6 MPI_ERR_RANK   Invalid rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Group_incl, MPI_Group_range_incl, MPI_Group_range_excl\n\
            comms, groups\n\
\n\
  Each  of the ranks to exclude must be a valid rank in the group and all\n\
  elements must be distinct or the function is erroneous\n\
\n\
  When a group is no longer being used, it should be freed with MPI_Group_free\n\
\n\
")

	PATN_GRP_F_GRP_RNK (NAME)

	/* [info grp2] = MPI_Group_excl (grp, ranks) */
