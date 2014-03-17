/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_get_parent
/*
 * ----------------------------------------------------
 * Get  the  parent communicator of a spawned MPI rank
 * it's an (inter)communicator in which remote ranks are the parents
 * [info parents] = MPI_Comm_get_parent
 * ----------------------------------------------------
 */
#include "mpitb.h"		// PATN_COM_F_VOID

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_get_parent    Get the parent (inter)communicator for spawned MPI rank\n\
\n\
  [info parents] = MPI_Comm_get_parent\n\
\n\
  parents (int) returned parent communicator\n\
                (remote group->parents) (local group->children)\n\
\n\
  info    (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
\n\
  SEE ALSO: MPI_Comm_spawn, MPI_Comm_spawn_multiple\n\
            process\n\
\n\
")

	PATN_COM_F_VOID (NAME)

	/* [info parents] = MPI_Comm_get_parent */
