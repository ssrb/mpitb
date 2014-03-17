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

args(0)       .print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",args(0)       .get_count());

	BLCK_ONESYM(NAME,0,attr)		/* attrnam, attrsym */
attrsym->def().print_info(std::cout);		/* count==2 */
printf("--------%d---------\n",attrsym->def().get_count());

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

	   octave_value* ovptr =
const_cast<octave_value*> ( & attrsym->def().get_rep() );
args(0)       .print_info(std::cout);		/* count==4 / 3*/
printf("-    -  %d   -    -\n",args(0).       get_count());
#if 0
attr          .print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",attr.          get_count());
#endif
attrsym->def().print_info(std::cout);		/* count==3 / 2 */
printf("--------%d---------\n",attrsym->def().get_count());

	octave_value ov (ovptr,attrsym->def().get_count()+1);
/*	octave_value ov = octave_value
			(ovptr,attrsym->def().get_count()+1); */
args(0).print_info(std::cout);			/* count==5 / 4 */
printf("-    -  %d   -    -\n",args(0).       get_count());
#if 0
attr          .print_info(std::cout);		/* count==4 */
printf("-    -  %d   -    -\n",attr.          get_count());
#endif
attrsym->def().print_info(std::cout);		/* count==4 / 3 */
printf("-    -  %d   -    -\n",attrsym->def().get_count());
ov            .print_info(std::cout);		/* count==4 / 3 */
printf("--------%d---------\n",ov.            get_count());

printf("###################\n");
printf("arg0 @ %p\trep %p\teval %p\n",	&args(0),		/* tmp */
					&args(0).get_rep(),
					&args(0).eval   ());	/* tmp */
#if 0
printf("attr @ %p\trep %p\teval %p\n",	&attr   ,
					&attr   .get_rep(), 0);
				/*	&attr   .eval   ()*/
#endif
printf("asym @ %p\trep %p\teval %p\n",	&attrsym->def(),
					&attrsym->def().get_rep(),
					&attrsym->def().eval   ());/*tmp*/
printf("ov   @ %p\trep %p\teval %p\n",	&ov     ,
					&ov     .get_rep(),
					&ov     .eval   ());	/* tmp */
printf("ovptr->\t\t\t -> %p\n",ovptr);

	return octave_value();
}

