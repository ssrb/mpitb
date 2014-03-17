/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Topo_test
/*
 * ----------------------------------------------------
 * Determines  the type of topology (if any) associated with a  communicator
 * [info topo] = MPI_Topo_test (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Topo_test		Determines topology (if any) associated with a comm.\n\
\n\
  [info topo] = MPI_Topo_test (comm)\n\
\n\
  comm (int)	communicator (handle)\n\
\n\
  topo (int)	topology type of communicator comm\n\
      1 MPI_CART	cartesian\n\
      2 MPI_GRAPH	graph-defined\n\
 -32766 MPI_UNDEFINED	undefined\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COM	Invalid communicator\n\
     13 MPI_ERR_ARG	Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Graph_create, MPI_Cart_create,\n\
            topo\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info topo] = MPI_Topo_test (comm) */
