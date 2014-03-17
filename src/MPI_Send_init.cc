/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Send_init
/*
 * ----------------------------------------------------
 * Builds a handle for a standard send
 * [info req] = MPI_Send_init (buf, dest, tag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hSend.h"		// PATN_REQ_F_B2IC

DEFUN_DLD(NAME, args, nargout,
"MPI_Send_init          Builds a handle for a standard send\n\
\n\
  [info req] = MPI_Send_init (buf, dest, tag, comm)\n\
\n\
  buf  (var) Octave variable used as buffer\n\
  dest (int) rank of destination\n\
  tag  (int) message tag\n\
  comm (int) communicator (handle)\n\
\n\
  req  (int) communication request (handle)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (0..Comm_size-1)\n\
      4 MPI_ERR_TAG    Invalid  tag  argument (0..MPI_TAG_UB attribute)\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Start[all], MPI_Test[all,any,some], MPI_Wait[all,any,some],\n\
            MPI_Send\n\
\n\
  The  communication  initialized  by  this  function will not be started\n\
  until req is given to MPI_Start or MPI_Startall .\n\
  The communication is then not guaranteed to progress or complete  until\n\
  req  has  been  given to one of the test or wait functions.\n\
\n\
  Once  any of the test or wait functions indicate that the communication\n\
  has finished,  the  communication  cycle  can  be  started  again  with\n\
  MPI_Start or MPI_Startall .\n\
  However,  once the communication has finished and it is no longer going\n\
  to be used, req should be freed with MPI_Request_free .\n\
\n\
")

	PATN_REQ_F_B2IC (NAME)

	/* [info req] = MPI_Send_init (buf, dest, tag, comm) */
