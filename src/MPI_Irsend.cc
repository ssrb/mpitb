/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Irsend
/*
 * ----------------------------------------------------
 * Begins a nonblocking ready send
 * [info req] = MPI_Irsend (buf, dest, tag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hSend.h"		// PATN_REQ_F_B2IC

DEFUN_DLD(NAME, args, nargout,
"MPI_Irsend             Starts a nonblocking ready send\n\
\n\
  [info req] = MPI_Irsend (buf, dest, tag, comm)\n\
\n\
  buf  (var) Octave variable used as buffer\n\
  dest (int) rank of destination\n\
  tag  (int) message tag\n\
  comm (int) communicator (handle)\n\
\n\
  req  (int) returned communication request (handle)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      4 MPI_ERR_TAG    Invalid  tag  argument (0..MPI_TAG_UB attribute)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (0..Comm_size-1)\n\
     16 MPI_ERR_OTHER  LAM/MPI is unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Rsend, MPI_Test (_Testall/any/some), MPI_Wait (all/any/some)\n\
\n\
  The communication started with this function is not guaranteed to progress\n\
  or complete until req has been given to one of the test or wait functions\n\
\n\
")

	PATN_REQ_F_B2IC (NAME)

	/* [info req] = MPI_Irsend (buf, dest, tag, comm) */
