/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Startall
/*
 * ----------------------------------------------------
 * Starts a collection of requests
 * info = MPI_Starttall (req [,req]...)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_F_RQS

DEFUN_DLD(NAME, args, nargout,
"MPI_Startall           Starts a collection of requests\n\
\n\
  info = MPI_Startall (req [,req]...)\n\
\n\
  req  (int)   communication requests (handles)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS            No error\n\
     13 MPI_ERR_ARG            Invalid argument\n\
      2 MPI_ERR_COUNT          Invalid count argument (internal to .oct file)\n\
      7 MPI_ERR_REQUEST        Invalid req (NULL, not pers. in MPI_Start[all])\n\
\n\
  SEE ALSO: MPI_Start, MPI_Test[all,any,some], MPI_Wait[all,any,some],\n\
            MPI_[BSR]Send_init, MPI_Request_free\n\
\n\
  This function starts a collection of communications that were previously\n\
  initialized with any of the MPI_*_init functions.\n\
\n\
")

	PATN_F_RQS (NAME)

	/* info = MPI_Starttall (req [,req]...) */
