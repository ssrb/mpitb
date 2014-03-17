/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_IN_PLACE
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI pointer symbol
 * ----------------------------------------------------
 * In order to be more or less implementation-independent,
 * we try to define all symbols that we know of in all implementations
 * and make them MPI_SUCCESS==0 if not defined in this implementation
 * (or MPI_ERRCODES_IGNORE=((int *) 0) if it's a pointer, instead of an int)
 * CAVEAT: notice we rely on the implementation #defining all these symbols
 * (ie, they're not constants or functions)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#ifndef MPI_IN_PLACE
#define MPI_IN_PLACE MPI_ERRCODES_IGNORE
#endif

#define   NAME	MPI_IN_PLACE
GEN_1_PTR(NAME)
#undef		MPI_IN_PLACE
DEFUN_DLD(NAME, args, nargout,
"MPI_IN_PLACE           MPITB constant for corresponding MPI pointer symbol\n\
\n\
  SEE ALSO: misc\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_PTR
}
