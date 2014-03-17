/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Ssend
/*
 * ----------------------------------------------------
 * Basic synchronous send
 * info = MPI_Ssend (buf, dest, tag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hSend.h"		// PATN_F_B2IC

DEFUN_DLD(NAME, args, nargout,
"MPI_Ssend              Basic synchronous send\n\
\n\
  info = MPI_Ssend (buf, dest, tag, comm)\n\
\n\
  buf  (var) Octave variable used as buffer\n\
  dest (int) rank of destination\n\
  tag  (int) message tag\n\
  comm (int) communicator (handle)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      4 MPI_ERR_TAG    Invalid  tag  argument (0..MPI_TAG_UB)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (0..Comm_size-1)\n\
\n\
  SEE ALSO: MPI_Send, MPI_Recv\n\
\n\
  This  function  will  not complete until the destination rank starts to\n\
  receive the message.  Note that this is different than saying that  the\n\
  message has been received by the destination rank.\n\
\n\
")

	PATN_F_B2IC (NAME)

	/* info = MPI_Ssend (buf, dest, tag, comm) */
