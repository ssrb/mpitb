/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_rank
/*
 * ----------------------------------------------------
 * Determines process rank in communicator given Cartesian location
 * [info rank] = MPI_Cart_rank (comm, coords)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INT_F_COM_INTVEC

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_rank		Determines proc. rank in comm. given cartesian location\n\
\n\
  [info rank] = MPI_Cart_rank (comm, coords)\n\
\n\
  comm  (int)    communicator with cartesian structure (handle)\n\
  coords(intvec) cartesian coordinates of process whose rank is looked for\n\
                 expected vector of MPI_Cartdim_get dimensions\n\
\n\
  rank   (int)	rank of specified process\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cartdim_get, MPI_Cart_get, MPI_Cart_coords,\n\
            topo\n\
\n\
")

	PATN_INT_F_COM_INTVEC (NAME)

	/* [info rank] = MPI_Cart_rank (comm, coords) */
