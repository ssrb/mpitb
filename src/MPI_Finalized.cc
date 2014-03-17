/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Finalized
/*
 * ----------------------------------------------------
 * Indicates whether MPI_Finalize has completed
 * [info flag] = MPI_Finalized
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Finalized          Indicates whether MPI_Finalize has completed\n\
\n\
  [info flag] = MPI_Finalized\n\
\n\
  flag(int) 0 false\n\
            1 true\n\
\n\
  info(int) return code\n\
      0 MPI_SUCCESS    This function always returns MPI_SUCCESS\n\
\n\
  SEE ALSO: MPI_Init, MPI_Finalize, MPI_Initialized\n\
            misc\n\
\n\
")

	PATN_INT_F_VOID (NAME)

	/* [info flag] = MPI_Finalized */
