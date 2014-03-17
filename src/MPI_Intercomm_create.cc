/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Intercomm_create
/*
 * ----------------------------------------------------
 * Creates an intercommuncator from two intracommunicators
 * [info comm] = MPI_Intercomm_create (l_comm, l_leader, r_comm, r_leader, tag)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hGrp.h"		// PATN_COM_F_2COMLDR_TAG

DEFUN_DLD(NAME, args, nargout,
"MPI_Intercomm_create	Creates an intercommunicator from two intracomms\n\
\n\
  [info comm] = MPI_Intercomm_create (l_comm, l_leader, p_comm, r_leader, tag)\n\
\n\
  l/p_comm  (int) Local/remote(peer) (intra)comms (handles)\n\
  l/r_leader(int) Ranks of leaders in local/peeer_comm (often 0)\n\
  tag       (int) Message tag to use in constructing intercommunicator\n\
                    if  multiple MPI_Intercomm_creates are being made,\n\
                    they should use different tags\n\
\n\
  comm (int)	returned comm (handle)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator (NULL?)\n\
      4 MPI_ERR_TAG	Invalid tag argument (must be nonnegative)\n\
     13 MPI_ERR_ARG	Invalid argument (NULL pointer?)\n\
     16 MPI_ERR_OTHER	LAM/MPI unable to acquire memory\n\
      6 MPI_ERR_RANK	Invalid source or destination rank (0..Commsize-1)\n\
\n\
  SEE ALSO: MPI_Intercomm_merge, MPI_Comm_free, MPI_Comm_remote_group,\n\
            MPI_Comm_remote_size, MPI_Comm_test_inter\n\
            comms\n\
\n\
  MPI_Intercomm_create is a collective operation on the newly created comm\n\
\n\
  local and remote comms must be disjoint\n\
  peer_comm allows communication between l_leader and r_leader\n\
\n\
  The MPI 1.1 Standard contains two mutually exclusive  comments  on  the\n\
  input intracommunicators.  One says that their repective groups must be\n\
  disjoint; the other that the leaders can be the  same  process.   After\n\
  some  discussion  by the MPI Forum, it has been decided that the groups\n\
  must be disjoint.  Note that the reason given for this in the  standard\n\
  is  not  the  reason  for  this choice; rather, the other operations on\n\
  intercommunicators (like MPI_Intercomm_merge ) do not make sense if the\n\
  groups are not disjoint.\n\
\n\
")

	PATN_COM_F_2COMLDR_TAG (NAME)

/*[info comm] = MPI_Intercomm_create (l_comm,l_leader, r_comm,r_leader, tag)*/
