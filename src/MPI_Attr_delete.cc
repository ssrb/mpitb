/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Attr_delete
/*
 * ----------------------------------------------------
 * Deletes communicator attribute value associated with a key (comm,kv)
 * info = MPI_Attr_delete (comm, kv)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hAttr.h"		// PATN_F_COM_KEY

DEFUN_DLD(NAME, args, nargout,
"MPI_Attr_delete	Deletes comm. attribute value associated with a key\n\
\n\
  info = MPI_Attr_delete (comm, kv)\n\
\n\
  comm (int)	communicator to which attribute is attached (handle)\n\
  kv   (int)	key value of the attribute to delete\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
     17 MPI_ERR_INTERN This is fatal. Please send a bug report to the LAM team\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free, MPI_Attr_get, MPI_Attr_delete\n\
            caching\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_delete_attr\n\
\n\
")

	PATN_F_COM_KEY(NAME)

	/* info = MPI_Attr_delete (comm, kv) */
