/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Initialized
/*
 * ----------------------------------------------------
 * Indicates whether MPI_Init has been called
 * [info flag] = MPI_Initialized
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Initialized        Indicates whether MPI_Init has been called\n\
\n\
  [info flag] = MPI_Initialized\n\
\n\
  flag(int) 0 false\n\
            1 true\n\
\n\
  info(int) return code\n\
      0 MPI_SUCCESS    This function always returns MPI_SUCCESS\n\
\n\
  SEE ALSO: MPI_Init, MPI_Finalize,\n\
            misc\n\
\n\
")

	PATN_INT_F_VOID (NAME)

	/* [info flag] = MPI_Initialized */
