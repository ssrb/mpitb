/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME	MPI_DUP_FN
/*
 * ----------------------------------------------------
 * MPITB constant for corresponding MPI_*_function (callback function) object
 * var = MPI_DUP_FN
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hConst.h"		// GEN_PTR/RET_PTR system

#define   NAME	MPI_DUP_FN
GEN_1_PTR(NAME)			// This is not a #define'd value, but
#undef          MPI_DUP_FN	// just because of that, #undef does not harm
DEFUN_DLD(NAME, args, nargout,
"MPI_DUP_FN		MPITB constant for corresponding callback function\n\
\n\
  Current value (LAM-7.1.3, OpenMPI-1.2.3 basically identical) is:\n\
\n\
 typedef int (MPI_Copy_function) (MPI_Comm, int, void*,void*,void*, int*);\n\
 int         MPI_DUP_FN        (comm, key, extra, attrin, attrout, flag)\n\
 {\n\
        *flag = 1;\n\
        *((void **) attrout) = attrin;\n\
\n\
        return(MPI_SUCCESS);\n\
 }\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free,\n\
            caching, handles\n\
\n\
")
{
	NARGCHK (NAME,0)	// no problem, not defined symbol but function
//	var = MPI_NULL_COPY_FN
//	RET_1_ARG(MPITB_intcast( NAME ))	// weird...that's @MPI_DUP_FN ?
	RET_1_PTR
}
