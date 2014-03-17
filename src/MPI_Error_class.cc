/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Error_class
/*
 * ----------------------------------------------------
 * Converts an error code into an error class
 * [info class] = MPI_Error_class (errcode)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Error_class        Converts an error code into an error class\n\
\n\
  [info class] = MPI_Error_class (errcode)\n\
\n\
  errcode(int) Error code returned by an MPI routine\n\
\n\
  class  (int) Error class associated with errcode\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
\n\
  SEE ALSO: MPI_Error_string, errors\n\
\n\
  Error codes are the values return by MPI routines\n\
  These can be converted into error classes with the routine MPI_Error_class\n\
  These messages are provided in English (En_US ‐ English/United States).\n\
\n\
")

	PATN_INT_F_INT (NAME, errcode)

	/* [info class] = MPI_Error_class (errcode) */
