/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_GROUP_EMPTY
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI_Group object
 * var = MPI_GROUP_EMPTY
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#define   NAME	MPI_GROUP_EMPTY
GEN_1_PTR(NAME)
#undef		MPI_GROUP_EMPTY
DEFUN_DLD(NAME, args, nargout,
"MPI_GROUP_EMPTY        MPITB constant for corresponding MPI_Group object\n\
\n\
  SEE ALSO: groups, handles\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_PTR
}
