/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_group
/*
 * ----------------------------------------------------
 * Accesses the group associated with given communicator
 * [info grp] = MPI_Comm_group (comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_GRP_F_COM

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_group		Accesses the group associated with given communicator\n\
\n\
  [info grp] = MPI_Comm_group (comm)\n\
\n\
  comm (int)	communicator (handle). MPI_COMM_NULL is not valid\n\
  grp  (int)	group (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
     13 MPI_ERR_ARG     Invalid argument (typically  a  NULL pointer)\n\
\n\
  SEE ALSO: MPI_Group_union, MPI_Group_difference, MPI_Group_intersection\n\
            comms, groups\n\
\n\
  It is an error  to  use  MPI_COMM_NULL  as  one  of  the  arguments  to\n\
  MPI_Comm_compare .  The relevant sections of the MPI standard are\n\
  (2.4.1  Opaque Objects) and (5.3.2.  Group Constructors)\n\
\n\
")

	PATN_GRP_F_COM (NAME)

	/* [info grp] = MPI_Comm_group (comm) */
