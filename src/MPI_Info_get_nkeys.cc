/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_get_nkeys
/*
 * ----------------------------------------------------
 * Returns the number of keys defined on an MPI_Info object
 * [info nkeys] = MPI_Info_get_nkeys (inf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_INT_F_INF

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_get_nkeys	Returns the number of keys defined on MPI_Info object\n\
\n\
  [info nkeys] = MPI_Info_get_nkeys (inf)\n\
\n\
  inf   (int)	info object (handle created with MPI_Info_create)\n\
\n\
  nkeys (int)	number of keys defined on inf\n\
\n\
  info  (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Info_set, MPI_Info_get, MPI_Info_create, MPI_Info_delete,\n\
            info\n\
\n\
")

	PATN_INT_F_INF (NAME)

	/* [info nkeys] = MPI_Info_get_nkeys (inf) */
