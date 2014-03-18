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
 * mytest-uniq: explore octave-value scalar representation / lazycopy
 * ----------------------------------------------------
 */
#include "mpitb.h"		// no builblocks, but for Makefile detection

DEFUN_DLD(NAME, args, nargout,
"test-uniq: explore octave-value scalar representation / lazycopy")
{
  int  nargin = args.length();
  if( (nargin<1)
	|| (! args(0).is_real_scalar())){
	printf("test: scalar arg required\n");	// error() instead?
	return octave_value();		// error returns nothing
  }

  printf("sizeof(          int)==%d\n", sizeof(int));
  printf("sizeof(     long int)==%d\n", sizeof(long int));
  printf("sizeof(long long int)==%d\n", sizeof(long long int));
  printf("sizeof(       double)==%d\n", sizeof(double));

  	printf("-----------\n");

  args(0).print_info(std::cout);
  const octave_value &ov = args(0).get_rep();
  char  *pc;
  int   *pi;
  double*pd;

  	printf("-----------\n");

  double d = ov.double_value();	pc = (char*) &d;
  printf("double  %8f\t",d);	pi = (int *) &d;
  printf("hex %02x %02x %02x %02x %02x %02x %02x %02x\t",
	      pc[0],pc[1],pc[2],pc[3],pc[4],pc[5],pc[6],pc[7]);
  printf("int %8d %8d\n",
	      pi[0],pi[1]);
  
  int    m = ov.int_value();	pc = (char*) &m;
  printf("int     %8d\t",m);
  printf("hex %02x %02x %02x %02x %02x %02x %02x %02x\n",
	      pc[0],pc[1],pc[2],pc[3],pc[4],pc[5],pc[6],pc[7]);
  
  	printf("-----------\n");

    pi= (int   *) &ov;
    pd= (double*) &ov;
    printf("dump\t as ints \t @ %p (%p)\t as ints (hex)\n"
	"%d %2d %9d %9d\t %x %2x %9x %9x\n"
	"%d %2d %9d %9d\t %x %2x %9x %9x\n"
	"%d %2d %9d %9d\t %x %2x %9x %9x\n"
	"%d %2d %9d %9d\t %x %2x %9x %9x\n"
	"dump\t as doubles\n"
	"%f %f %f %f\t  %f %f %f %f\n",
	pi, pd,
	pi[ 0], pi[ 1], pi[ 2], pi[ 3],	pi[ 0], pi[ 1], pi[ 2], pi[ 3],
	pi[ 4], pi[ 5], pi[ 6], pi[ 7],	pi[ 4], pi[ 5], pi[ 6], pi[ 7],
	pi[ 8], pi[ 9], pi[10], pi[11],	pi[ 8], pi[ 9], pi[10], pi[11],
	pi[12], pi[13], pi[14], pi[15],	pi[12], pi[13], pi[14], pi[15],
	pd[ 0], pd[ 1], pd[ 2], pd[ 3],	pd[ 4], pd[ 5], pd[ 6], pd[ 7]);

  return octave_value();
}

