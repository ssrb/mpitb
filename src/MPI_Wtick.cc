/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Wtick
/*
 * ----------------------------------------------------
 * Returns the resolution of MPI_Wtime (in seconds)
 * res = MPI_Wtick
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_DBL_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Wtick              Returns the resolution of MPI_Wtime\n\
\n\
  res = MPI_Wtick\n\
\n\
  res (double)  Time in seconds of resolution of MPI_Wtime\n\
\n\
  SEE ALSO: misc\n\
\n\
")

	PATN_DBL_F_VOID (NAME)

	/* res = MPI_Wtick */
