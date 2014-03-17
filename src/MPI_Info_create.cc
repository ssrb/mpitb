/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_create
/*
 * ----------------------------------------------------
 * Create and initialize an empty MPI_Info object
 * [info inf]= MPI_Info_create
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_INF_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_create	Create and initialize an empty MPI_Info object\n\
\n\
  [info inf] = MPI_Info_create\n\
\n\
  inf  (int)	info object (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Info_free, MPI_Info_set, MPI_Info_get, MPI_Info_delete,\n\
            info\n\
\n\
  Upon  successful  return,  the  created MPI_Info object will contain no\n\
  (key, value) pairs\n\
\n\
  When an info object is no longer being used, it should  be  freed  with\n\
  MPI_Info_free\n\
\n\
")

	PATN_INF_F_VOID (NAME)

	/* [info inf]= MPI_Info_create */
