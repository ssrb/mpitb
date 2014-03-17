/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cartdim_get
/*
 * ----------------------------------------------------
 * Retrieves Cartesian topology information associated with a  communicator
 * [info ndims] = MPI_Cartdim_get (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"	// PATN_INT_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Cartdim_get	Retrieves cartesian topo. info. associated with comm.\n\
\n\
  [info ndims] = MPI_Cartdim_get (comm)\n\
\n\
  comm   (int)	communicator with cartesian structure (handle)\n\
  ndims  (int)	number of dimensions of the cartesian structure\n\
\n\
  info   (int)	return code\n\
      0 MPI_SUCCESS		No error\n\
      5 MPI_ERR_COM		Invalid communicator\n\
     11 MPI_ERR_TOPOLOGY	Invalid topology\n\
     13 MPI_ERR_ARG		Invalid argument (typically a NULL pointer)\n\
\n\
  SEE ALSO: MPI_Cart_create, MPI_Cart_map, MPI_Cart_get,\n\
            topo\n\
\n\
  In C language, this call is useful to allocate memory for MPI_Cart_get\n\
  In MPITB, MPI_Cart_get calls itself to MPI_Cartdim_get to retrieve info\n\
\n\
")

	PATN_INT_F_COM (NAME)

	/* [info ndims] = MPI_Cartdim_get (comm) */
