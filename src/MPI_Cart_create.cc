/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_create
/*
 * ----------------------------------------------------
 * Makes a new communicator to which topology information has been attached
 * [info comm_cart] = MPI_Cart_create (comm, dims, periods, reorder)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_COM_F_COM_2INTVEC_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_create	Makes new comm. with cartesian topology info attached\n\
\n\
  [info comm_cart] = MPI_Cart_create (comm, dims, periods, reorder)\n\
\n\
  comm   (int)	input communicator without topology (handle)\n\
  dims(intvec)	number of processes in each dimension\n\
  periods(vec)	whether grid is periodic (true) or not (false) in each dim\n\
  reorder(int)	ranking may be reordered (true) or not (false)\n\
\n\
  comm_cart(int)	communicator with new cartesian topology (handle)\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
     12 MPI_ERR_DIMS		Illegal dimension argument (NULL, <=0 ?)\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Dims_create, MPI_Cartdim_get, MPI_Cart_map,\n\
            topo\n\
\n\
  LAM/MPI currently ignores the reorder flag\n\
  If prod.of.dims < Comm_size, returned MPI_COMM_NULL in remaining ranks\n\
  MPI_Cart_create is a collective operation on comm\n\
\n\
")

	PATN_COM_F_COM_2INTVEC_INT (NAME)

	/* [info comm_cart] = MPI_Cart_create (comm, dims, periods, reorder) */
