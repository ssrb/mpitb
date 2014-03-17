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
"test: explore octave-value scalar representation / lazycopy")
{
  int  nargin = args.length();
  if (nargin<1){
	printf("test: 1 arg required\n");
	return octave_value();		// error returns nothing
  }

//args(0).make_unique();		// would make_unique this copy args(0)

  const std::string varnam = args.name_tags()(0);
  symbol_record    *varsym = curr_sym_tab->lookup(varnam);
  if (!varsym){
	printf("test: variable arg required\n");
	return octave_value();		// error returns nothing
  }
  printf("----- args(0) ---------------------\n");
  args(0).print_info(std::cout);
  printf("----- var->def() ------------------\n");
  varsym->def().print_info(std::cout);
  printf("----- var->def().get_rep() --------\n");
  varsym->def().get_rep().print_info(std::cout);
  printf("---------- def() ---------- uniq --\n");

		    varsym->def().make_unique();// the variable definition

  printf("----- args(0) ---------------------\n");
  args(0).print_info(std::cout);
  printf("----- var->def() ------------------\n");
  varsym->def().print_info(std::cout);
  printf("----- var->def().get_rep() --------\n");
  varsym->def().get_rep().print_info(std::cout);
  printf("###################################\n");

  if (varsym->def().is_matrix_type()){

//	( (Matrix&) varsym->def().get_rep() )   .make_unique();	// protected
//		    varsym->def().matrix_value().make_unique();	// protected
//								// & unsensible
	Matrix m = varsym->def().matrix_value();
  printf("----- m ---------------------------\n");
  m.print_info(std::cout,"");
							// force make_unique
//varsym->def().matrix_value().elem(0,0)=0;
//printf("m    (0,0)==%f\n",                                m(0,0));
//printf("m_val(0,0)==%f\n",varsym->def().matrix_value().elem(0,0));
  							    m(0,0)=m(0,0);
//printf("m    (0,0)==%f\n",                                m(0,0));
  printf("----- m ---------------------------\n");
  m.print_info(std::cout,"");

//double &d =	varsym->def().matrix_value().elem(0,0);
//		varsym->def().matrix_value()(0,0)=0;
//	varsym->define( * varsym->def().matrix_value().clone());
//	m(0,0)=0;
  varsym->define( octave_value(m) );

  printf("----- var->def().matrix_value()----\n");
  varsym->def().matrix_value().print_info(std::cout,"");
  printf("----- var->def().get_rep().matv()--\n");
  varsym->def().get_rep().matrix_value().print_info(std::cout,"");
  printf("###################################\n");

  printf("----- args(0) ---------------------\n");
  args(0).print_info(std::cout);
  printf("----- var->def() ------------------\n");
  varsym->def().print_info(std::cout);
  printf("----- var->def().get_rep() --------\n");
  varsym->def().get_rep().print_info(std::cout);
  printf("-----------------------------------\n");
  }

const octave_value &varref =			// not args(0).get_rep()
		    varsym->def().get_rep();	// but the var.get_rep()

  if (varref.is_scalar_type()){
    printf("double %f\n", varref.scalar_value());
    double*pd= (double*) &varref;
    printf("double %f @ %p\n",     pd[1], pd);
    printf("setting it to 3.14\n"); pd[1] = 3.14;
    printf("double %f\n", varref.scalar_value());
  }

  void **pv= (void**) &varref;
  int   *pi= (int  *) &varref;
  double*pd= (double*)&varref;
  printf("dump\t as ints \t @ %p \t  or as pointers\n"
	"%d %2d %9d %9d\t  %p %p %p %p\n"
	"%d %2d %9d %9d\t  %p %p %p %p\n"
	"%d %2d %9d %9d\t  %p %p %p %p\n"
	"%d %2d %9d %9d\t  %p %p %p %p\n"
	"dump\t as doubles\n"
	"%f %f %f %f\t  %f %f %f %f\n",
	pv,
	pi[ 0], pi[ 1], pi[ 2], pi[ 3],   pv[ 0], pv[ 1], pv[ 2], pv[ 3],
	pi[ 4], pi[ 5], pi[ 6], pi[ 7],   pv[ 4], pv[ 5], pv[ 6], pv[ 7],
	pi[ 8], pi[ 9], pi[10], pi[11],   pv[ 8], pv[ 9], pv[10], pv[11],
	pi[12], pi[13], pi[14], pi[15],   pv[12], pv[13], pv[14], pv[15],
	pd[ 0], pd[ 1], pd[ 2], pd[ 3],   pd[ 4], pd[ 5], pd[ 6], pd[ 7]);

  return octave_value();
}

