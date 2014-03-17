/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Group_translate_ranks
/*
 * ----------------------------------------------------
 * Translates the ranks of processes in one group to those in another group
 * [info ranks2] = MPI_Group_translate_ranks (grp1, ranks1, grp2)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_G_RNK_G

DEFUN_DLD(NAME, args, nargout,
"MPI_Group_translate_ranks	Transl.procs' ranks in one grp to another grp\n\
\n\
  [info ranks2] = MPI_Group_translate_ranks (grp1, ranks1, grp2)\n\
\n\
  grp[x] (int)    groups' handles\n\
  ranks1 (intvec) vector of zero or more valid ranks in grp1\n\
  ranks2 (intvec) vector of corresponding ranks in grp2,\n\
		   MPI_UNDEFINED when no correspondence exists\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      9 MPI_ERR_GROUP	Null group passed to function\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
      6 MPI_ERR_RANK	Invalid rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Group_rank,\n\
            groups\n\
\n\
")

	PATN_GRP_F_G_RNK_G (NAME)

	/* [info ranks2] = MPI_Group_translate_ranks (grp1, ranks1, grp2) */
