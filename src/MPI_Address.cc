/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Address
/*
 * ----------------------------------------------------
 * Returns the memory address of the argument Octave var
 * Gets the address of a location in memory
 * [info addr] = MPI_Address (var)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK etc
#include "hBuff.h"		// type enums - declare vars - aux get_MPI_Buff

DEFUN_DLD(NAME, args, nargout,
"MPI_Address            Gets the address of an Octave variable's data in memory\n\
\n\
  [info addr] = MPI_Address (var)\n\
\n\
  var          Octave variable whose address is returned\n\
\n\
  addr (long)  memory address\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
\n\
  SEE ALSO: MPI_Get_address (not implemented)\n\
            misc\n\
\n\
  This function is deprecated. It has been replaced with an MPI-2 function\n\
\n\
")
{
	NARGCHK    (NAME,1)
/*
 * [info addr] = MPI_Address (var)
 */
	BLCK_ONESYM(NAME,0,var)			// varnam, varsym

	DECLARE_BUFF_VARS			// BUF, SIZ, TYPE
	BUF = get_MPI_Buff (varsym, SIZ,true,	// true: SIZ in bytes
				   TYPE,false);	// false: don't make_unique
	warning_with_id("MPITB:Address-verbose",
		"MPITB:get_MPI_Buff(%s) == %ld (%p)",
		varsym->name().c_str(),
		reinterpret_cast<long>(BUF),BUF);

	MPI_Aint               addr ;		// typedef long MPI_Aint;
	int info = NAME (BUF ,&addr);

	RET_2_ARG (info,       addr)
}
