/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_HOST
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * MPI_Attr_get(MPI_COMM_WORLD, MPI_HOST)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#define   NAME	MPI_HOST
GEN_1_INT(NAME)			// works if #defined MPI... (LAM/MPI), and
#undef		MPI_HOST	// does not harm if enum{MPI...} (OpenMPI)
DEFUN_DLD(NAME, args, nargout,
"MPI_HOST               MPITB constant for corresponding MPI symbol\n\
\n\
  SEE ALSO: MPI_ATTRKVALS\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
