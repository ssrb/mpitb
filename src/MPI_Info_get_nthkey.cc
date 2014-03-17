/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_get_nthkey
/*
 * ----------------------------------------------------
 * Get  a key indexed by integer from an MPI_Info object
 * [info key] = MPI_Info_get_nthkey (inf, n)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_STR_F_INF_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_get_nthkey	Get a key indexed by integer from an MPI_Info object\n\
\n\
  [info key] = MPI_Info_get_nthkey (inf, n)\n\
\n\
  inf  (int)	info object (handle created with MPI_Info_create)\n\
  n    (int)	index of key to retrieve (0..MPI_Info_get_nkeys-1)\n\
\n\
  key  (str)	character string of nth entry in inf (<=MPI_MAX_INFO_KEY chars)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Info_get_nkeys, MPI_Info_get, MPI_Info_create, MPI_Info_delete\n\
            info\n\
\n\
")

	PATN_STR_F_INF_INT (NAME)

	/* [info key] = MPI_Info_get_nthkey (inf, n) */
