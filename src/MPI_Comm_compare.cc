/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_compare
/*
 * ----------------------------------------------------
 * Compares two communicators
 * [info result] = MPI_Comm_compare (comm1, comm2)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_INT_F_2COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_compare	Compares two communicators\n\
\n\
  [info result] = MPI_Comm_compare (comm1, comm2)\n\
\n\
  commX  (int)	communicators (handles). MPI_COMM_NULL is not valid\n\
\n\
  result (int)	comparison code\n\
      1 MPI_IDENT	contexts and groups are the same\n\
      2 MPI_CONGRUENT	different contexts but identical groups\n\
      3 MPI_SIMILAR	different contexts but  similar  groups\n\
      4 MPI_UNEQUAL	otherwise\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG	Invalid argument (typically  a  NULL pointer)\n\
\n\
  SEE ALSO: MPI_Group_compare, MPI_Comm_size, MPI_Comm_rank\n\
            comms, groups\n\
\n\
  It is an error  to  use  MPI_COMM_NULL  as  one  of  the  arguments  to\n\
  MPI_Comm_compare .  The relevant sections of the MPI standard are\n\
  (2.4.1  Opaque Objects) and (5.4.1. Communicator Accessors)\n\
\n\
")

	PATN_INT_F_2COM (NAME)

	/* [info result] = MPI_Comm_compare (comm1, comm2) */
