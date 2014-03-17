/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_ROOT
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * if info==MPI_ROOT, ..., end
 * ----------------------------------------------------
 * In order to be more or less implementation-independent,
 * we try to define all symbols that we know of in all implementations
 * and make them MPI_SUCCESS==0 if not defined in this implementation
 * CAVEAT: notice we rely on the implementation #defining all these symbols
 * (ie, they're not constants or functions)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#ifndef	MPI_ROOT
#define	MPI_ROOT MPI_SUCCESS
#endif

#define   NAME	MPI_ROOT
GEN_1_INT(NAME)
#undef		MPI_ROOT
DEFUN_DLD(NAME, args, nargout,
"MPI_ROOT               MPITB constant for corresponding MPI symbol\n\
\n\
  SEE ALSO: misc\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
