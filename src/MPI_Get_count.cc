/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Get_count
/*
 * ----------------------------------------------------
 * MPITB: Get the size (in bytes) of a message
 * In MPI, this call: Gets the number of "top level" elements
 * [info count] = MPI_Get_count (stat)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hStat.h"		// is_MPI_Stat(), get_MPI_Stat()
 
DEFUN_DLD(NAME, args, nargout,
"MPI_Get_count          Returns the number of bytes in a message\n\
\n\
  [info count] = MPI_Get_count (stat)\n\
\n\
  stat   (struc) return status of receive operation (see MPI_Recv)\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (.oct file w/MPI_PACKED)\n\
\n\
  count(int) number of received bytes\n\
             see MPI_Get_elements to translate to #doubles or #chars...\n\
\n\
  In LAM/MPI, _Get_count    Gets the number of `top level' elements\n\
              _Get_elements Returns number of basic elements in a datatype\n\
\n\
  SEE ALSO: MPI_Get_elements\n\
\n\
")
{
	NARGCHK(NAME,1)
/*
 * [info count] = MPI_Get_count (stat)
 */
    if ((! args(0).is_map() )		// Too peculiar to apply usual macros
     || (! is_MPI_Stat (args(0)))){
	error("MPI_Get_count: required stat(struc)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
    }

    MPI_Status stat = get_MPI_Stat ( args(0) );
    int count, info = NAME (&stat, MPI_PACKED, &count);

	RET_2_ARG (info, count)
}
