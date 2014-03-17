/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Get_processor_name
/*
 * ----------------------------------------------------
 * Gets the name of the processor
 * [info name] = MPI_Get_processor_name
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_STR_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Get_processor_name Gets the name of the processor\n\
\n\
  [info name] = MPI_Get_processor_name\n\
\n\
  name (str) A unique specifier for the actual (as opposed to virtual) node\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     16 MPI_ERR_OTHER  Other error; use MPI_Error_string for more information\n\
\n\
 SEE ALSO: environ\n\
\n\
")

	PATN_STR_F_VOID (NAME)

	/* [info name] = MPI_Get_processor_name */
