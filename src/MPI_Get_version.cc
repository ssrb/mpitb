/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Get_version
/*
 * ----------------------------------------------------
 * Obtain the version number of MPI that LAM/MPI supports
 * [info major minor] = MPI_Get_version
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_2INT_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Get_version       Obtain the version number of MPI that LAM/MPI supports\n\
\n\
  [info major minor] = MPI_Get_version\n\
\n\
  major (int) major      version number\n\
  minor (int) minor (sub)version number\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
\n\
  SEE ALSO: environ, misc\n\
\n\
")

	PATN_2INT_F_VOID (NAME)

	/* [info major minor] = MPI_Get_version */
