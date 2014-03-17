/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Cancel
/*
 * ----------------------------------------------------
 * Cancels a communication request
 * info = MPI_Cancel (req)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_F_REQ

DEFUN_DLD(NAME, args, nargout,
"MPI_Cancel             Cancels a communication request\n\
\n\
  info = MPI_Cancel (req)\n\
\n\
  req  (int) communication request (handle returned by MPI_Irecv)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS     No error\n\
      7 MPI_ERR_REQUEST Invalid req (NULL, not persistent in MPI_Start[all])\n\
     13 MPI_ERR_ARG     Invalid argument\n\
\n\
  SEE ALSO: MPI_Test[all,some,any], MPI_Wait[all,some,any], MPI_Test_cancelled\n\
            cancel\n\
\n\
  only implemented for receive requests; it is a no-op for send requests\n\
  for multibuffering schemes, where speculative MPI_Irecv calls are made\n\
\n\
")

	PATN_F_REQ (NAME)

	/* info = MPI_Cancel (req) */
