/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Testany
/*
 * ----------------------------------------------------
 * Tests for completion of any previdously initiated  communication
 * [info idx flag stat] = MPI_Testany (req [,req]...)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_2INT_STA_F_REQ

DEFUN_DLD(NAME, args, nargout,
"MPI_Testany            Tests for completion of any previously initiated comm\n\
\n\
  [info idx flag stat] = MPI_Testany (req [,req]...)\n\
\n\
  req  (int)   communication requests (handles returned by _I[bsr]send/_Irecv)\n\
\n\
  idx  (int)   index of operation that completed, or MPI_UNDEFINED if none\n\
  flag (int)   true if one of the operations is complete (logical)\n\
\n\
  stat(struc)  status object\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      7 MPI_ERR_REQUEST Invalid req (NULL, not persistent in MPI_Start[all])\n\
     13 MPI_ERR_ARG    Invalid argument\n\
\n\
  SEE ALSO: MPI_Test [all, some], MPI_Wait [all, some, any], MPI_Start [all]\n\
            nonblocking\n\
\n\
  For send operations, the only use of status is  for  MPI_Test_cancelled\n\
  in  the  case that there is an error, in which case the MPI_ERROR field\n\
  of status will be set\n\
\n\
")

	PATN_2INT_STA_F_REQ (NAME)

	/* [info idx flag stat] = MPI_Testany (req [,req]...) */
