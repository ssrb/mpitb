/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Attr_get
/*
 * ----------------------------------------------------
 * Retrieves communicator attribute value by key (comm,kv)
 * [info attr flag] = MPI_Attr_get (comm, kv)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hAttr.h"		// PATN_ATR_INT_F_COM_KEY

DEFUN_DLD(NAME, args, nargout,
"MPI_Attr_get		Retrieves communicator attribute value by key\n\
\n\
  [info attr flag] = MPI_Attr_get (comm, kv)\n\
\n\
  comm (int)	communicator to which attribute is attached (handle)\n\
  kv   (int)	key value used to store attribute with MPI_Attr_put\n\
\n\
  flag (int)	1 if attr.was extracted; 0 if no attr. is associated with key\n\
  attr		Octave variable in which an attribute copy will be stored\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free, MPI_Attr_put, MPI_Attr_delete\n\
            caching, MPI_ATTR_KVAL\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_get_attr\n\
\n\
")

	PATN_ATR_INT_F_COM_KEY(NAME)

	/* [info attr flag] = MPI_Attr_get (comm, kv) */
