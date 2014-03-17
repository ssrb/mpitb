/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME	MPI_Finalize
/*
 * ----------------------------------------------------
 * Terminates MPI execution environment
 * info = MPI_Finalize
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Finalize           Terminates MPI execution environment\n\
\n\
  info = MPI_Finalize\n\
\n\
  info(int) return code\n\
      0 MPI_SUCCESS    No error\n\
     17 MPI_ERR_INTERN This is fatal. Please send bug report to LAM\n\
     16 MPI_ERR_OTHER  A different thread attempts to finalize MPI than\n\
                       the thread that initialized MPI\n\
\n\
  SEE ALSO: MPI_Init, MPI_Initialized, MPI_Finalized\n\
            misc\n\
\n\
")

	PATN_F_VOID (NAME)

	/* info = MPI_Finalize */
