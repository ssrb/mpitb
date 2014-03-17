/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_ERRORS_RETURN
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI_Errhandler object
 * var = MPI_ERRORS_RETURN
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#define   NAME	MPI_ERRORS_RETURN
GEN_1_PTR(NAME)
#undef		MPI_ERRORS_RETURN
DEFUN_DLD(NAME, args, nargout,
"MPI_ERRORS_RETURN     MPITB constant for corresponding MPI_Errhandler object\n\
\n\
  SEE ALSO: MPI_Errhandler_set, MPI_Errhandler_get,\n\
            errors, handles\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_PTR
}
