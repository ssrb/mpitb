/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graph_get
/*
 * ----------------------------------------------------
 * Retrieves graph topology information associated with a communicator
 * [info index edges] = MPI_Graph_get (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_2INTVEC_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Graph_get		Retrieves graph topology info. associated with a comm.\n\
\n\
  [info index edges] = MPI_Graph_get (comm)\n\
\n\
  comm   (int)	communicator with graph structure (handle)\n\
\n\
  index(intvec) no.neighbors (node 0) accumulated with previous (node>0)\n\
  edges(intvec) neighbor ranks, as many as index[nnodes]\n\
                   node n neighbors are in edges[index[n-1]+1..index[n]]\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Graph_create, MPI_Graphdims_get, MPI_Graph_map,\n\
            topo\n\
\n\
")

	PATN_2INTVEC_F_COM (NAME)

	/* [info index edges] = MPI_Graph_get (comm) */
