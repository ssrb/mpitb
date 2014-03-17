/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Get_elements
/*
 * ----------------------------------------------------
 * Returns the number of basic elements in a datatype
 * [info elems] = MPI_Get_elements (stat, type)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hStat.h"		// is_MPI_Stat(), get_MPI_Stat()
#include "hType.h"		//		  get_MPI_Type()
 
DEFUN_DLD(NAME, args, nargout,
"MPI_Get_elements       Gets the number of `top level' elements\n\
\n\
  [info elems] = MPI_Get_elements (stat, type)\n\
\n\
  stat   (struc) return status of receive operation (see MPI_Recv)\n\
       src (int)       source rank for the accepted message\n\
       tag (int)       message tag for the accepted message\n\
       err (int)       return code\n\
       len (int)       message length\n\
\n\
  type   Octave value whose class is used to interpret elems\n\
               See MPI_Pack for a listing of compatible MPITB classes\n\
               Non-compatible classes count as bytes (MPI_PACKED)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (.oct file w/MPI_PACKED)\n\
\n\
  elems(int) number of received elements\n\
             see MPI_Get_count to translate to #bytes\n\
\n\
  In LAM/MPI, _Get_count    Gets the number of `top level' elements\n\
              _Get_elements Returns number of basic elements in a datatype\n\
\n\
  SEE ALSO: MPI_Get_count\n\
\n\
")
{
	NARGCHK(NAME,2)
/*
 * [info elems] = MPI_Get_elements (stat, type)
 */
    if ((! args(0).is_map() )		// Too peculiar to apply usual macros
     || (! is_MPI_Stat (args(0)))){
	error("MPI_Get_elements: required stat(struc)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
    }

    MPI_Status   stat = get_MPI_Stat(args(0));
    MPI_Datatype type = get_MPI_Type(args(1));
    int elems,   info = NAME(&stat, type, &elems);

	RET_2_ARG (info, elems)
}
