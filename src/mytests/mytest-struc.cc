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
"test: skeleton to test Octave API")
{
	MPI_Status stat={1,2,3,4};

	printf("src %d\n",stat.MPI_SOURCE);
	printf("tag %d\n",stat.MPI_TAG   );
	printf("err %d\n",stat.MPI_ERROR );
	printf("len %d\n",stat.st_length );

	return octave_value();
}
