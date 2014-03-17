/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	IMPI_CLIENT_COLOR
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * MPI_Attr_get(MPI_COMM_WORLD, IMPI_CLIENT_COLOR)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#define   NAME	IMPI_CLIENT_COLOR
GEN_1_INT(NAME)			  // works if #defined MPI... (LAM/MPI), and
#undef		IMPI_CLIENT_COLOR // does not harm if enum{MPI...} (OpenMPI)
DEFUN_DLD(NAME, args, nargout,
"IMPI_CLIENT_COLOR      MPITB constant for corresponding MPI symbol\n\
\n\
  SEE ALSO: MPI_ATTRKVALS\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
