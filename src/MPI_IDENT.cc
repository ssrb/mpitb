/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_IDENT
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * if info==MPI_IDENT, ..., end
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#define   NAME	MPI_IDENT
GEN_1_INT(NAME)			// works if #defined MPI... (LAM/MPI), and
#undef		MPI_IDENT	// does not harm if enum{MPI...} (OpenMPI)
DEFUN_DLD(NAME, args, nargout,
"MPI_IDENT              MPITB constant for corresponding MPI symbol\n\
\n\
  SEE ALSO: comms\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
