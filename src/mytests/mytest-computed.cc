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
"test-compassgn: understanding computed assignment (check?true:false)\n\
  original code example:\n\
	bool qdirt = false;\n\
	if ((n==0) && (IDX==SIZ)) {SIZ++; qdirt=true;}\n\
  replaced by:\n\
	bool qdirt=( (n==0) && (IDX==SIZ) ? SIZ++, true : false);\n\
  Try with\n\
	mytest(n,IDX,SIZ)\n\
	mytest(0,3,3) -> true\n\
")
{
	NARGCHK    (NAME,3)

	printf("---------------\n");
	BLCK_ONEINT(NAME,0,n)
	BLCK_ONEINT(NAME,1,IDX)
	BLCK_ONEINT(NAME,2,SIZ)
	bool qdirt = false;
	if ((n==0) && (IDX==SIZ)) {SIZ++; qdirt=true;}
	printf("original result: qdirt==%d, SIZ==%d\n", qdirt,SIZ);
	printf("---------------\n");
	BLCK_ONEINT(NAME,0,n_)
	BLCK_ONEINT(NAME,1,IDX_)
	BLCK_ONEINT(NAME,2,SIZ_)
	bool qdirt_ = ( (n_==0) && (IDX_==SIZ_) ? SIZ_++, true : false);
	if ((n==0) && (IDX==SIZ)) {SIZ++; qdirt=true;}
	printf("computed result: qdirt==%d, SIZ==%d\n", qdirt_,SIZ_);
	printf("---------------\n");
	printf("SIZ-=qdirt == %d\n", SIZ_-=qdirt_);

  return octave_value();
}
