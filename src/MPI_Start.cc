/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Start
/*
 * ----------------------------------------------------
 * Initiates a communication with a persistent request handle
 * info = MPI_Start (req)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_F_REQ

DEFUN_DLD(NAME, args, nargout,
"MPI_Start              Initiates a communic. with a persistent request handle\n\
\n\
  info = MPI_Start (req)\n\
\n\
  req  (int)   communication request (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS            No error\n\
     13 MPI_ERR_ARG            Invalid argument\n\
      7 MPI_ERR_REQUEST        Invalid req (NULL, not pers. in MPI_Start[all])\n\
\n\
  SEE ALSO: MPI_Startall, MPI_Test[all,any,some], MPI_Wait[all,any,some],\n\
            MPI_[BSR]Send_init, MPI_Request_free\n\
\n\
  This function starts a communication that  was  previously  initialized\n\
  with any of the MPI_*_init functions.\n\
\n\
")

	PATN_F_REQ (NAME)

	/* info = MPI_Start (req) */
