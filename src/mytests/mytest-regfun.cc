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

struct regfs{
octave_value st_attr;		/* static original attr copy */
};
#define  NULOV octave_value()	/* NULL octave_value */
struct regfs regfun[]={		/* static? */
	{NULOV}
};


DEFUN_DLD(NAME, args, nargout,
"test: skeleton to test Octave API")
{
	NARGCHK    (NAME,1)

	regfun[0].st_attr = args(0);

	printf("sizeof(void*)   \t%d\n",   sizeof(void*)               );
	printf("sizeof(int)     \t%d\n",   sizeof(int)                 );
	printf("\n");
	printf("sizeof(double)  \t%d\n",   sizeof(double)              );
	printf("sizeof(oct_v)   \t%d\n",   sizeof(octave_value)        );
	printf("\n");
	printf("sizeof(regfs)   \t%d\n",   sizeof(regfs)               );
	printf("sizeof(regfun)  \t%d\n",   sizeof(regfun)              );
	printf("\n");
	printf("       regfun   \t%p\n",   regfun                      );
	printf("&      regfun[0]\t%p\n", & regfun[0]                   );
	printf("& attr          \t%p\n", & regfun[0].st_attr           );
	printf("& attr.get_rep()\t%p\n", & regfun[0].st_attr.get_rep() );
	printf("\n");

	double*pd= (double*) &regfun[0].st_attr;
	int   *pi= (int   *) &regfun[0].st_attr;
	void **pp= (void **) &regfun[0].st_attr;
	printf("doubles @ %p\t%e\n"   ,  pd, pd[0]);
	printf("ints    @ %p\t%d %d\n",  pi, pi[0], pi[1]);
	printf("void*'s @ %p\t%p %p\n",  pp, pp[0], pp[1]);

	return octave_value();
}
