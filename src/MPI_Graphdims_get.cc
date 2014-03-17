/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graphdims_get
/*
 * ----------------------------------------------------
 * Retrieves  graph topology information associated with a  communicator
 * [info nnodes nedges] = MPI_Graphdims_get (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_2INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Graphdims_get	Retrieves graph topology info. associated with a comm.\n\
\n\
  [info nnodes nedges] = MPI_Graphdims_get (comm)\n\
\n\
  comm   (int)	communicator with graph structure (handle)\n\
\n\
  nnodes (int)	number of nodes in graph\n\
  nedges (int)	number of edges in graph\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Graph_create, MPI_Graph_map, MPI_Graph_get,\n\
            topo\n\
\n\
  In C language, this call is useful to allocate memory for MPI_Graph_get\n\
  In MPITB, MPI_Graph_get calls itself to MPI_Graphdims_get to retrieve info\n\
\n\
")

	PATN_2INT_F_COM (NAME)

	/* [info nnodes nedges] = MPI_Graphdims_get (comm) */
