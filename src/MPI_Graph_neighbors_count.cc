/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graph_neighbors_count
/*
 * ----------------------------------------------------
 * Returns the number of neighbors of a node associated with a graph topology
 * [info nneighs] = MPI_Graph_neighbors_count (comm, rank)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INT_F_COM_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Graph_neighbors_count	Returns #neighbors of node in graph topo. comm.\n\
\n\
  [info nneighs] = MPI_Graph_neighbors_count (comm, rank)\n\
\n\
  comm   (int)	communicator with graph topology (handle)\n\
  rank   (int)	rank of process whose no. neighbors is returned\n\
\n\
  nneighs(int)	number of neighbors of specified process\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
      6 MPI_ERR_RANK		Invalid rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Graphdims_create, MPI_Graphdims_get, MPI_Graph_neighbors,\n\
            topo\n\
\n\
  In C language, this call useful to allocate memory for MPI_Graph_neighbors\n\
  In MPITB, MPI_Graph_neighbors calls itself to this call to retrieve info\n\
\n\
")

	PATN_INT_F_COM_INT (NAME)

	/* [info nneighs] = MPI_Graph_neighbors_count (comm, rank) */
