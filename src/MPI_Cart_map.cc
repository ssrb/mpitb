/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_map
/*
 * ----------------------------------------------------
 * Maps process to Cartesian topology information
 * Low-level topo function. Don't use it.
 * [info nrank] = MPI_Cart_map (comm, dims, periods)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INT_F_COM_2INTVEC

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_map		Maps process to cartesian topology information\n\
\n\
  [info nrank] = MPI_Cart_map (comm, dims, periods)\n\
\n\
  comm   (int)	input communicator (handle)\n\
  dims(intvec)	number of processes in each coordinate direction\n\
  periods(vec)	whether grid is periodic (true) or not (false) in each dim\n\
\n\
  nrank  (int)	reordered rank of the calling process (in new cart.topo.)\n\
		MPI_UNDEFINED if calling process does not belong to grid\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     12 MPI_ERR_DIMS		Illegal dimension argument (NULL, <=0 ?)\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cart_create, MPI_Cartdim_get, MPI_Cart_get,\n\
            topo\n\
\n\
  This is considered a low-level topology function, used to implement others\n\
  End user would not call it unless she is creating additional topologies\n\
\n\
  According to the standard (MPI 1.2 Chap.6, p.188), MPI_Cart_map computes\n\
  the `optimal' placement for the calling process on the physical machine.\n\
  A possible implementation of this function is to always return the rank\n\
  of the calling process, that is, not to perform any reordering.\n\
\n\
  See the final note on MPI_Cart_create `LAM currently ignores reorder flag'\n\
\n\
")

	PATN_INT_F_COM_2INTVEC (NAME)

	/* [info nrank] = MPI_Cart_map (comm, dims, periods) */
