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
 * Global MPITB include file
 * ----------------------------------------------------
 */

/*
 * Every MPITB command (Oct-file) source file needs <oct.h> & <mpi.h>
 */

#include <oct.h>	/* Octave */
//nclude <mpi.h>	/* MPI_whatever */

  #define MPIPP_H		dirty&works, cut include preprocessing
//				saves around 60K (79K->12K) in final .oct size
//#define LAM_WANT_MPI2CPP 0	redef warning, and we are not sure if it's LAM
//#define LAM_WANT_CPPFILE 0	redef?
#include <mpi.h>	/* MPI_whatever */

#if LAM_MPI		/* File size for MPITB object files with LAM */
#if LAM_WANT_MPI2CPP	/* increases from 10K to 70K when LAM C++ bindings */
// Now we know we are using LAM and perhaps C++ bindings
// how could we now undo the inlined C++ methods? From this C source code?
// tried -fno-(default)-inline			increases 23(17)K
// tried -fno-implicit-(inline)-templates	decreases 1K (70->69K)
// tried -fvisibility(-inlines-)(=)hidden	no effect
// tried --param inline-unit-growth=1/10/100	bad or no effect
#endif
#endif


// Saves typing: copied from Octave sources src/ov.h
// See liboctave/oct-types.h for octave_idx_type and [u]intXX_t typedefs
// we can #define intDEX octave_idx_type, but what about MPI_INTDEX?
#define             intDEX   octave_idx_type
#if    SIZEOF_OCTAVE_IDX_TYPE == SIZEOF_LONG
//efine             intDEX              long
#define         MPI_INTDEX          MPI_LONG
#elif  SIZEOF_OCTAVE_IDX_TYPE == SIZEOF_INT
//efine             intDEX              int
#define         MPI_INTDEX          MPI_INT
#else
#error "no octave_value extractor for octave_idx_type"
#endif


/* -------------------------------------------------------------------------
 * -------- IA32 / IA64 glue logic -----------------------------------------
 * ------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------
 * MPITB was originally designed for IA32, and casted MPI pointers to C ints
 * (both 4B wide) which in turn translated to Octave flints
 * 
 * On IA64 void* takes 8B and MPITB segfaulted. C long ints are also 8B wide,
 * so the logic choice seemed using 8B longs instead of 4B ints,
 * but that also segfaulted, due to implicit cast to flints (dblints).
 * (Since 1+12bits devoted to sign+mantissa, last LSB address bits got lost).
 *
 * Solution: explicit uint64 Octave object representation (when _ia64_ ) of
 * MPI_<whatever> types (which are all pointers to C structs) to avoid implicit
 * flint conversion when returning an integral value to Octave (see RET_1_ARG)
 *
 * A cleaner solution should be pursued, such as asking sizeof(MPI_ptrtype)
 * at compile time (even for any MPI non-LAM implementation?) and matching it
 * with sizeof(int/long) and sizeof(octave_uint32/64)
 * -------------------------------------------- Octave-2.9.10::config.h ----
   775	#undef IDX_TYPE_LONG
   825	#define SIZEOF_INT 4
   828	#define SIZEOF_LONG 4
	#define SIZEOF_LONG_LONG 8
	#define SIZEOF_SHORT 2
   837	#undef SIZEOF_VOID_P
   864	#undef USE_64_BIT_IDX_T
  1041	#ifdef USE_64_BIT_IDX_T
	  #define SIZEOF_OCTAVE_IDX_TYPE 8
	 #else
	  #define SIZEOF_OCTAVE_IDX_TYPE SIZEOF_INT
	 #endif
 * -------------------------------------------- Octave-2.9.10::oct-types.h -
typedef int octave_idx_type;
 * -------------------------------------------------------------------------
 *
 * Worse problems can be foreseen in the MPI_*pack* family of routines,
 * perhaps all compatibility Octave types <-> LAM/MPI types must be redesigned
 * (and then it will also affect the *reduce* family and operators... argh!)
 * or recomputed auto-magically at compile time... and then generate doc/help
 * ------------------------------------------------------------------------- */

				// Octave object able to store MPI C-pointer
				// and ov.method() returning it
#if __ia64__ || __x86_64__				// IA-64 / EM64T/AMD64
#define MPITB_OctPtrTyp		octave_uint64
#define MPITB_OctIntFcn		uint64_scalar_value
#define MPITB_64BIT		1
#define MPITB_ST_offset		2

#else	// #elif __i386__				// IA-32
#define MPITB_OctPtrTyp		octave_uint32
#define MPITB_OctIntFcn		uint32_scalar_value
#define MPITB_64BIT		0
#define MPITB_ST_offset		1
#endif
				// Minimal check that ov could be a pointer
#define MPITB_isOPtrTyp(ov)	((ov.is_scalar_type())&&(ov.is_numeric_type()))
				// construct octave_value from MPI C-Pointer
#define MPITB_intcast(cptr)			MPITB_OctPtrTyp		 (\
				reinterpret_cast <unsigned long> ( cptr ) )
				// translate back ov object to MPI C-Pointer
#define MPITB_ptrcast(typ,ov)	reinterpret_cast <typ> ( ov.		  \
						MPITB_OctIntFcn().value() )


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * RET_1_ARG: Return 1 octave_value
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Buffer_attach.cc:			MPI_Buffer_detach.cc:
 * MPI_COMM_NULL.cc:	MPI_COMM_SELF.cc:	MPI_COMM_WORLD.cc:
 * MPI_DUP_FN.cc:	MPI_Errhandler_create/free.cc:
 * MPI_ERRHANDLER_NULL	MPI_ERRORS_ARE_FATAL	MPI_ERRORS_RETURN.cc:
 * MPI_GROUP_EMPTY.cc:	MPI_GROUP_NULL.cc:	MPI_INFO_NULL.cc:
 * MPI_Init.cc: (commented out)		MPI_Keyval_create/free/Attr_put.cc:
 * MPI_MAX.cc:		MPI_MIN.cc:		MPI_OP_NULL.cc:
 * MPI_PROD.cc:		MPI_SUM.cc:		MPI_Pack.cc:
 * MPI_NULL_COPY_FN.cc:	MPI_NULL_DELETE_FN.cc:	MPI_REQUEST_NULL.cc:		
 * -------------------------------------------------------------------------
 * hAttr.h:   ( 1 uses)
 * hColl.h:   (11 uses)	hErr.h:    ( 1 uses)	hGrp.h:    ( 2 uses)
 * hInfo.h:   ( 5 uses)	hSend.h:   ( 1 uses)	hTstWait.h:( 2 uses)
 * mpitb.h:   ( 4 uses)
 * -------------------------------------------------------------------------
 */

#define RET_1_ARG(ARG)							\
	return	octave_value (ARG);	// use whichever constructor that fits


/* -------------------------------------------------------------------------
 * RET_2_ARG: Return 2 octave_values in an octave_value_list
 * we let automatic typecasting work so that the same macro serves
 * for returning ints, doubles, matrices, strings, whatever
 * reverse arg order for performance, to avoid retval resizing (link JWE mail)
 * http://velveeta.che.wisc.edu/octave/lists/help-octave/2005/514
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Address.cc:	MPI_Errhandler_create/free.cc:
 * MPI_Get_count.cc:	MPI_Keyval_create/free/Attr_put.cc:
 * MPI_Get_elements.cc:	MPI_Pack_size.cc:	MPI_Unpack.cc:
 * -------------------------------------------------------------------------
 * hErr.h:    (1 uses)	hGrp.h:   (12 uses)	hInfo.h:  ( 7 uses)
 * hRecv.h:   (3 uses)	hSend.h:  ( 1 uses)	hTopo.h:  (12 uses)
 * hTstWait.h:(3 uses)	mpitb.h:  ( 7 uses)
 * -------------------------------------------------------------------------
 */

#define RET_2_ARG(ARG0,ARG1)						\
	octave_value_list retval;					\
		retval(1) = ARG1;					\
		retval(0) = ARG0;					\
	return	retval;


/* -------------------------------------------------------------------------
 * RET_3_ARG: Return 3 octave_values in an octave_value_list
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Comm_spawn.cc: (commented out)
 * -------------------------------------------------------------------------
 * hAttr.h:   (1 uses)	hInfo.h:   (2 uses)	hTopo.h:   (4 uses)
 * hTstWait.h:(6 uses)	mpitb.h:   (1 uses)
 * -------------------------------------------------------------------------
 */

#define RET_3_ARG(ARG0,ARG1,ARG2)					\
	octave_value_list retval;					\
		retval(2) = ARG2;					\
		retval(1) = ARG1;					\
		retval(0) = ARG0;					\
	return	retval;


/* -------------------------------------------------------------------------
 * RET_4_ARG: Return 4 octave_values in an octave_value_list
 * -------------------------------------------------------------------------
 * crossref: used from
 * 
 * -------------------------------------------------------------------------
 * hTopo.h: (2 uses)	hTstWait.h:  (1 use)
 * -------------------------------------------------------------------------
 */

#define RET_4_ARG(ARG0,ARG1,ARG2,ARG3)					\
	octave_value_list retval;					\
		retval(3) = ARG3;					\
		retval(2) = ARG2;					\
		retval(1) = ARG1;					\
		retval(0) = ARG0;					\
	return	retval;


/* -------------------------------------------------------------------------
 * some blocks below require argument prescan even though they use
 * stringification (PNAME macro-expansion in this case) , like this
 *
 * #define ANYBLOCK(PNAME,BLAH)\
 *         ...
 *         printf(#PNAME ": error text\n")
 *
 * See cpp manual Section 3.10.6 (point 2) for more info on this common pitfall
 * here PNAME is the program name defined in the <PNAME>.cc file,
 * stringification inhibits arg-prescan, it would expand to "PNAME: error text"
 * instead of "<real-MPI-function-name>: error text"
 *
 * using a func-like-macro to stringify, cleans the 1st macro from strngfctions
 * so its argument list is correctly pre-scanned
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRNGFY: Stringify argument for error messages and such,
 *	forcing macro-argument prescan whih would be avoided
 *	if argument was directly stringified in macro body
 * -------------------------------------------------------------------------
 * crossref: used from
 * -------------------------------------------------------------------------
 * 
 * -------------------------------------------------------------------------
 */

#define	STRFY(ARGNAM)  #ARGNAM

#define	STRNGFY(ARGNAME)  \
	STRFY  (ARGNAME)	// 2-level, as opposed to 1-level above


/* -------------------------------------------------------------------------
 * NARGCHK: Check number of args
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Address.cc:	MPI_Buffer_attach.cc:		MPI_Buffer_detach.cc:
 * MPI_COMM_NULL.cc:	MPI_COMM_SELF.cc:		MPI_COMM_WORLD.cc:
 * MPI_Comm_spawn.cc:	MPI_Comm_spawn_multiple.cc:	MPI_DUP_FN.cc:
 * MPI_ERRHANDLER_NULL.cc:	MPI_ERRORS_ARE_FATAL.cc:MPI_ERRORS_RETURN.cc:
 * MPI_Errhandler_create/free.cc:	MPI_Keyval_create/free/Attr_put.cc:
 * MPI_GROUP_EMPTY.cc:	MPI_GROUP_NULL.cc:		MPI_INFO_NULL.cc:
 * MPI_Get_count.cc:	MPI_Get_elements.cc:		MPI_SUM.cc:
 * MPI_Init.cc:		MPI_MAX.cc:	MPI_MIN.cc:	MPI_OP_NULL.cc:
 * MPI_NULL_COPY_FN.cc:	MPI_NULL_DELETE_FN.cc:		MPI_PROD.cc:
 * MPI_Pack.cc:		MPI_Pack_size.cc:		MPI_REQUEST_NULL.cc:
 * -------------------------------------------------------------------------
 * hAttr.h: ( 2 uses)	hColl.h: (11 uses)	hErr.h:  ( 2 uses)
 * hGrp.h:  (14 uses)	hInfo.h: (14 uses)	hRecv.h: ( 1 uses)
 * hSndRecv.h:(1uses)	hTopo.h: (14 uses)	hTstWait.h:(2uses)
 * mpitb.h: (12 uses)
 * -------------------------------------------------------------------------
 */

#define NARGCHK(PNAME,NARGIN)		/* program NAME, # of ARGS */	\
	if (args.length()!= NARGIN){					\
	  error(STRFY(PNAME)	": %d args required", NARGIN);		\
	  RET_1_ARG  (-1);		/* error returns nothing */	\
	}

//	  RET_1_ARG  (MPI_ERR_ARG);	/* error returns nothing */
//		this couldn't be used with MPI_ERR_ARG.cc
//		which needed to     #undef MPI_ERR_ARG

/* -------------------------------------------------------------------------
 * BLCK_ONEINT: Take one int argument ARG Nth into source code (chosen name)
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Pack.cc:		MPI_Unpack.cc:		MPI_Keyval_create/free/put.cc:
 * -------------------------------------------------------------------------
 * hAttr.h:   (2 uses)	hColl.h:   (1 uses)	hGrp.h:    (3 uses)	
 * hInfo.h:   (2 uses)	hRecv.h:   (4 uses)	hSndRecv.h:(2 uses)
 * hTopo.h:   (9 uses)	hTstWait.h:(2 uses)	mpitb.h:   (4 uses)
 * -------------------------------------------------------------------------
 */

#define BLCK_ONEINT(PNAME,ARGN,INTNAME)	/* NAME of the int var */	\
	if ( ! args(ARGN).is_real_scalar()){	/* weak int test */	\
	error(STRFY(PNAME)	": required arg#%d: "			\
	      STRFY(INTNAME)	"(int)",	ARGN+1);		\
	  RET_1_ARG(MPI_ERR_ARG);	/* error returns nothing */	\
	}								\
	int INTNAME = args(ARGN).int_value();


/* -------------------------------------------------------------------------
 * BLCK_ONESTR: Take one string argument ARG Nth into source code (chosen name)
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Keyval_create/free/Attr_put.cc:	(2 uses)
 * MPI_Errhandler_create/free.cc:	(1 uses)
 * -------------------------------------------------------------------------
 * hInfo.h: (10 uses)
 * -------------------------------------------------------------------------
 */

#define BLCK_ONESTR(PNAME,ARGN,STRNAME)	/* ARG Nth in arglist */	\
        if ( ! args(ARGN).is_string()){					\
	error(STRFY(PNAME)	": required arg#%d: "			\
	      STRFY(STRNAME)	"(str)",	ARGN+1);		\
	  RET_1_ARG(MPI_ERR_ARG);	/* error returns nothing */	\
	}								\
	std::string STRNAME = args(ARGN).string_value();


/* -------------------------------------------------------------------------
 * BLCK_ONECOM: Take comm argument ARG Nth into source code (name always "comm")
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Keyval_create/free/Attr_put.cc:	(1 uses)
 * MPI_Pack.cc:		MPI_Pack_size.cc:	MPI_Unpack.cc:
 * -------------------------------------------------------------------------
 * hAttr.h:   (2 uses)	hColl.h:   (6 uses)	hErr.h:    (2 uses)
 * hGrp.h:    (5 uses)	hInfo.h:   (1 uses)	hRecv.h:   (1 uses)
 * hSndRecv.h:(1 uses)	hTopo.h:   (13 uses)	hTstWait.h:(1 uses)
 * mpitb.h:   (4 uses)
 * -------------------------------------------------------------------------
 */

#define BLCK_ONECOM(PNAME,ARGN)		/* ARG Nth in arglist  */	\
	if ( ! MPITB_isOPtrTyp(args(ARGN)) ){				\
	error(STRFY(PNAME)	": required arg#%d: comm(ptr)", ARGN+1);\
	  RET_1_ARG(MPI_ERR_ARG);	/* error returns nothing */	\
	}								\
	MPI_Comm comm = MPITB_ptrcast ( MPI_Comm, args(ARGN) );


/* -------------------------------------------------------------------------
 * BLCK_NULSYM: Try to get symbol of a given argument ARG Nth (chosen name)
 * if it's not an Octave variable, lookup will produce NULL pointer
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Keyval_create/free/Attr_put.cc:	(1 uses)
 * MPI_Pack.cc:		MPI_Unpack.cc:
 * -------------------------------------------------------------------------
 * hColl.h:  (1 uses)	hRecv.h:   (1 uses)
 * -------------------------------------------------------------------------
 */

#define BLCK_NULSYM(PNAME,ARGN,SYMNAME)	/* NAME of the var ARG Nth */	\
  const std::string SYMNAME##nam = args.name_tags()(ARGN);		\
  symbol_record    *SYMNAME##sym = curr_sym_tab->lookup(SYMNAME##nam);
// BUG?: operator [] const ? (name_tags() string_vector::[])
// PNAME not used here but used in similar _ONESYM, so kept for symmetry


/* -------------------------------------------------------------------------
 * BLCK_ONESYM: Take one var argument ARG Nth into source code (chosen name)
 * var arguments must be Octave variables (lookup produces non-NULL symbol),
 * so that they can be changed using varsym->define(new_value)
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Address.cc:	MPI_Buffer_attach.cc:	
 * MPI_Errhandler_create/free.cc:	MPI_Keyval_create/free/Attr_put.cc:
 * MPI_Pack.cc: (2uses)	MPI_Pack_size.cc:	MPI_Unpack.cc:
 * -------------------------------------------------------------------------
 * hGrp.h:     (2 uses)	hInfo.h:    (1 uses)	hRecv.h: (2 uses)
 * hSndRecv.h: (1 uses)	hTstWait.h: (1 uses)
 * -------------------------------------------------------------------------
 */

#define BLCK_ONESYM(PNAME,ARGN,SYMNAME)	/* NAME of the var ARG Nth */	\
  const std::string SYMNAME##nam = args.name_tags()(ARGN);		\
  symbol_record    *SYMNAME##sym = curr_sym_tab->lookup(SYMNAME##nam);	\
									\
	if ( ! SYMNAME##sym){						\
	error(STRFY(PNAME)	": required arg#%d: "			\
	      STRFY(SYMNAME)	"(var)",	ARGN+1);		\
	  RET_1_ARG(MPI_ERR_ARG);	/* error returns nothing */	\
	}
// BUG?: operator [] const ? (name_tags() string_vector::[])


/* -------------------------------------------------------------------------
 * -------- Call patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 * we make even individual patterns for just 1 MPI routine
 * it's easier to read them all together - MPITB easier to maintain
 *
 * all patterns carry the PNAME macro: code more reutilizable,
 * and allows for stringification at the BLCK_* level (error messages)
 * ------------------------------------------------------------------- */

			/* ------------- *
			 * PATN_*_F_VOID *
			 * ------------- */
/* -------------------------------------------------------------------
 * info = MPI_Finalize			int MPI_Finalize(void)
 * ------------------------------------------------------------------- */
#define PATN_F_VOID(PNAME)		/* programNAME */		\
{									\
	NARGCHK    (PNAME,0)						\
	int  info = PNAME ();						\
	RET_1_ARG  (info)	/* info -> octave scalar (flint) */	\
}


/* -------------------------------------------------------------------
 * [info flag] = MPI_Initialized	int MPI_Initialized(int *flag)
 * [info flag] = MPI_Finalized		int MPI_Finalized  (int *flag)
 * ------------------------------------------------------------------- */
#define PATN_INT_F_VOID(PNAME)		/* programNAME */		\
{									\
	NARGCHK        (PNAME,0)					\
	int int0=0,info=PNAME(&int0);					\
	RET_2_ARG (info,       int0)					\
}


/* -------------------------------------------------------------------
 * [info major minor] = MPI_Get_version
 *			int MPI_Get_version(int *version, int *subversion)
 * ------------------------------------------------------------------- */
#define PATN_2INT_F_VOID(PNAME)		/* programNAME */		\
{									\
	NARGCHK    (PNAME,0)						\
	int int0=0, int1=0,						\
	    info=   PNAME(&int0,&int1);					\
	RET_3_ARG  (info,  int0, int1)					\
}


/* -------------------------------------------------------------------
 * res  = MPI_Wtick			double MPI_Wtick(void)
 * secs = MPI_Wtime			double MPI_Wtime(void)
 * ------------------------------------------------------------------- */
#define PATN_DBL_F_VOID(PNAME)		/* programNAME */		\
{									\
	NARGCHK        (PNAME,0)					\
	double    dbl = PNAME( );					\
	RET_1_ARG(dbl)							\
}


/* -------------------------------------------------------------------
 * [info name] = MPI_Get_processor_name
 *			int MPI_Get_processor_name(char *name, int *len)
 * ------------------------------------------------------------------- */
#define PATN_STR_F_VOID(PNAME)		/* programNAME */		\
{					/* BUG: PROC_NAME? */		\
	NARGCHK        (PNAME,0)	/* not "general" STR_F_VOID */	\
									\
	char name[MPI_MAX_PROCESSOR_NAME];	name[0] = 0;		\
	int len=0,info=PNAME (name, &len);				\
									\
	RET_2_ARG(info,       name)	/* name -> octave_string */	\
}


/* -------------------------------------------------------------------
 * [info parents] = MPI_Comm_get_parent
 *				int MPI_Comm_get_parent(MPI_Comm *parent)
 * ------------------------------------------------------------------- */
#define PATN_COM_F_VOID(PNAME)		/* programNAME */		\
{					/* -> hGrp.h? */		\
	NARGCHK        (PNAME,0)					\
	MPI_Comm               comm=0;					\
	int        info=PNAME( &comm );					\
	RET_2_ARG (info,						\
		MPITB_intcast( comm ))					\
}


			/* ------------ *
			 * PATN_*_F_INT *
			 * ------------ */
/* -------------------------------------------------------------------
 * [info class] = MPI_Error_class (errcode)
 *				int MPI_Error_class(int errcode, int *class)
 * ------------------------------------------------------------------- */
#define PATN_INT_F_INT( PNAME,  INTNAME)/* programNAME, int-arg-NAME */	\
{									\
	NARGCHK        (PNAME,1)					\
	BLCK_ONEINT    (PNAME,0,INTNAME)	/* errcode */		\
	int int0=0,info=PNAME  (INTNAME, &int0);			\
	RET_2_ARG (info,                  int0)				\
}


/* -------------------------------------------------------------------
 * [info msg] = MPI_Error_string  (errcode)
 *		int MPI_Error_string(int errcode, char *msg, int *plen)
 * ------------------------------------------------------------------- */
#define PATN_STR_F_INT( PNAME)		/* programNAME */		\
{					/* BUG: errcode? ERR_STR? */	\
	NARGCHK        (PNAME,1)	/* not "general" STR_F_INT */	\
	BLCK_ONEINT    (PNAME,0,errcode)	/* errcode */		\
									\
	char str[MPI_MAX_ERROR_STRING];	str[0] = 0;			\
	int len=0,info= PNAME  (errcode, str, &len);			\
									\
	RET_2_ARG(info,                 str) /* str -> octave_string */	\
}


			/* -------------- *
			 * PATN_*_F_COM_* *
			 * -------------- */
/* -------------------------------------------------------------------
 * info = MPI_Barrier (comm)
 *					int MPI_Barrier(MPI_Comm comm)
 * 		does not change comm (unlike MPI_Comm_free/disconnect)
 * 		see  PATN_F_COMREF @ hGrp
 * ------------------------------------------------------------------- */
#define PATN_F_COM( PNAME)		/* programNAME */		\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONECOM(PNAME,0)		 /* comm  */			\
	int info =  PNAME		   (comm);			\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * [info flag]   = MPI_Comm_test_inter   (comm)
 * [info rank]   = MPI_Comm_rank         (comm)
 * [info size]   = MPI_Comm_size         (comm)
 * [info size]   = MPI_Comm_remote_size  (comm)
 *			int MPI_Comm_test_inter	(MPI_Comm comm, int *flag)
 *			int MPI_Comm_rank	(MPI_Comm comm, int *rank)
 *			int MPI_Comm_size	(MPI_Comm comm, int *psize)
 *			int MPI_Comm_remote_size(MPI_Comm comm, int *psize)
 * -------------------------------------------------------------------
 * [info topo ]  = MPI_Topo_test         (comm)
 * [info ndims]  = MPI_Cartdim_get       (comm)
 *			int MPI_Topo_test  (MPI_Comm comm, int *ptopo)
 *			int MPI_Cartdim_get(MPI_Comm comm, int *pndims)
 * ------------------------------------------------------------------- */
#define PATN_INT_F_COM(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONECOM   (PNAME,0)		/* comm */			\
    int int0=0,   info=PNAME (comm, &int0);				\
	RET_2_ARG(info,		     int0)				\
}


/* -------------------------------------------------------------------
 * info = MPI_Abort (comm, errcode)
 *				int MPI_Abort(MPI_Comm comm, int errcode)
 * ------------------------------------------------------------------- */
#define PATN_F_COM_INT(PNAME,  INTNAME)	/* programNAME, int-arg-NAME */	\
{									\
	NARGCHK       (PNAME,2)						\
	BLCK_ONECOM   (PNAME,0)		/* comm    */			\
	BLCK_ONEINT   (PNAME,1,INTNAME)	/* errcode */			\
									\
	int info=PNAME(comm,   INTNAME);				\
	RET_1_ARG     (info)						\
}


/* -------------------------------------------------------------------
 * [info intracom] = MPI_Intercomm_merge (intercom, high)
 *	int MPI_Intercomm_merge(MPI_Comm comm, int high, MPI_Comm *newcomm)
 * ------------------------------------------------------------------- */
#define PATN_COM_F_COM_INT(PNAME,INAM)	/* programNAME, int-arg-NAME */	\
{					/* -> hGrp.h ? */		\
	NARGCHK       (PNAME,2)						\
	BLCK_ONECOM   (PNAME,0)		/* comm */			\
	BLCK_ONEINT   (PNAME,1,INAM)	/* high */			\
									\
	MPI_Comm                    intracomm=0;			\
	int info=PNAME(comm, INAM, &intracomm );			\
	RET_2_ARG     (info,						\
		      MPITB_intcast(intracomm ))			\
}


/* ------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Cross-ref: Functions without call pattern:
 * [info addr]   = MPI_Address       (var)
 *  info         = MPI_Buffer_attach (buf)
 *  info         = MPI_Buffer_detach (   )
 * [info children errs] = MPI_Comm_spawn (prog,args,nproc, inf,root,parents)
 * [info children errs] = MPI_Comm_spawn_multiple
 * 					 (cprg,carg,cnprc,cinf,root,parents)
 * ----------- shared .oct file --------------------------------------------
 * [info eh]     = MPI_Errhandler_create ('OctaveCMD')
 *  info         = MPI_Errhandler_free   (eh)
 * -------------------------------------------------------------------------
 * [info count]  = MPI_Get_count     (stat)
 * [info elems]  = MPI_Get_elements  (stat, type)
 *  info         = MPI_Init        [ ('arg' [, 'arg']... ) ]
 * ----------- shared .oct file --------------------------------------------
 * [info kv]     = MPI_Keyval_create (copy_fn, del_fn, extra_st)
 *  info         = MPI_Keyval_free         (kv)
 *  info         = MPI_Attr_put      (comm, kv, attr)
 * -------------------------------------------------------------------------
 *  info         = MPI_Pack          (var, buf, pos, comm)
 * [info size ]  = MPI_Pack_size     (var,           comm)
 * [info retnams]= MPI_Unpack        (     buf, pos, comm [,'vname']... )
 * 		   print_info        ( octave_value )
 * 		   unsetenv          ('env_var_name')
 *
 *   and all MPI constants (MPI_COMM_NULL, MPI_GROUP_EMPTY...)
 * ------------------------------------------------------------------------- */

