/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_get_valuelen
/*
 * ----------------------------------------------------
 * Get the length of a value for a given key in an MPI_Info object
 * [info len flag] = MPI_Info_get_valuelen (inf, key)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_2INT_F_INF_STR

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_get_valuelen	Get length of value for a given key in MPI_Info object\n\
\n\
  [info len flag] = MPI_Info_get_valuelen (inf, key)\n\
\n\
  inf  (int)	info object (handle created with MPI_Info_create)\n\
  key  (str)	character string of the index key\n\
\n\
  len  (int)	length of the value associated with key (bytes)\n\
  flag (int)	true (1) if key defined on inf, false (0) if not (logical)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
     33 MPI_INFO_KEY	Invalid key (null, empty, > MPI_MAX_INFO_KEY chars)\n\
\n\
  SEE ALSO: MPI_Info_get, MPI_Info_get_nkeys, MPI_Info_get_nthkey\n\
            info\n\
\n\
  The length returned does not include the end-of-string character.\n\
  If the key is not found on inf, valuelen is left alone.\n\
\n\
")

	PATN_2INT_F_INF_STR (NAME)

	/* [info len flag] = MPI_Info_get_valuelen (inf, key) */
