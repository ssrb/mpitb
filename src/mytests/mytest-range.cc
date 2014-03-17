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
#include "mpitb.h"		// no builblocks, but for Makefile detection

DEFUN_DLD(NAME, args, nargout,
"test-range: ellucidating start address for ranges\n\
 Try for instance mytest(a) with:\n\
	a=1:3:10\n\
	a=1:2:10\n\
")
{
  int  nargin = args.length();
  if( (nargin<1)
	|| (! args(0).is_range())){
	printf("test: range arg required\n");
	return octave_value();		// error returns nothing
  }
  octave_value ov=args(0);
  Range  rv  = ov.range_value();
	       ov.print_info(std::cout);

  double d[] = {rv.base(), rv.limit(), rv.inc(), 0};
  int   *pi  = (int *) &d[3];  	*pi  =  rv.nelem();
  	 pi  = (int *)  d;
  printf("d=[\t%f %f\t\t%f %f]\n",		 d[ 0],  d[ 1],  d[ 2],  d[ 3]);
  printf("i=[\t%d %d %d %d\t%d %d %d %d]\n",	pi[ 0], pi[ 1], pi[ 2], pi[ 3],
						pi[ 4], pi[ 5], pi[ 6], pi[ 7]);
  double*pd= (double*) &rv;
         pi= (int   *)  pd;
  printf("-----------dump ov.range_value()\n");
  printf("d=[\t%f %f\t\t%f %f]\n",		pd[ 0], pd[ 1], pd[ 2], pd[ 3]);
  printf("i=[\t%d %d %d %d\t%d %d %d %d]\n",	pi[ 0], pi[ 1], pi[ 2], pi[ 3],
						pi[ 4], pi[ 5], pi[ 6], pi[ 7]);
  printf("-----------pointers to ov.rep\n");
  printf(" & ov.  get_rep(): %p\n", & ov.  get_rep());
  printf("ov.internal_rep(): %p\n",ov.internal_rep());
  printf("ov.mex_get_data(): %p\n",ov.mex_get_data());

	 pd= (double*) ov.internal_rep();
         pi= (int   *) pd;
  printf("-----------dump ov.internal_rep()\n");
  printf("d=[\t%f"
	  "\t%f %f\t\t%f %f]\n",	pd[ 0], pd[ 1], pd[ 2], pd[ 3],	pd[ 4]);
  printf("i=[\t%d %d"
	  "\t%d %d %d %d"
	  "\t%d %d %d %d]\n",	pi[ 0], pi[ 1], pi[ 2], pi[ 3],	pi[ 4], pi[ 5],
						pi[ 6], pi[ 7],	pi[ 8], pi[ 9]);

  return octave_value();
}

