/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Waitall
/*
 * ----------------------------------------------------
 * Waits for all given communications to complete
 * [info stats] = MPI_Waitall (req [,req]...)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_STS_F_RQS

DEFUN_DLD(NAME, args, nargout,
"MPI_Waitall            Waits for all given communications to complete\n\
\n\
  [info stats] = MPI_Waitall (req [,req]...)\n\
\n\
  req  (int)   communication requests (handles returned by _I[bsr]send/_Irecv)\n\
\n\
  stats(struct vec) returned vector of status objects (one for each request)\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS            No error\n\
      7 MPI_ERR_REQUEST        Invalid req (NULL, not pers. in MPI_Start[all])\n\
     13 MPI_ERR_ARG            Invalid argument\n\
      2 MPI_ERR_COUNT          Invalid count argument (internal to .oct file)\n\
     18 MPI_ERR_IN_STATUS      The  actual  error value is in stats\n\
     19 MPI_ERR_PENDING        Pending request (not an error)\n\
\n\
  SEE ALSO: MPI_Test [all, some, any], MPI_Wait [some, any], MPI_Start [all]\n\
            nonblocking\n\
\n\
  For send operations, the only use of status is  for  MPI_Test_cancelled\n\
  in  the  case that there is an error, in which case the MPI_ERROR field\n\
  of status will be set\n\
\n\
")

	PATN_STS_F_RQS (NAME)

	/* [info stats] = MPI_Waitall (req [,req]...) */
