/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Dims_create
/*
 * ----------------------------------------------------
 * Creates a division of processors in a cartesian grid
 * [info dims] = MPI_Dims_create (nnodes, ndims)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INTVEC_F_2INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Dims_create	Creates a division of processors in a cartesian grid\n\
\n\
  [info dims] = MPI_Dims_create (nnodes, ndims)\n\
\n\
  nnodes (int)	number of nodes in a grid\n\
  ndims  (int)	number of cartesian dimensions\n\
\n\
  dims(intvec)	number of nodes in each dimension\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     12 MPI_ERR_DIMS	Illegal dimension argument (NULL, <=0 ?)\n\
     13 MPI_ERR_ARG	Invalid argument (typically a NULL pointer)\n\
     16 MPI_ERR_OTHER	LAM/MPI implementation is unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Cart_create, MPI_Cartdim_get, MPI_Cart_map,\n\
            topo\n\
\n\
")

	PATN_INTVEC_F_2INT (NAME)

	/* [info dims] = MPI_Dims_create (nnodes, ndims) */
