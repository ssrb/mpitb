/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Init
/*
 * ----------------------------------------------------
 * Initialize the MPI execution environment
 * info = MPI_Init [ ( 'arg' [, 'arg']... ) ]
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h

DEFUN_DLD(NAME, args, nargout,
"MPI_Init               Initialize the MPI execution environment\n\
\n\
  info = MPI_Init [ ( 'arg' [, 'arg']... ) ]\n\
\n\
  arg (str) arguments to pass\n\
  info(int) return code\n\
      0 MPI_SUCCESS    No error\n\
     16 MPI_ERR_OTHER  Attempt was made to call MPI_INIT a  second  time\n\
                       MPI_INIT may only be called once in a program\n\
                       Clear all MPITB MEX files before trying it\n\
\n\
  SEE ALSO: MPI_Finalize, MPI_Initialized, MPI_Finalized\n\
            misc\n\
\n\
")
{
    int nargin = args.length();			// Too flexible to use NARGCHK
/*
 * info = MPI_Init [ ( 'arg' [, 'arg']... ) ]
 */
    for (int i=0; i<nargin; i++){
	if( ! args(i).is_string() ) {
	    error("MPI_Init: args must be strings");
	    return octave_value (MPI_ERR_ARG);	// error returns nothing
	}
    }

    string_vector argvec = args.make_argv("MPI_Init");
    char **argve= argvec.c_str_vec();
    char **argv =&argve[1];

//  printf("args: "); for (int i=0; i<nargin; i++) printf("%s ",argv[i]);
//  printf("\n");

//  int info = NAME    (...);
    int info = MPI_Init(&nargin, &argv);
    
    free(argve);
    return octave_value (info);
//	RET_1_ARG       (info)
}
