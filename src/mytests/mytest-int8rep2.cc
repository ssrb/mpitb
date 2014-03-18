/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  mytest
/*
 * ----------------------------------------------------
 * mytest: do something
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc

DEFUN_DLD(NAME, args, nargout,
"test-int8rep: See if can cast i8 object to C char pointer\n\
 try \n\
	a=int8(3)\n\
	mytest(a)\n\
\n\
")
{
	NARGCHK(NAME,1)

	args(0).print_info(std::cout);
	if (!args(0).is_integer_type()) error("is not integer!");
  printf(" ov.i8_val().       value(): %d\n",  args(0).int8_scalar_value().
								   value() );
  printf("*ov.i8_val().mex_get_data(): %d\n",*(char*)
					      (args(0).int8_scalar_value().
							   mex_get_data()) );
  printf("*ov.mex_get_data         (): %d\n",*(char*)
					      (args(0).	   mex_get_data()) );
//printf("*ov.i8_val()               : %d\n",*(char*)
//					     &(args(0).int8_scalar_value()));

  return octave_value();
}

