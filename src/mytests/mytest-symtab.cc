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
 * mytest-symtab: explore why 3 copies in script variables
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc

DEFUN_DLD(mytest, args, nargout,
"test-symtab: insight on how many copies are done of an octave_value\n\
 when interactive or inside a script as opposed to inside a function.\n\
 Check with a script like this:\n\
 % function jo\n\
 % ju\n\
 a=0\n\
 mytest(a)\n\
\n\
 function ju\n\
 b=0\n\
 mytest(b)\n\
\n\
")
{
/* --------------------------------------------------------------------------
 * Interactive/script arg: 2 copies: symbol table, args (args(0) 1 more)
 * Function arg: normally 3 copies. Look in symbol table for coincidences
 * --------------------------------------------------------------------------
 */
	NARGCHK(NAME,1)

	BLCK_ONESYM(NAME,0,attr)		/* attrnam, attrsym */
printf("========== sym->def() %s'symrec\n",attrnam.c_str());
	/* count == 2 copies */		   attrsym->def().print_info(std::cout);
printf("---------- %d copies f/%s's sym\n",attrsym->def().get_count(),
					   attrnam.c_str());

/* --------------------------------------------------------------------------
 * 
 * --------------------------------------------------------------------------
 */
printf("---------- curr_sym_tab details:\n");
printf("\t    size: %d\n",	curr_sym_tab->size());
printf("---------- done\n");
				curr_sym_tab->print_info(std::cout);

printf("---------- fbi_sym_tab details:\n");
printf("\t    size: %d\n",	fbi_sym_tab->size());
printf("---------- done\n");
				fbi_sym_tab->print_info(std::cout);

printf("---------- global_sym_tab details:\n");
printf("\t    size: %d\n",	global_sym_tab->size());
printf("---------- done\n");
				global_sym_tab->print_info(std::cout);

printf("---------- top_level_sym_tab details:\n");
printf("\t    size: %d\n",	top_level_sym_tab->size());
printf("---------- done\n");
				top_level_sym_tab->print_info(std::cout);
/* --------------------------------------------------------------------------
 * Other ideas:
 * --------------------------------------------------------------------------
 */
	return octave_value();
}

