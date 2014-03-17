/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
/*
 * ----------------------------------------------------
 * Error-Handling Functions: MPI_Errhandler_ get/set
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * [info eh] = MPI_Errhandler_get (comm)
 *  info     = MPI_Errhandler_set (comm, eh)
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * BLCK_ONEHND:
 * Take ErrorHandler ARG Nth into source code (name ehnd always)
 * -------------------------------------------------------------------------
 * crossref: used from here
 * MPI_Errhandler_free (1 uses)
 * -------------------------------------------------------------------------
 * hErr.h  (1 uses):	PATN_F_COM_ERRH
 * -------------------------------------------------------------------------
 */
#define BLCK_ONEHND(PNAME,ARGN)		/* ARG Nth in arglist  */	\
/*	if ( ! args(ARGN).is_real_scalar()){	*/			\
	if ( ! MPITB_isOPtrTyp(args(ARGN))){				\
	  error(STRFY(PNAME) ": required arg#%d: ehnd(ptr)", ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Errhandler ehnd = MPITB_ptrcast ( MPI_Errhandler, args(ARGN) );


/* -------------------------------------------------------------------------
 * -------- Call Patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */
 
/* -------------------------------------------------------------------
 * we make even individual patterns for just 1 MPI routine
 * it's easier to read them all together - MPITB easier to maintain
 *
 * all patterns carry the PNAME macro: code more reutilizable,
 * and allows for stringification at the lowest level (error messages)
 * ------------------------------------------------------------------- */
                                                                                
/* -------------------------------------------------------------------
 * [info eh] = MPI_Errhandler_get (comm)
 * ------------------------------------------------------------------- */

#define PATN_ERRH_F_COM(PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONECOM(PNAME,0)	/* comm */				\
									\
	MPI_Errhandler		 ehnd=0;				\
	int info =  PNAME(comm, &ehnd);					\
									\
	RET_2_ARG  (info, MPITB_intcast(				\
				 ehnd	))				\
}


/* -------------------------------------------------------------------
 * info = MPI_Errhandler_set (comm, eh)
 * ------------------------------------------------------------------- */

#define PATN_F_COM_ERRH(PNAME)						\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)	/* comm */				\
	BLCK_ONEHND(PNAME,1)	/* ehnd */				\
									\
	int info =  PNAME (comm, ehnd);					\
	RET_1_ARG  (info)						\
}

