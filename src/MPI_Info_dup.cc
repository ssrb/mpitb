/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Info_dup
/*
 * ----------------------------------------------------
 * Duplicate an MPI_Info object
 * [info newinf] = MPI_Info_dup (inf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_INF_F_INF

DEFUN_DLD(NAME, args, nargout,
"MPI_Info_dup		Duplicate an MPI_Info object\n\
\n\
  [info newinf] = MPI_Info_dup (inf)\n\
\n\
  inf    (int)	source info object (handle)\n\
  newinf (int)	newly created info object (handle)\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Info_create, MPI_Info_free,\n\
            info\n\
\n\
  Not only will the (key,value) pairs be duplicated,\n\
  the order of keys will be the same in newinf as it is in inf\n\
\n\
  When an info object is no longer being used, it should be freed with\n\
  MPI_Info_free\n\
\n\
")

	PATN_INF_F_INF (NAME)

	/* [info newinf] = MPI_Info_dup (inf) */
