/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_DISTRIBUTE_DFLT_DARG
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#define   NAME	MPI_DISTRIBUTE_DFLT_DARG
GEN_1_INT(NAME)
#undef		MPI_DISTRIBUTE_DFLT_DARG
DEFUN_DLD(NAME, args, nargout,
"MPI_DISTRIBUTE_DFLT_DARG  MPITB constant for corresponding MPI symbol\n\
\n\
  SEE ALSO: misc\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
