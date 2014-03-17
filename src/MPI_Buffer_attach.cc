/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Buffer_attach
/*
 * ----------------------------------------------------
 * Attaches a user‐defined buffer for sending (for use in MPI_Bsend, etc)
 * info = MPI_Buffer_attach (buf)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hBuff.h"		// type enums - declare vars - aux get_MPI_Buff	

DEFUN_DLD(NAME, args, nargout,
"MPI_Buffer_attach      Attaches a user-defined buffer for sending\n\
\n\
  info = MPI_Buffer_attach (buf)\n\
\n\
  buf (var)    user-provided Octave variable to be used as message buffer\n\
       The size given should be the sum of the sizes of all outstanding Bsends\n\
       that you intend to have, plus a few bytes for each  Bsend that you do.\n\
       Compute size using MPI_Pack_size(1st)+...+(nth)+ n*MPI_BSEND_OVERHEAD\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer\n\
     17 MPI_ERR_INTERN This is fatal. Please send bug report to LAM\n\
\n\
  SEE ALSO: MPI_Buffer_detach, MPI_Bsend\n\
\n\
")
{
	NARGCHK    (NAME,1)
/*
 * info = MPI_Buffer_attach (buf)
 */
	BLCK_ONESYM(NAME,0,buf)			// bufnam, bufsym

	DECLARE_BUFF_VARS			// BUF, SIZ, TYPE
	BUF = get_MPI_Buff(bufsym, SIZ,true,	// true: SIZ in bytes
				  TYPE,true);	// true: MK_UNIQ (buff overwr)
    if(!BUF) return octave_value(MPI_ERR_ARG);	// errormsg from get_MPI_Buff()

	int info =  NAME (BUF, SIZ);

	RET_1_ARG  (info)
}
