/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cart_get
/*
 * ----------------------------------------------------
 * Retrieves  Cartesian  topology information associated with a  communicator
 * [info dims periods coords] = MPI_Cart_get (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTopo.h"		// PATN_3INTVEC_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Cart_get		Retrieves cartesian topo. info. associated with comm.\n\
\n\
  [info dims periods coords] = MPI_Cart_get (comm)\n\
\n\
  comm   (int)	communicator with cartesian structure (handle)\n\
\n\
  dims(intvec)	number  of  processes  for each cartesian dimension\n\
  periods(vec)	periodicity (true/false) for each cartesian dimension\n\
  coords (vec)	coordinates  of calling process in cartesian structure\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cart_create, MPI_Cartdim_get, MPI_Cart_map,\n\
            topo\n\
\n\
")

	PATN_3INTVEC_F_COM (NAME)

	/* [info dims periods coords] = MPI_Cart_get (comm) */
