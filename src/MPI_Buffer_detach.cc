/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Buffer_detach
/*
 * ----------------------------------------------------
 * Removes an existing buffer (for use in MPI_Bsend, etc)
 * info = MPI_Buffer_detach
 * ----------------------------------------------------
 */
#include "mpitb.h"		// builblocks NARGCHK, etc
#include "hBuff.h"		// type enums - declare vars - aux get_MPI_Buff

DEFUN_DLD(NAME, args, nargout,
"MPI_Buffer_detach      Removes an existing buffer (for use in MPI_Bsend)\n\
\n\
  info = MPI_Buffer_detach\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
\n\
  SEE ALSO: MPI_Buffer_attach, MPI_Bsend\n\
\n\
")
{
	NARGCHK   (NAME,0)
/*
 * info = MPI_Buffer_detach
 */
	DECLARE_BUFF_VARS

	int info = NAME (&BUF, &SIZ);	// this info is wasted, spoiled
		   TYPE=MPI_PACKED;	// to avoid warning: unused

	RET_1_ARG (info)
}
/* BUG: should return bufptr/size and
 * 	there should exist some MPE_Buffer_reattach(bufptr, size)
 * 	that could use those returnvals to leave the buffer as it was
 * Currently MPI_Buffer_attach must have as only arg an Octave var
 * Read MPI man page for details
 */

