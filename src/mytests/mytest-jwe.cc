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
"test: skeleton to test Octave API\n"
"copied from http://velveeta.che.wisc.edu/octave/lists/help-octave/2005/514\n\
 Illustrates the use of print_usage(), error_state and error()\n\
 as well as reverse-order return list. Call with 2 int arguments\n")
{
  octave_value_list retval;

  if (args.length () != 2)
    print_usage();
  else
    {
      int r = args(0).int_value();
      int c = args(1).int_value();

      if (error_state){
//	  error ("test_jwe: expecting integer arguments");	// uses stderr
	  printf("test_jwe: expecting integer arguments\n");	// uses stdout
	  return octave_value(9);
	}
      else
	{
	  // XXX FIXME XXX -- check that r and c are both non-negative?

	  retval(2) = r*c;
	  retval(1) = c;
	  retval(0) = r;
	}
    }

  return retval;
}

