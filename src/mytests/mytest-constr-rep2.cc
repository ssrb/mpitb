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
#include "mpitb.h"		// builblocks  NARGCHK, etc

DEFUN_DLD(NAME, args, nargout,
"test: skeleton to test Octave API")
{
	NARGCHK(NAME,1)

args(0)       .print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",args(0)       .get_count());

	BLCK_ONESYM(NAME,0,attr)		/* attrnam, attrsym */
attrsym->def().print_info(std::cout);		/* count==2 */
printf("--------%d---------\n",attrsym->def().get_count());


	   octave_value* ovptr =( & attrsym->def() );
/*const_cast<octave_value*> ( & attrsym->def() );*/
args(0)       .print_info(std::cout);		/* count==3*/
printf("-    -  %d   -    -\n",args(0)       .get_count());
attrsym->def().print_info(std::cout);		/* count==2 */
printf("-    -  %d   -    -\n",attrsym->def().get_count());
ovptr  ->      print_info(std::cout);		/* count==2 */
printf("--------%d---------\n",ovptr  ->      get_count());

	   void* voptr = static_cast<void*>(ovptr);
args(0)       .print_info(std::cout);		/* count==3*/
printf("-    -  %d   -    -\n",args(0)       .get_count());
attrsym->def().print_info(std::cout);		/* count==2 */
printf("-    -  %d   -    -\n",attrsym->def().get_count());
ovptr  ->      print_info(std::cout);		/* count==2 */
printf("-    -  %d   -    -\n",ovptr  ->      get_count());
static_cast<octave_value*>
(voptr)->      print_info(std::cout);		/* count==2 */
printf("--------%d---------\n",static_cast<octave_value*>
			      (voptr) ->      get_count());

	octave_value ov = * static_cast<octave_value*>(voptr);
args(0)       .print_info(std::cout);		/* count==4 */
printf("-    -  %d   -    -\n",args(0)       .get_count());
attrsym->def().print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",attrsym->def().get_count());
ovptr  ->      print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",ovptr  ->      get_count());
static_cast<octave_value*>
(voptr)->      print_info(std::cout);		/* count==3 */
printf("-    -  %d   -    -\n",static_cast<octave_value*>
			      (voptr) ->      get_count());
ov            .print_info(std::cout);		/* count==3 */
printf("--------%d---------\n",ov.            get_count());

printf("###################\n");
printf("arg0 @ %p\trep %p\teval %p\n",	&args(0),		/* tmp */
					&args(0).get_rep(),
					&args(0).eval   ());	/* tmp */
printf("asym @ %p\trep %p\teval %p\n",	&attrsym->def(),
					&attrsym->def().get_rep(),
					&attrsym->def().eval   ());/*tmp*/
printf("ovptr->%p\n",ovptr);
printf("voptr->%p\n",voptr);
printf("ov   @ %p\trep %p\teval %p\n",	&ov     ,
					&ov     .get_rep(),
					&ov     .eval   ());	/* tmp */

	return octave_value();
}

