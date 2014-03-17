/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_shift
/*
 * ----------------------------------------------------
 * Returns  the  shifted source and destination ranks,
 *        given a  shift direction and amount
 * [info rsrc rdst] = MPI_Cart_shift (comm, dir, disp)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_2INT_F_COM_2INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_shift		Returns src/dst ranks, given shift direction and amount\n\
\n\
  [info rsrc rdst] = MPI_Cart_shift (comm, dir, disp)\n\
\n\
  comm (int)	communicator with cartesian structure (handle)\n\
  dir  (int)	coordinate dimension of shift (0..ndims-1)\n\
  disp (int)	displacement (> 0: upwards shift, < 0: downwards shift)\n\
		routing displacement between -dim[dir]+1 .. dim[dir]-1\n\
\n\
  rsrc (int)	rank of source process      (for this node with this routing)\n\
  rdst (int)	rank of destination process (for this node with this routing)\n\
		rsrc/rdst may be MPI_PROC_NULL when cart.topo. not periodic\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
     12 MPI_ERR_DIMS		Illegal dimension argument (NULL, <=0 ?)\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cart_create, MPI_Cart_get, MPI_Cart_rank, MPI_Cart_coords,\n\
            topo\n\
\n\
  handy to use with MPI_Sendrecv[_replace]\n\
\n\
")

	PATN_2INT_F_COM_2INT (NAME)

	/* [info rsrc rdst] = MPI_Cart_shift (comm, dir, disp) */
