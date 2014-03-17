/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Recv
/*
 * ----------------------------------------------------
 * Performs a basic receive
 * [info stat] = MPI_Recv (buf, src, tag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hRecv.h"		// PATN_MAP_F_BUF_2INT_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Recv               Performs a basic receive\n\
\n\
  [info stat] = MPI_Recv (buf, src, tag, comm)\n\
\n\
  buf  (var) Octave variable used as buffer\n\
  src  (int) rank of source\n\
  tag  (int) message tag\n\
  comm (int) communicator (handle)\n\
\n\
  stat(struc)status object\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      4 MPI_ERR_TAG    Invalid tag  argument (MPI_ANY_TAG,0..MPI_TAG_UB attr)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (MPI_ANY_SOURCE, 0..Comm_size-1)\n\
\n\
  SEE ALSO: MPI_Send, MPI_Probe, MPI_Iprobe, MPI_Irecv, MPI_Recv_init\n\
\n\
")

	PATN_MAP_F_BUF_2INT_COM (NAME)

	/* [info stat] = MPI_Recv (buf, src, tag, comm) */
