/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Abort
/*
 * ----------------------------------------------------
 * Terminates MPI execution environment
 * info = MPI_Abort (comm, errcode)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_F_COM_INT

DEFUN_DLD(NAME, args, nargout,
"MPI_Abort              Terminates MPI execution environment\n\
\n\
  info = MPI_Abort (comm, errcode)\n\
\n\
  comm   (int) communicator handle of tasks to abort (handle)\n\
               for non-IMPI jobs, this will terminate all processes\n\
  errcode(int) error code to return to invoking environment\n\
               split into 3 sections:\n\
               lower 8 bits: error class\n\
               next 9 bits: function enumeration (over  300  MPI functions)\n\
               upper 15 bits: error code (POSIX return value for the program)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
\n\
  SEE ALSO: MPI_Init, MPI_Finalize\n\
            misc\n\
\n\
")

	PATN_F_COM_INT (NAME, errcode)

	/* info = MPI_Abort (comm, errcode) */
