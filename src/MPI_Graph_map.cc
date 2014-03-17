/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graph_map
/*
 * ----------------------------------------------------
 * Maps process to graph topology information
 * Low-level topo function. Don't use it.
 * [info nrank] = MPI_Graph_map (comm, index, edges)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_INT_F_COM_INTVEC_INTVEC

DEFUN_DLD(NAME, args, nargout,
"MPI_Graph_map		Maps process to graph topology information\n\
\n\
  [info nrank] = MPI_Graph_map (comm, index, edges)\n\
\n\
  comm (int)    input communicator (handle)\n\
  index(intvec) no.neighbors (node 0) accumulated with previous (node>0)\n\
  edges(intvec) neighbor ranks, as many as index[nnodes]\n\
                   node n neighbors are in edges[index[n-1]+1..index[n]]\n\
\n\
  nrank  (int)	reordered rank of the calling process in new graph topology\n\
		MPI_UNDEFINED if the calling process does not belong to graph\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Graph_create, MPI_Graphdims_get, MPI_Graph_get,\n\
            topo\n\
\n\
  This is considered a low-level topology function, used to implement others\n\
  End user would not call it unless she is creating additional topologies\n\
\n\
  See the final note on MPI_Graph_create `LAM currently ignores reorder flag'\n\
\n\
")

	PATN_INT_F_COM_INTVEC_INTVEC (NAME)

	/* [info nrank] = MPI_Graph_map (comm, index, edges) */
