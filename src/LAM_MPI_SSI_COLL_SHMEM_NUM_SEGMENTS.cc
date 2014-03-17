/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI symbol
 * MPI_Attr_get(MPI_COMM_WORLD, LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS)
 * ----------------------------------------------------
 * In order to be more or less implementation-independent,
 * we try to define all attrkvals that we know of in all implementations
 * and make them MPI_ANY_TAG==-1 if not defined in this implementation
 * which will make it fail if used in the incorrect implementation
 * CAVEAT: notice we rely on the implementation #defining all these symbols
 * (ie, they're not constants or functions)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_INT/RET_INT system

#ifndef	LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS
#define	LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS MPI_ANY_TAG
#endif

#define   NAME	LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS
GEN_1_INT(NAME)					    // works if #defined &
#undef		LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS // doesn't harm if enum
DEFUN_DLD(NAME, args, nargout,
"LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS  MPITB constant for corresp. MPI symbol\n\
\n\
  SEE ALSO: MPI_ATTRKVALS\n\
\n\
")
{
	NARGCHK (NAME,0)
	RET_1_INT
}
