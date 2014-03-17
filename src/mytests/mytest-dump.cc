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
"test-dump: dump object storage data to figure out\n\
  where are possibly stored the private ST scalar; members\n\
 Try for instance mytest(a) with:\n\
	b=2.3     , a=b\n\
	b=int32(7), a=b\n\
	b=7+5i    , a=b\n\
")
{
	NARGCHK(NAME,1)
	printf("-------------\n");
	args(0).print_info(std::cout);
//	args(0).make_unique();		// mk_uniq this copy? args(0)
	printf("get_count: %d\n",args(0).get_count());

	BLCK_ONESYM(NAME,0,var)			/* varnam, varsym */
	octave_value &varref = varsym->def();	// the variable definition
	printf("aftervref: %d\n",args(0).get_count());
	printf("vrefcount: %d\n",varref.get_count());
	printf("-------------\n");

  double*pd= (double*) &varref;
  printf("hexdump of:\n");
  int	*pi= (int   *) &varref;
  printf("varref \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
	 pi= (int   *) &args(0);
  printf("args(0)\t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("see equal rep pointers (1st void* place)\n");
	printf("-------------\n");

#if 0
	varref.make_unique();			// mk_uniq _it_, not arg(0)
	printf("after uniq:%d\n",varref.get_count());

	 pd= (double*) &varref;
  printf("hexdump of:\n");
	 pi= (int   *) &varref;
  printf("varref \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
	 pi= (int   *) &args(0);
  printf("args(0)\t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("see different pointers\n");
	printf("-------------\n");
#endif

	double			       d = varref.scalar_value();
	printf("doubleval: %f\n", d);

	const octave_base_value & getrep = varref.get_rep();
	      octave_base_value * intrep = varref.internal_rep();
	printf("vrefcount: %d\n",varref.get_count());

	 pd= (double*)  intrep;
  printf("hexdump of:\n");
	 pi= (int   *) &getrep;
  printf("getrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
	 pi= (int   *)  intrep;
  printf("intrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("see intr=getr addresses (@ addresses)\n");
  printf("see vrefcount (2nd int place) and (scalar place) after it\n");
  printf("dbldump it:\n");
  printf("intrep \t@ %p |\t%10.4g %10.4g %10.4g %10.4g\n",
						   pd,pd[0],pd[1],pd[2],pd[3]);
	printf("-------------\n");

  printf("setting it to (double) 3.14\n");
#if MPITB_64BIT
	 pd[2] = 3.14;
#else
	 pd[1] = 3.14;
#endif
         d = varref.scalar_value();
	printf("doubleval: %f\n", d);
	printf("vrefcount: %d\n",varref.get_count());

  printf("hexdump of:\n");
  printf("getrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("intrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("see changed double value\n");
  printf("dbldump it:\n");
  printf("intrep \t@ %p |\t%10.4g %10.4g %10.4g %10.4g\n",
						   pd,pd[0],pd[1],pd[2],pd[3]);
	printf("-------------\n");

  printf("setting it to (int32) 7\n");
#if MPITB_64BIT
	 pi[4] = 7; pi[5]=0;
#else
	 pi[2] = 7; pi[3]=0;
#endif
         d = varref.scalar_value();
	printf("doubleval: %f\n", d);
	printf("vrefcount: %d\n",varref.get_count());

  printf("hexdump of:\n");
  printf("getrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("intrep \t@ %p |\t%10x %10x %10x %10x\n", pi,pi[0],pi[1],pi[2],pi[3]);
  printf("see changed int32 value\n");
  printf("dbldump it:\n");
  printf("intrep \t@ %p |\t%10.4g %10.4g %10.4g %10.4g\n",
						   pd,pd[0],pd[1],pd[2],pd[3]);
	printf("-------------\n");

  return octave_value();
}

/* ----------------------------------------------------------------
 * See $OCTAVE/src/ov-base-scalar.h     - where ST scalar is defined
 * and its super-classes (base classes) ov.h / ov-base.h
 * where the other members/attributes are defined
 *
 * protected:                              from ov.h (2.9.10)
 *   octave_base_value *rep;            // The real representation.
 * private:                                an octave_value is just a pointer
 *   ...                                                ...to a base_value
 *
 * protected:                              from ov-base.h (2.9.10)
 *   ...                                   protected methods...
 *   int count;                         // A reference count.
 * private:                                an octave_base_value is just a count
 *   static int curr_print_indent_level;        ...of how many pointers to it
 *   static bool beginning_of_line;             ...and the count is inherited
 *  DECLARE_OV_BASE_TYPEID_FUNCTIONS_AND_DATA   ...on the specialized class
 * };
 *
 * template <class ST>                     from ov-base-scalar.h (2.9.10)
 * class octave_base_scalar :              an octave_scalar of type ST
 *  public octave_base_value               is a base_value (inherits count)
 * {                                       with additional attribute of type ST
 * public:
 *   ...
 * protected:                              here it is:
 *   ST scalar;                         // The value of this scalar.
 *   mutable MatrixType typ;
 * };
 *
 * class OCTAVE_API octave_complex :       from ov-complex.h
 *  public octave_base_scalar<Complex>     here ST==Complex
 *
 * class OCTINTERP_API octave_scalar :     from ov-scalar.h
 *  public octave_base_scalar<double>      here ST==double
 * ---------------------------------------------------------------- */
