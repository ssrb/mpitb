/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_free
/*
 * ----------------------------------------------------
 * Free an MPI_Info object
 * info = MPI_Info_free (inf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_F_INF

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_free		Free an MPI_Info object\n\
\n\
  info = MPI_Info_free (inf)\n\
\n\
  inf  (int)	info object to be freed (handle). MPI_INFO_NULL not valid\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Info_create, MPI_Info_set, MPI_Info_get, MPI_Info_delete,\n\
            info\n\
\n\
  Upon successful completion, info will be set to MPI_INFO_NULL\n\
\n\
")

	PATN_F_INF (NAME)

	/* info = MPI_Info_free (inf) */
