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
 * Attribute Functions: MPI_Attr [_delete / _get]
 * ----------------------------------------------------
 * Other attr funcs in wrapper file:	MPI_Keyval [_create / _free]
 * 					MPI_Attr_put
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

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
 * MAX_ATTR_KVAL, depends on LAM/OMPI
 * ------------------------------------------------------------------- */

#if      LAM_MPI
 #define MAX_ATTR_KVAL	LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS
#elif    OPEN_MPI
 #define MAX_ATTR_KVAL	IMPI_HOST_COLOR
#else
 #error No max attribute keyval known (this is neither LAM/ nor Open-MPI)
#endif	/* LAM_MPI / OPEN_MPI */


/* -------------------------------------------------------------------
 * [info attr flag] = MPI_Attr_get (comm, kv)
 * ------------------------------------------------------------------- */

#define PATN_ATR_INT_F_COM_KEY(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)		/* comm   */			\
	BLCK_ONEINT(PNAME,1,keyval)	/* keyval */			\
									\
	int info, flag=0, *pred;	/* for LAM predefined attrs */	\
	octave_value ret, *attr;	/* for Octave user attribs  */	\
	Matrix empty;							\
	ret =  empty;			/* default return value */	\
									\
	if (keyval <= MAX_ATTR_KVAL){	/* Max predefined key value */	\
									\
		info = PNAME(comm, keyval, &pred, &flag);		\
	    if(!info && flag)        ret = *pred;			\
	}								\
    else{								\
		info = PNAME(comm, keyval, &attr, &flag);		\
	    if(!info && flag)        ret = *attr;			\
	}								\
									\
	RET_3_ARG (info, ret, flag)					\
}
	/* [info attr flag] = MPI_Attr_get (comm, kv) */



/* -------------------------------------------------------------------
 * info = MPI_Attr_delete (comm, kv)
 * ------------------------------------------------------------------- */

#define PATN_F_COM_KEY(PNAME)						\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)		/* comm   */			\
	BLCK_ONEINT(PNAME,1,keyval)	/* keyval */			\
									\
	int info =  PNAME(comm, keyval);/* side effect - */		\
					/* detaches kv from comm, */	\
	RET_1_ARG  (info)		/* actual destroy by... */	\
}					/* ...octave wrapper */
					/* which really mustn't destroy */
/*--------------------------------------------------------------------------
 * OCTAVE_DELETE_FN is rendered completely useless, cannot destroy attr
 * which is safely stored in wrapper table
 * Octave user can empty the copy passed to it (attr=[]) but few else
 * only the wrapper routine can do it
 * 
 * Funnily enough, the wrapper routine mustn't do it either
 * since although we don't want the attr here anymore,
 * it might have been "copied" (which means that a void* to it was passed)
 * to another comm from which we might want to go on accessing the attr
 *
 * We only destroy attributes (which are "statically copied" with the keyval)
 * when the keyval itself is freed (and we don't mind if there are
 * void*s to it hanging around, since MPI_Attr_get will fail due to the
 * invalid keyval freed)
 *--------------------------------------------------------------------------
 */

