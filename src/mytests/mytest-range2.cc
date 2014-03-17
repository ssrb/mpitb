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
#include "mpitb.h"		// MPITB_64BIT !!!

DEFUN_DLD(NAME, args, nargout,
"test-range: meaning of capacity(), nelem(), numel(), byte_size()\n\
  with ranges...\n\
 Try for instance mytest(a) with:\n\
	a=1:3:10\n\
	a=1:2:10\n\
")
{
	NARGCHK(NAME,1)
	printf("-------------\n");
	args(0).print_info(std::cout);
	printf("-------------\n");

	if ( ! args(0).is_range() )
	  error("not a range");

	printf("byte_size(): %d\n", args(0).byte_size   ());
	printf("capacity (): %d\n", args(0).capacity    ());
//	printf("nelem    (): %d\n", args(0).nelem       ());
	printf("numel    (): %d\n", args(0).numel       ());
//	printf("data     (): %p\n", args(0).data        ());
//	printf("fortran_v(): %p\n", args(0).fortran_vec ());
	printf("mex_get_d(): %p\n", args(0).mex_get_data());
	printf("-------------\n");

  return octave_value();
}
