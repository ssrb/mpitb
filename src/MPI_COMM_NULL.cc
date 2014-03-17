/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_COMM_NULL
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI_Comm object
 * var = MPI_COMM_NULL
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#define   NAME	MPI_COMM_NULL
GEN_1_PTR(NAME)
#undef		MPI_COMM_NULL
DEFUN_DLD(NAME, args, nargout,
"MPI_COMM_NULL          MPITB constant for corresponding handle\n\
\n\
  SEE ALSO: comms, handles\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_PTR
}
