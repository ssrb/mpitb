/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Sendrecv_replace
/*
 * ----------------------------------------------------
 * Sends and receives using a single buffer
 * [info stat] = MPI_Sendrecv_replace (buf, dest, stag, ...
 *                                          src , rtag, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hRecv.h"		// PATN_MAP_F_2_B2IC_RP

DEFUN_DLD(NAME, args, nargout,
"MPI_Sendrecv_replace   Sends and receives using a single buffer\n\
\n\
  [info stat] = MPI_Sendrecv_replace (buf, dest, stag, ...\n\
                                           src , rtag, comm)\n\
\n\
  buf       (var) Octave variable used as both send/recv buffer\n\
  src ,dest (int) rank of source/destination\n\
  stag,rtag (int) send/receive tags\n\
  comm      (int) communicator (handle)\n\
\n\
  stat    (struc) returned status object\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      4 MPI_ERR_TAG    Invalid tag  argument (MPI_ANY_TAG, 0..MPI_TAG_UB attr)\n\
      6 MPI_ERR_RANK   Invalid src/dst rank (MPI_ANY_SOURCE, 0..Comm_size-1)\n\
     15 MPI_ERR_TRUNCATE       Message truncated on receive\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
\n\
  SEE ALSO: MPI_Sendrecv\n\
\n\
  common misconception: src and dest do not have to be the same\n\
  common mistake: to mismatch the src/dst tags, which can result in deadlock\n\
  This function is guaranteed not to deadlock in situations  where  pairs\n\
  of  blocking sends and receives may deadlock.\n\
\n\
")

	PATN_MAP_F_2_B2IC_RP (NAME)

	/* [info stat] = MPI_Sendrecv_replace (buf, dest, stag, ...
	 *                                          src , rtag, comm)
	 */
