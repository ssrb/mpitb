/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_NULL_DELETE_FN
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI_*_function (callback function) object
 * var = MPI_NULL_DELETE_FN
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#define   NAME	MPI_NULL_DELETE_FN
GEN_1_PTR(NAME)
#undef		MPI_NULL_DELETE_FN
DEFUN_DLD(NAME, args, nargout,
"MPI_NULL_DELETE_FN	MPITB constant for corresponding handle\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free,\n\
            caching, handles\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_PTR
}
