/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_sub
/*
 * ----------------------------------------------------
 * Partitions  a communicator into subgroups which  form
 *        lower-dimensional cartesian subgrids
 * [info comm_sub] = MPI_Cart_sub (comm, dims)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_COM_F_COM_INTVEC

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_sub		Partitions cartesian comm. into lower-dim. subgroups\n\
\n\
  [info comm_sub] = MPI_Cart_sub (comm, dims)\n\
\n\
  comm (int)    communicator with cartesian structure (handle)\n\
  rdims(intvec) boolean flags marking remanent dimensions\n\
                expected to be MPI_Cartdim_get elements long\n\
\n\
  comm_sub(int) communicator containing subgrid that includes calling process\n\
\n\
	Example:  cartesian grid (2x3x4) 24 processes\n\
	rdims=[1 0 1] -> obtained   (3) 3 comms. of (2x4) 8 processes\n\
	rdims=[0 0 1] -> obtained (2x3) 6 comms. of   (4) 4 processes\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cartdim_get, MPI_Cart_get, MPI_Cart_rank, MPI_Cart_coords,\n\
            topo\n\
\n\
  The i-th entry of remdims specifies whether the i-th dimension is kept\n\
  in the subgrid (true) or is dropped (false)\n\
\n\
  As many sub-comms are obtained as the product of removed dimensions\n\
  MPI_Cart_sub is a collective operation on comm\n\
\n\
")

	PATN_COM_F_COM_INTVEC (NAME)

	/* [info comm_sub] = MPI_Cart_sub (comm, dims) */
