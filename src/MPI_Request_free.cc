/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Request_free
/*
 * ----------------------------------------------------
 * Frees a communication request object
 * info = MPI_Request_free (req)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_F_REQ

DEFUN_DLD(NAME, args, nargout,
"MPI_Request_free       Frees a communication request object\n\
\n\
  info = MPI_Request_free (req)\n\
\n\
  req  (int)   communication request (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS            No error\n\
      7 MPI_ERR_REQUEST        Invalid req (NULL, not pers. in MPI_Start[all])\n\
     13 MPI_ERR_ARG            Invalid argument\n\
\n\
 SEE ALSO: MPI_Start[all], MPI_Test[all,any,some], MPI_Wait[all,any,some],\n\
           MPI_[BSR]Send_init, MPI_Recv_init, MPI_I[bsr]send, MPI_Irecv\n\
\n\
  This  routine is normally used to free persistent requests created with\n\
  MPI_Recv_init or MPI_[BRS]Send_init. However, it can be used to free a\n\
  request created with MPI_Irecv or MPI_I[brs]send. In that case the user\n\
  can not use the MPI_Test/Wait[all,any,some] routines on the request.\n\
\n\
  It is permitted to free an active request. However, once freed, you\n\
  can not use the request in an MPI_Test/Wait[all,any,some] routine.\n\
\n\
  req is set to MPI_REQUEST_NULL upon completion.\n\
\n\
")

	PATN_F_REQ (NAME)

	/* info = MPI_Request_free (req) */
