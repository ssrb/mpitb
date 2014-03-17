/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_coords
/*
 * ----------------------------------------------------
 * Determines  process  coords  in cartesian topology, given rank in group
 * [info coords] = MPI_Cart_coords (comm, rank)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INTVEC_COORD_F_COM_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_coords	Determines process coords in cartesian topo. given rank\n\
\n\
  [info coords] = MPI_Cart_coords (comm, rank)\n\
\n\
  comm   (int)	communicator with cartesian structure (handle)\n\
  rank   (int)	rank of process whose cartesian coordinates are looked for\n\
\n\
  coords(intvec) cartesian  coordinates of specified process\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      6 MPI_ERR_RANK		Invalid rank (0..Commsize-1)\n\
     12 MPI_ERR_DIMS		Illegal dimension argument (NULL, <=0 ?)\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cartdim_get, MPI_Cart_get, MPI_Cart_rank,\n\
            topo\n\
\n\
")

	PATN_INTVEC_COORD_F_COM_INT (NAME)

	/* [info coords] = MPI_Cart_coords (comm, rank) */
