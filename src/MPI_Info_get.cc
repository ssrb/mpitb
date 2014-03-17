/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_get
/*
 * ----------------------------------------------------
 * Get a (key, value) pair from an MPI_Info object
 * [info flag val] = MPI_Info_get (inf, key)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_INT_STR_F_INF_STR

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_get		Get a (key, value) pair from an MPI_Info object\n\
\n\
  [info flag val] = MPI_Info_get (inf, key)\n\
\n\
  inf  (int)	info object (handle created with MPI_Info_create)\n\
  key  (str)	character string of the index key\n\
\n\
  flag (int)	true (1) if key defined on info , false (0) if not (logical)\n\
  val  (str)	character string of the value associated to key in inf\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
     33 MPI_INFO_KEY	Invalid key (null, empty, > MPI_MAX_INFO_KEY chars)\n\
\n\
  SEE ALSO: MPI_Info_set, MPI_Info_delete, MPI_Info_create, MPI_Info_free,\n\
            info\n\
\n\
")

	PATN_INT_STR_F_INF_STR (NAME)

	/* [info flag val] = MPI_Info_get (inf, key) */
