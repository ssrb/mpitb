/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Probe
/*
 * ----------------------------------------------------
 * Blocking test for a message
 * [info stat] = MPI_Probe (src, tag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hTstWait.h"		// PATN_STA_F_2IC

DEFUN_DLD(NAME, args, nargout,
"MPI_Probe              Blocking test for a message\n\
\n\
  [info stat] = MPI_Probe (src, tag, comm)\n\
\n\
  src  (int) expected source rank, or MPI_ANY_SOURCE\n\
  tag  (int) expected tag value or MPI_ANY_TAG\n\
  comm (int) communicator (handle)\n\
\n\
  stat(struc)status object\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int)return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (null?)\n\
      4 MPI_ERR_TAG    Invalid tag argument (MPI_ANY_TAG, 0..MPI_TAG_UB attr)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (MPI_ANY_SOURCE, 0..Comm_size-1)\n\
\n\
  SEE ALSO: MPI_Iprobe, MPI_Recv, MPI_Irecv\n\
            cancel\n\
\n\
")

	PATN_STA_F_2IC (NAME)		// STC -> src,tag,comm

	/* [info stat] = MPI_Probe (src, tag, comm) */
