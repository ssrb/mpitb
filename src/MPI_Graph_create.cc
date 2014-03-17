/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Graph_create
/*
 * ----------------------------------------------------
 * Makes a new communicator to which topology information has been attached
 * [info comm_graph] = MPI_Graph_create (comm, index, edges, reorder)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_COM_F_COM_INTVEC_INTVEC_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Graph_create	Makes new comm. with grid topology information attached\n\
\n\
  [info comm_graph] = MPI_Graph_create (comm, index, edges, reorder)\n\
\n\
  comm (int)    input communicator without topology (handle)\n\
  index(intvec) no.neighbors (node 0) accumulated with previous (node>0)\n\
  edges(intvec) neighbor ranks, as many as index[nnodes]\n\
                   or last index such that index[last]>0\n\
                node n neighbors are in edges[index[n-1]+1..index[n]]\n\
  reorder(int)  ranking may be reordered (true) or not (false)\n\
\n\
  comm_graph(int) communicator with graph topology added (handle)\n\
\n\
	Example:Rank	Neighbors\n\
		0	1, 3		nnodes =  4\n\
		1	0		index  = [2 3 4 6]\n\
		2	3		edges  = [1 3 0 3 0 2]\n\
		3	0, 2\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS            No error\n\
      5 MPI_ERR_COM            Invalid communicator\n\
     11 MPI_ERR_TOPOLOGY       Invalid topology\n\
     13 MPI_ERR_ARG            Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Graphdims_get, MPI_Graph_map,\n\
            topo\n\
\n\
  LAM/MPI currently ignores the reorder flag\n\
  If nnodes < Comm_size, returned MPI_COMM_NULL in remaining ranks\n\
  MPI_Graph_create is a collective operation on comm\n\
\n\
")

	PATN_COM_F_COM_INTVEC_INTVEC_INT (NAME)

	/* [info comm_graph] = MPI_Graph_create(comm, index, edges, reorder) */
