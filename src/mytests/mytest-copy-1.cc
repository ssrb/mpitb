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
/*
#include "mpitb.h"		// buildblocks NARGCHK, etc
 */
#include "oct.h"

#define BLCK_ONESYM(NAME,ARGN,SYMNAME)	/* NAME of the var ARG Nth */	\
	const std::string SYMNAME##nam = args.name_tags()(ARGN);	\
	symbol_record    *SYMNAME##sym = curr_sym_tab->lookup(SYMNAME##nam);\
									\
	if ( ! SYMNAME##sym){						\
	  error(#NAME ": required arg#%d: " #SYMNAME "(var)", ARGN+1);	\
	  return octave_value();	/* error returns nothing */	\
	}

DEFUN_DLD(mytest, args, nargout,
"test-copies-1: insight on how many copies are done of an octave_value\n\
 even using constructors in the code, not only due to args(0) copy\n\
 check with:\n\
 z=0		% 1st copy when variable defined->stored in symtable\n\
 a=0		% 2nd copy when included in a function argument list\n\
 		% 3rd copy accessing(creating) args(0) inside function\n\
 mytest(a,\"z\")	% args(0):3 copies, a->def():2, z->def():1\n\
 b=0		% no lazy copy optimizations\n\
 mytest(b,\"a\")	% args(0):3, b->def():2, a->def():1\n\
 b=a		% just normal, unoptimized lazy copy\n\
 mytest(b,\"a\")	% args(0):4, b->def():3, a->def():3\n\
		% for a/b 2 copies each symbol, and b is in arglist\n\
 mytest(b,a)	% args(0):5, b->def():4, args(1):5, a->def():4\n\
		% for a/b copy each other, and _both_ a&b in arglist\n\
 c=5, mytest(c,\"a\")	% args(0):4, c->def():3, a->def():2\n\
			% for c is (1) args(0), (2) arglist, (3) defined, ...\n\
			% ... and (4) alive due to the assignment sentence\n\
			% or: (1)def (2)alive (3)arglist [c'def] (4)args(0)\n\
 d=c=4, mytest(d,\"b\")	% args(0):5, d->def():4, b->def():2\n\
			% insert (5) lazycopied c->d after reason (1) above\n\
 d=c=b, mytest(d,\"a\")	% args(0):6, d->def():5, a->def():5\n\
			% it's (4) lazycopies abcd & (5)alive assignment\n\
			% which explains a&d->defs, and (6)\n\
BRAINDAMAGE args(0)\n\
\n\
")
{
/* --------------------------------------------------------------------------
 * First arg: normally 3 copies: symbol table, args, args(0) (creates 1 more)
 *		args(0).get_count() == 3 (since args(0) not in LHS,
 *					  it's octave_value operator() (...))
 * 	attrsym->def() .get_count() == 2 (it's octave_value & def(),
 *					  def() doesn't construct any new copy,
 *					  2nd copy linked in args)
 * --------------------------------------------------------------------------
 */
	if (args.length()<1){
	  error("mytest: 1 arg required");
	  return octave_value();
	}

printf("========== args(0)\n");
	/* count == 3 copies */		    args(0).print_info(std::cout);
printf("---------- %d copies f/ args(0)\n", args(0).get_count());

	BLCK_ONESYM(mytest,0,attr)		/* attrnam, attrsym */
printf("========== sym->def() (symbol record for %s)\n",	attrnam.c_str());
	/* count == 2 copies */		    attrsym->def().print_info(std::cout);
printf("---------- %d copies f/ %s's sym\n",attrsym->def().get_count(),
								attrnam.c_str());
#if 1
			octave_value & ov = attrsym->def();
printf("==========after octave_value & ov = sym->def())\n");
	/* count == 2 copies */				ov.print_info(std::cout);
printf("---------- %d copies f/ %s's new reference\n",	ov.get_count(),
								attrnam.c_str());
#endif


	if (args.length()>1){
	  printf("========== 2nd argument provided... ");
	  if (args(1).is_string()){
	    printf("string (varname)\n");
/* --------------------------------------------------------------------------
 * Second arg: if string, then it's a variable name (perhaps not in args)
 *	bttrsym->def(). get_count() == 1 (no lazy copies around!!!)
 * --------------------------------------------------------------------------
 */
     symbol_record*bttrsym=curr_sym_tab->lookup(args(1).string_value());
	    if (bttrsym){

printf("========== variable %s defined\n",	args(1).string_value().c_str());
	/* count == 1 copies */		    bttrsym->def().print_info(std::cout);
printf("---------- %d copies f/ %s's sym\n", bttrsym->def().get_count(),
						args(1).string_value().c_str());

	    } else {
printf("========== variable %s undefined\n",	args(1).string_value().c_str());
	    }//(bttrsym)

	  } else {//(args(1).is_string())
/* --------------------------------------------------------------------------
 * Second arg: if not string, then it's another arg to whom check #copies
 *	 	args(1).get_count() == 3 (or more if twice the same)
 * 	bttrsym->def() .get_count() == 2 (or three times the same...)
 * --------------------------------------------------------------------------
 */
	    printf("not string (treated as another octave_value)\n");

printf("========== args(1)\n");
	/* count == 3 copies */		    args(1).print_info(std::cout);
printf("---------- %d copies f/ args(1)\n", args(1).get_count());

	BLCK_ONESYM(mytest,1,bttr)		/* bttrnam, bttrsym */
printf("========== sym->def() (symbol record for %s)\n",	bttrnam.c_str());
	/* count == 2 copies */		    bttrsym->def().print_info(std::cout);
printf("---------- %d copies f/ %s's sym\n",bttrsym->def().get_count(),
								bttrnam.c_str());
	  }//(args(1).is_string())

	}//(args.length()>1)

/* --------------------------------------------------------------------------
 * Other ideas:
 *	- additional args may selectively increase count for 1st or 2nd arg
 *	- additional _references_ should leave the count unaffected
 *	- additional _copies_ should increase count
 *	    whenever an octave_base_value is linked from a new octave_value
 * --------------------------------------------------------------------------
 */

#if 0
	const octave_value&attr  =		/* attr   */
		    args(0);			/* whichever type it is */
args(0)       .print_info(std::cout);		/* count==4 */
printf("-    -  %d   -    -\n",args(0).       get_count());
attr          .print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",attr.          get_count());
attrsym->def().print_info(std::cout);		/* count==3 */
printf("--------%d---------\n",attrsym->def().get_count());
#endif

#if 0
	   octave_value* ovptr =
const_cast<octave_value*> ( & attrsym->def().get_rep() );
args(0)       .print_info(std::cout);		/* count==4 / 3*/
printf("-    -  %d   -    -\n",args(0).       get_count());
#endif

#if 0
attr          .print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",attr.          get_count());
#endif

#if 0
attrsym->def().print_info(std::cout);		/* count==3 / 2 */
printf("--------%d---------\n",attrsym->def().get_count());

	octave_value ov (ovptr,attrsym->def().get_count()+1);
/*	octave_value ov = octave_value
			(ovptr,attrsym->def().get_count()+1); */
args(0).print_info(std::cout);			/* count==5 / 4 */
printf("-    -  %d   -    -\n",args(0).       get_count());
#endif

#if 0
attr          .print_info(std::cout);		/* count==4 */
printf("-    -  %d   -    -\n",attr.          get_count());
#endif

#if 0
attrsym->def().print_info(std::cout);		/* count==4 / 3 */
printf("-    -  %d   -    -\n",attrsym->def().get_count());
ov            .print_info(std::cout);		/* count==4 / 3 */
printf("--------%d---------\n",ov.            get_count());

printf("###################\n");
printf("arg0 @ %p\trep %p\teval %p\n",	&args(0),		/* tmp */
					&args(0).get_rep(),
					&args(0).eval   ());	/* tmp */
#endif

#if 0
printf("attr @ %p\trep %p\teval %p\n",	&attr   ,
					&attr   .get_rep(), 0);
				/*	&attr   .eval   ()*/
#endif

#if 0
printf("asym @ %p\trep %p\teval %p\n",	&attrsym->def(),
					&attrsym->def().get_rep(),
					&attrsym->def().eval   ());/*tmp*/
printf("ov   @ %p\trep %p\teval %p\n",	&ov     ,
					&ov     .get_rep(),
					&ov     .eval   ());	/* tmp */
printf("ovptr->\t\t\t -> %p\n",ovptr);
#endif

	return octave_value();
}

