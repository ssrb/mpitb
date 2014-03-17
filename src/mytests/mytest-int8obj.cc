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

  octave_value ov = args(0);

  printf("type_id:        \t%d\n",ov.type_id());
  printf("type_name:      \t%s\n",ov.type_name().c_str());
  printf("class_name:     \t%s\n",ov.class_name().c_str());
  printf("is_real_type:   \t%d\n",ov.is_real_type());
  printf("is_scalar_type: \t%d\n",ov.is_scalar_type());
  printf("is_real_scalar: \t%d\n",ov.is_real_scalar());
  printf("is_numeric_type:\t%d\n",ov.is_numeric_type());

  octave_int8 i8 = ov.int8_scalar_value();
  double       d = i8;
  printf("i8\t=\t%f\n",d);

  const unsigned char *i8ptr = i8.iptr();
        octave_int8_t  oi8   = i8.value();

  printf("\n");
  printf("octave_int8 object @ %p\n",&i8);
  printf("octave_int8 ival   @ %p\n", i8ptr);
  printf("octave_int8  value   %d\n",oi8);

  return octave_value(i8);
}
