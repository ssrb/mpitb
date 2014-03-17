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
#include "mpitb.h"		// builblocks  NARGCHK, etc

DEFUN_DLD(NAME, args, nargout,
"test-size: exercises length(), byte_size(), numel(), capacity()\n\
  calls to query varsize in Octave API\n\
 Try for instance mytest(a) with the following values:\n\
  a= 2.3 , a=[2 3],		length() returns max dim,\n\
  a= 2+3i, a=[2 i],		numel(),\n\
  a='2.3', a=\"2.3\",		capacity(), both return total num elems\n\
  c=[2 3], b=[c;c;c],		byte_size is numel()*elementsize in bytes\n\
  a=cat(3,b,b,b,b),		matrix.nnz() can be called and is again numel\n\
")
{
	NARGCHK(NAME,1)

	args(0).print_info(std::cout);		/* count==3 */

	printf("get_count() == %d\n",args(0).get_count());
	printf("-----------------\n" 			);
	printf("rows     () == %d\n",args(0).rows     ());
	printf("columns  () == %d\n",args(0).columns  ());
	printf("length   () == %d\n",args(0).length   ());
	printf("ndims    () == %d\n",args(0).ndims    ());
	printf("-----------------\n" 			);
	printf("numel    () == %d\n",args(0).numel    ());
	printf("capacity () == %d\n",args(0).capacity ());
	printf("byte_size() == %d\n",args(0).byte_size());
	printf("-----------------\n" 			);
	printf("nnz      () == %d\n",args(0).nnz      ());
//	printf("nzmax    () == %d\n",args(0).nzmax    ());
//	printf("nfields  () == %d\n",args(0).nfields  ());
	printf("-----------------\n" 			);

	return octave_value();
}

