/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_accept
/*
 * ----------------------------------------------------
 * Accept a connection from another MPI process (using MPI_Comm_connect)
 * [info clcomm] = MPI_Comm_accept (port, inf, root, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hInfo.h"		// PATN_COM_F_PIRC

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_accept	Accept a connection from another MPI process\n\
\n\
  [info clcomm] = MPI_Comm_accept (port, inf, root, comm)\n\
\n\
  port   (str)	port name (returned from MPI_Open_port) (only used at root)\n\
  inf    (int)	extra information (MPI_Info handle)     (only used at root)\n\
  root   (int)	rank in comm of root node accepting connection\n\
  comm   (int)	intracommunicator over which the call is collective\n\
\n\
  clcomm (int)	intercommunicator with client as remote group\n\
\n\
  info   (int)	return code \n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator\n\
      8 MPI_ERR_ROOT	Invalid root (0..Comm_size-1)\n\
     13 MPI_ERR_ARG	Invalid argument\n\
     17 MPI_ERR_INTERN	This is fatal - send a bug report to LAM\n\
     16 MPI_ERR_OTHER	Other error - use MPI_Error_string for more info\n\
     24 MPI_ERR_FLAGS	Mismatch of run-time flags. For example, lamd/c2c mode\n\
\n\
  SEE ALSO: MPI_Open_port, MPI_Close_port, MPI_Comm_connect,\n\
            MPI_Comm_disconnect,\n\
            process\n\
\n\
  The MPI_Comm_accept and MPI_Comm_connect calls simulate the\n\
  accept() / connect() semantics of standard socket programming --\n\
  a client `connects' to a server.\n\
\n\
  LAM/MPI allows for previously unrelated MPI programs to join into a MPI\n\
  program with these calls. Both MPI programs must be running by the same\n\
  user in the same LAM universe (i.e., in the scope of a single lamboot).\n\
\n\
")

	PATN_COM_F_PIRC (NAME)		// PIRC -> Port, Inf, Root, Comm

	/* [info clcomm] = MPI_Comm_accept (port, inf, root, comm) */
