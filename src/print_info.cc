/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define NAME print_info
/*
 * ----------------------------------------------------
 * exercises octave_value's.print_info() method
 * usage:	print_info ( whatever )
 * ----------------------------------------------------
 */
#include <oct.h>

 DEFUN_DLD ( print_info, args, nargout,
"print_info: exercises the member so called\n\
\n\
")
{
  int nargin = args.length();
  if (nargin<1){
	printf("print_info: 1 arg required\n");
	return octave_value();
  }

  const std::string varnam = args.name_tags()(0);
  symbol_table::symbol_record varsym = symbol_table::find_symbol(varnam);

  if (! varsym.is_defined()){
	error("print_info: required var, not expression");
	return octave_value();
  }

  varsym.varval().print_info(std::cout);

  return octave_value();
}
