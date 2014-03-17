/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Wtime
/*
 * ----------------------------------------------------
 * Returns an elapsed time on the calling processor (in seconds)
 * secs = MPI_Wtime
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_DBL_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Wtime              Returns an elapsed time on the calling processor\n\
\n\
  secs = MPI_Wtime\n\
\n\
  secs (double) Time in seconds since an arbitrary time in the past\n\
\n\
  SEE ALSO: misc\n\
\n\
")

	PATN_DBL_F_VOID (NAME)

	/* secs = MPI_Wtime */
