/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Errhandler_set
/*
 * ----------------------------------------------------
 * Sets the error handler for a communicator
 * info = MPI_Errhandler_set (comm, eh)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hErr.h"		// PATN_F_COM_ERRH

DEFUN_DLD(NAME, args, nargout,
"MPI_Errhandler_set     Sets the error handler for a communicator\n\
\n\
  info = MPI_Errhandler_set (comm, eh)\n\
\n\
  comm (int) communicator to set the error handler for (handle)\n\
  eh   (int) new MPI error handler for communicator (handle)\n\
               see MPI_ERRORS_ARE_FATAL, MPI_ERRORS_RETURN,\n\
               and MPI_Errhandler_create\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    no error\n\
      5 MPI_ERR_COMM   invalid communicator (null handle?)\n\
     13 MPI_ERR_ARG    invalid argument (null pointer?)\n\
\n\
  SEE ALSO: MPI_Errhandler_get, MPI_Errhandler_create, MPI_Errhandler_free,\n\
               errors\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_set_errhandler\n\
\n\
  If an error occurs in an MPI function, the current MPI error handler is\n\
  called to handle it. By default, this error handler aborts the MPI job.\n\
  The error handler may be changed with MPI_Errhandler_set; the predefined\n\
  error handler MPI_ERRORS_RETURN may be used to cause error values to be\n\
  returned.\n\
\n\
  Note that MPI does not guarantee that an MPI program can continue\n\
  past an error.\n\
\n\
")

	PATN_F_COM_ERRH (NAME)

	/* info = MPI_Errhandler_set (comm, eh) */
