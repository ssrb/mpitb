/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Lookup_name
/*
 * ----------------------------------------------------
 * Lookup a previously published name
 * [info port] = MPI_Lookup_name (svc, inf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, ot.h
#include "hInfo.h"		// PATN_STR_F_STR_INF

DEFUN_DLD(NAME, args, nargout,
"MPI_Lookup_name	Lookup a previously published name\n\
\n\
  [info port] = MPI_Lookup_name (svc, inf)\n\
\n\
  svc  (str)	published service name for the desired port\n\
  inf  (int)	implementation specific information (MPI_Info handle)\n\
\n\
  port (str)	server port name for the desired service (use MPI_Comm_connect)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument\n\
     25 MPI_ERR_SERVICE problem with service name (already/not yet published)\n\
\n\
  SEE ALSO: MPI_Publish_name, MPI_Unpublish_name,\n\
            process\n\
\n\
  Currently, LAM/MPI ignores any info keys that are passed to this function.\n\
  As such, info keys specific to a different implementation can safely be\n\
  passed to this function. Additionally, the MPI constant MPI_INFO_NULL can\n\
  also be used.\n\
\n\
  The scope of the published name is the LAM universe -- MPI processes\n\
  running on any node included from the initial lamboot are capable of\n\
  retrieving a name published by MPI_Publish_name. Service names stay in\n\
  existence until they are either manually removed with MPI_Unpublish_name\n\
  or cleaned with the lamclean command. It is a common mistake to forget\n\
  to MPI_Unpublish_name at the end of a run, and therefore leave published\n\
  names in the LAM runtime environment.\n\
\n\
")

	PATN_STR_F_STR_INF (NAME)

	/* [info port] = MPI_Lookup_name (svc, inf) */
