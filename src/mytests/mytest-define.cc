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
 * test-define: can a boolean macro argument select expanded code?!?
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
/* ----------------------------	wrong
#define MACRO(boolarg)	\
#if boolarg				\
printf("Code true  branch selected\n");	\
#else					\
printf("Code false branch selected\n");	\
#endif
   ----------------------------	*/
#define MACRO(boolarg)	\
if (boolarg)	printf("Code true  branch selected\n");	\
else		printf("Code false branch selected\n");
		// trust on compiler optimizations
		// for boolarg is known at compile time

DEFUN_DLD(NAME, args, nargout,
"test-define: can a boolean macro argument select expanded code?!?\n\
simply try: mytest\n\
\n\
")
{
	MACRO(1)
	MACRO(0)
	return octave_value();
}

