/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graph_neighbors
/*
 * ----------------------------------------------------
 * Returns the neighbors of a node associated  with a graph topology
 * [info neighs] = MPI_Graph_neighbors (comm, rank)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INTVEC_NEIGH_F_COM_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Graph_neighbors	Returns neighbors of node in comm. with graph topology\n\
\n\
  [info neighs] = MPI_Graph_neighbors (comm, rank)\n\
\n\
  comm   (int)	communicator with graph topology (handle)\n\
  rank   (int)	rank of process whose neighbors are returned\n\
\n\
  neighs(intvec) ranks of processes neighbors to specified process\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
      6 MPI_ERR_RANK		Invalid rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Graphdims_create, MPI_Graphdims_get, MPI_Graph_neighbors_count\n\
            topo\n\
\n\
")

	PATN_INTVEC_NEIGH_F_COM_INT (NAME)

	/* [info neighs] = MPI_Graph_neighbors (comm, rank) */
