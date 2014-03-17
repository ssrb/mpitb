/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Test_cancelled
/*
 * ----------------------------------------------------
 * Tests to see if a request was cancelled
 * [info flag stat] = MPI_Test_cancelled (stat)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_INT_STA_F_STA

DEFUN_DLD(NAME, args, nargout,
"MPI_Test_cancelled     Tests to see if a request was cancelled\n\
\n\
  [info flag statout] = MPI_Test_cancelled (statin)\n\
\n\
  stat(struc)status object\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  flag (int) true (1) if the request has been cancelled, false (0) otherwise\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
\n\
  SEE ALSO: MPI_Cancel\n\
            cancel\n\
\n\
  a copy of the possibly-modified-by-MPI stat struct is returned in 3rd argout\n\
\n\
")

	PATN_INT_STA_F_STA (NAME)

	/* [info flag stat] = MPI_Test_cancelled (stat) */
