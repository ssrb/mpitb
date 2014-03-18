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
"test: skeleton to test Octave API")
{
	NARGCHK(NAME,1)

	args(0).print_info(std::cout);
//	args(0).get_rep().make_unique();	// mk_uniq this copy? args(0)
	BLCK_ONESYM(NAME,0,var)			/* varnam, varsym */
	BLCK_ONEINT(NAME,0,val)			/* varnam, varsym */
  printf("int value: %d\n",val);

	varsym->def().make_unique();		// the variable definition
	const octave_value &varref =		// not args(0).get_rep()
	varsym->def().get_rep();		// but the var.get_rep()

  double d = varref.scalar_value();
  printf("double %f\n", d);

  double*pd= (double*) &varref;
  int   *pi= (int   *) &varref;
  printf("double %f @ %p\n",  pd[1], pd);
  printf("int %d %d @ %p\n",  pi[2], pi[3], pi);

  printf("dump\t%d %d %d %d\n\t%d %d %d %d\n\t%d %d %d %d\n\t%d %d %d %d\n",
	  pi[ 0], pi[ 1], pi[ 2], pi[ 3], pi[ 4], pi[ 5], pi[ 6], pi[ 7],
	  pi[ 8], pi[ 9], pi[10], pi[11], pi[12], pi[13], pi[14], pi[15]);

  printf("setting it to 3.14\n");
	 pd[1] = 3.14;
  printf("double %f @ %p\n", pd[1], pd);
  printf("int %d %d @ %p\n", pi[2], pi[3], pi);


  printf("dump\t%d %d %d %d\n\t%d %d %d %d\n\t%d %d %d %d\n\t%d %d %d %d\n",
	  pi[ 0], pi[ 1], pi[ 2], pi[ 3], pi[ 4], pi[ 5], pi[ 6], pi[ 7],
	  pi[ 8], pi[ 9], pi[10], pi[11], pi[12], pi[13], pi[14], pi[15]);

  return octave_value();
}

