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
 * Type-saver for constants: most LAM/OMPI constants are #defined symbols
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * GEN_1_PTR: Generate code to return an MPI #defined pointer, such as
 *  typedef struct _comm            *MPI_Comm;
 *  #define MPI_COMM_WORLD         ((MPI_Comm) &lam_mpi_comm_world)
 * (definition stolen from mpi.h)
 *
 * 		before we #undef it (to be able to DEFUN_DLD it)
 * -------------------------------------------------------------------------
 * RET_1_PTR: Call the above generated code to return the MPI pointer
 * 		these macros always used in pairs, they rely on name retcode()
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_BAND.cc              MPI_ERRORS_RETURN.cc  MPI_NULL_COPY_FN.cc
 * MPI_BOR.cc               MPI_GROUP_EMPTY.cc    MPI_NULL_DELETE_FN.cc
 * MPI_BXOR.cc              MPI_GROUP_NULL.cc     MPI_OP_NULL.cc
 * MPI_COMM_NULL.cc         MPI_INFO_NULL.cc      MPI_PROD.cc
 * MPI_COMM_SELF.cc         MPI_LAND.cc           MPI_REPLACE.cc
 * MPI_COMM_WORLD.cc        MPI_LOR.cc            MPI_REQUEST_NULL.cc
 * MPI_DUP_FN.cc            MPI_LXOR.cc           MPI_SUM.cc
 * MPI_ERRHANDLER_NULL.cc   MPI_MAX.cc
 * MPI_ERRORS_ARE_FATAL.cc  MPI_MIN.cc
 * -------------------------------------------------------------------------
 */

#define GEN_1_PTR(PNAME)						\
MPITB_OctPtrTyp retcode(void){						\
	return MPITB_intcast(PNAME);					\
}

#define RET_1_PTR							\
	RET_1_ARG (retcode());


/* -------------------------------------------------------------------------
 * GEN_1_INT: Generate code to return a MPI #defined enum-int, such as
 *  #define MPI_IDENT               1
 * or
 *  enum {  MPI_IDENT , MPI_CONGRUENT , MPI_SIMILAR , MPI_UNEQUAL};
 * (definition stolen from mpi.h)
 *
 * 		before we #undef it (to be able to DEFUN_DLD it)
 * -------------------------------------------------------------------------
 * RET_1_INT: Call the above generated code to return the MPI enum-int
 * 		these macros always used in pairs, they rely on name retcode()
 * -------------------------------------------------------------------------
 * crossref: used from
 * -------------------------------------------------------------------------
 *	MPI_IDENT	MPI_CONGRUENT	MPI_SIMILAR	MPI_UNEQUAL
 * -------------------------------------------------------------------------
 * --------------------(LAM/OMPI)------------
 *	MPI_ANY_SOURCE	(-1/=)	MPI_PROC_NULL	(-2/=)	MPI_CANCEL_SOURCE(-3/?)
 *	MPI_ROOT	(-4/=)	MPI_ANY_TAG	(-1/=)	MPI_GER_TAG	 (-2/?)
 *	MPI_MERGE_TAG	(-3/?)	next4:256/256/64/-32766	MPI_MAX_PROCESSOR_NAME
 *	MPI_MAX_ERROR_STRING	MPI_MAX_OBJECT_NAME	MPI_UNDEFINED		
 *	MPI_CART	( 1/=)	MPI_GRAPH	( 2/=)	MPI_KEYVAL_INVALID(-1=)
 * -------------------------------------------------------------------------
 * ----------------------------	(OMPI/LAM)		MPI_SUCCESS	  ( 0=)
 *	MPI_ERR_BUFFER (1=)	MPI_ERR_COUNT   ( 2/=)	MPI_ERR_TYPE	  ( 3=)
 *	MPI_ERR_TAG		MPI_ERR_COMM		MPI_ERR_RANK	  ( 6=)
 *	MPI_ERR_REQUEST		MPI_ERR_ROOT		MPI_ERR_GROUP	  ( 9=)
 *	MPI_ERR_OP		MPI_ERR_TOPOLOGY	MPI_ERR_DIMS	  (12=)
 *	MPI_ERR_ARG		MPI_ERR_UNKNOWN		MPI_ERR_TRUNCATE  (15=)
 *	MPI_ERR_OTHER		MPI_ERR_INTERN		MPI_ERR_IN_STATUS (18=)
 *	MPI_ERR_PENDING (19=)	MPI_ERR_ACCESS  (20/?)	MPI_ERR_AMODE	  (21?)
 *	MPI_ERR_ASSERT	  (?)	MPI_ERR_BAD_FILE   (?)	MPI_ERR_BASE	 (24#36)
 *	MPI_ERR_CONVERSION(?)	MPI_ERR_DISP	   (?)	MPI_ERR_DUP_DATAREP(27?)
 *	MPI_ERR_FILE_EXISTS?)	MPI_ERR_FILE_IN_USE(?)	MPI_ERR_FILE	  (30?)
 *	MPI_ERR_INFO_KEY(31#33)MPI_ERR_INFO_NOKEY(32#29)MPI_ERR_INFO_VALUE33#34)
 *	MPI_ERR_INFO	(34?)	MPI_ERR_IO	 (35?)	MPI_ERR_KEYVAL	 (36#28)
 *	MPI_ERR_LOCKTYPE(37?)	MPI_ERR_NAME	 (38#26)MPI_ERR_NO_MEM	 (39#35)
 *	MPI_ERR_NOT_SAME(40?)	MPI_ERR_NO_SPACE (41?)MPI_ERR_NO_SUCH_FILE42?)
 *	MPI_ERR_PORT	(43?)	MPI_ERR_QUOTA	 (44?)	MPI_ERR_READ_ONLY(45?)
 *	MPI_ERR_RMA_CONFLICT46?	MPI_ERR_RMA_SYNC (47?)	MPI_ERR_SERVICE	 (48#25)
 *	MPI_ERR_SIZE	(49?)MPI_ERR_SPAWN(50#27)MPI_ERR_UNSUPPORTED_DATAREP 51?
 *	MPI_ERR_UNSUPPORTED_OPERATION52?MPI_ERR_WIN53#30)MPI_ERR_LASTCODE54#37)
 *	MPI_ERR_SYSRESOURCE (-2#20)
 * -------------------------------------------------------------------------
 *	MPI_ERR_LOCALDEAD(?/21)	MPI_ERR_REMOTEDEAD (?/22)
 *	MPI_ERR_VALUE	 (?/23)	MPI_ERR_FLAGS	   (?/24)	
 *	MPI_ERR_EPOCH	 (?/31)	MPI_ERR_TYPENOTSUP (?/32)
 * -------------------------------------------------------------------------
 */

#define GEN_1_INT(PNAME)						\
int retcode(void){							\
	return   (PNAME);						\
}

#define RET_1_INT							\
	RET_1_ARG (retcode());

/* --------------------- documentation crossrefs ---------------------------
 * -------------------------------------------------------------------------
 * collective.m:
 *	MPI_MAX		MPI_LAND	MPI_LXOR
 *	MPI_MIN		MPI_BAND	MPI_BXOR
 *	MPI_SUM		MPI_LOR		MPI_REPLACE
 *	MPI_PROD	MPI_BOR		MPI_OP_NULL
 * -------------------------------------------------------------------------
 * comms.m ->		MPI_IDENT
 *	MPI_COMM_WORLD	MPI_CONGRUENT
 *	MPI_COMM_SELF	MPI_SIMILAR
 *	MPI_COMM_NULL	MPI_UNEQUAL
 * -------------------------------------------------------------------------
 * errors.m:
 *	MPI_ERRORS_ARE_FATAL	MPI_ERRHANDLER_NULL	MPI_ERRCODES
 *	MPI_ERRORS_RETURN	OCTAVE_ERRORS_RETURN	MPI_SUCESS
 * mpi_errcodes.m:
 *	up to MPI_ERR_LASTCODE (37 in LAM, 54+1 in OpenMPI) error codes
 *	+ MPI_SUCCESS=0
 * -------------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * -------- Other solutions: #1 --------------------------------------------
 * ------------------------------------------------------------------------- */
#if 0
// See CPP manual, Section "Macros", "Macro Pitfalls", "Argument Prescan"
// and compare the 2nd "special case" by which prescan is required
// to what here happens: cannot pass #defined symbol as 1st arg to DEFUN_DLD
// because it gets expanded, since stringification occurs some level below

// Brute force: undefine, define later again (dependency on LAM version?)

#undef		MPI_COMM_WORLD
#define   NAME	MPI_COMM_WORLD
DEFUN_DLD(NAME, args, nargout,
"MPI_COMM_WORLD         MPITB constant for corresponding communicator object\n\
")
{
        NARGCHK (NAME,0)        // Take advantage it's currently undefined
#define MPI_COMM_WORLD ((MPI_Comm) &lam_mpi_comm_world)
        RET_1_ARG(MPITB_intcast( NAME ))        // defined -> expanded
}
#endif

/* -------------------------------------------------------------------------
 * -------- Other solutions: #2 --------------------------------------------
 * ------------------------------------------------------------------------- */
#if 0
// More Brute force: generate code and then undefine

MPITB_OctPtrTyp retcode(void){ return MPITB_intcast(MPI_COMM_WORLD); }
#undef          MPI_COMM_WORLD

#define   NAME  MPI_COMM_WORLD
DEFUN_DLD(NAME, args, nargout,
"MPI_COMM_WORLD         MPITB constant for corresponding communicator object\n\
\n\
")
{
        NARGCHK (NAME,0)
        RET_1_ARG(retcode())
}
#endif

/* -------------------------------------------------------------------------
 * -------- Other solutions: #3 --------------------------------------------
 * ------------------------------------------------------------------------- */
#if 0
// Simply reorder for conciseness

#define   NAME  MPI_COMM_WORLD
MPITB_OctPtrTyp retcode(void){ return MPITB_intcast(NAME); }
#undef          MPI_COMM_WORLD

#endif

/* -------------------------------------------------------------------------
 * -------- Other solutions: #4 --------------------------------------------
 * ------------------------------------------------------------------------- */
#if 0
// Final solution with save-typing macro
#define	  NAME	MPI_COMM_WORLD
GEN_1_PTR(NAME)
#undef          MPI_COMM_WORLD
DEFUN_DLD(NAME, args, nargout,
"MPI_COMM_WORLD         MPITB constant for corresponding communicator object\n\
")
{
        NARGCHK (NAME,0)
        RET_1_PTR
}
#endif
