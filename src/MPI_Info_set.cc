/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_set
/*
 * ----------------------------------------------------
 * Set a (key, value) pair in an MPI_Info object
 * info = MPI_Info_set (inf, key, val)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_F_INF_STR_STR

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_set		Set a (key, value) pair in an MPI_Info object\n\
\n\
  info = MPI_Info_set (inf, key, val)\n\
\n\
  inf  (int)	info object (handle created with MPI_Info_create)\n\
  key  (str)	character string of the index key\n\
  val  (str)	character string of the value\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
     33 MPI_INFO_KEY	Invalid key (null, empty, > MPI_MAX_INFO_KEY chars)\n\
     29 MPI_INFO_NOKEY	Key looked up on MPI_Info object was not found\n\
     17 MPI_ERR_INTERN	This is fatal. Please send a bug report to the LAM team\n\
\n\
  SEE ALSO: MPI_Info_delete, MPI_Info_get, MPI_Info_create, MPI_Info_free,\n\
            info\n\
\n\
")

	PATN_F_INF_STR_STR (NAME)

	/* info = MPI_Info_set (inf, key, val) */
