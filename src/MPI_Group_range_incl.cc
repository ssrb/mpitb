/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_range_incl
/*
 * ----------------------------------------------------
 * Creates a new group from ranges of ranks in an existing group
 * [info grp2] = MPI_Group_range_incl (grp, ranges)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_GRP_RNG

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_range_incl	Create new group from ranges of ranks in existing group\n\
\n\
  [info grp2] = MPI_Group_range_incl (grp, ranges)\n\
\n\
  grp[x]   (int) groups (handles)\n\
  ranges(intarr) array nx3 of integer triplets, of the form\n\
                         [first  rank,  last  rank, stride]\n\
                 indicating ranks in grp to be included in grp2 (in that order)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      9 MPI_ERR_GROUP  Null group passed to function\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
     13 MPI_ERR_ARG    Invalid argument (typically  a  NULL pointer)\n\
      6 MPI_ERR_RANK   Invalid rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Group_range_excl, MPI_Group_incl, MPI_Group_excl\n\
            comms, groups\n\
\n\
  When a group is no longer being used, it should be freed with MPI_Group_free\n\
\n\
")

	PATN_GRP_F_GRP_RNG (NAME)

	/* [info grp2] = MPI_Group_range_incl (grp, ranges) */
