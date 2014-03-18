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
 * Collective Functions: Barrier Bcast Gather Scatter Reduce
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------
 * info = MPI_Barrier	 (comm)		// not here, see mpitb include
 *
 * info = MPI_Bcast      ( var,             root,  comm)
 *
 * info = MPI_Gather     (svar, rvar,       root,  comm)
 * info = MPI_Gatherv    (svar, rvar, cnts, disps,
 *                                          root,  comm)
 * info = MPI_Allgather  (svar, rvar,              comm)
 * info = MPI_Allgatherv (svar, rvar, cnts, disps, comm)
 *
 * info = MPI_Scatter    (svar, rvar,       root,  comm)
 * info = MPI_Scatterv   (svar,       cnts, disps,
 *                              rvar,       root,  comm)
 * info = MPI_Alltoall   (svar, rvar, srcnt,       comm)
 * info = MPI_Alltoallv  (svar,       scnts,sdisps,
 *                              rvar, rcnts,rdisps,comm)
 *
 * info = MPI_Reduce     (svar, rvar, op,   root,  comm)
 * info = MPI_Allreduce  (svar, rvar, op,          comm)
 * info = MPI_Scan       (svar, rvar, op,          comm)
 * info = MPI_Reduce_scatter(svar, rvar, cnts, op, comm)
 * ------------------------------------------------------------------- */

#include "hBuff.h"		// type enums - declare vars - aux get_MPI_Buff

/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * ROOTCOM_GET:
 * Take root/comm pair of ARG Nth/N+1th into source code (those names always)
 * See ROOTCOM_GET comment in hInfo.h, this macro would be handy there too
 * (instead of BLCK_ONEINT/BLCK_ONECOM)
 * -------------------------------------------------------------------------
 * crossref: used from
 * hColl.h (6 uses):	PATN_F_VRC	PATN_F_SRRC
 *			PATN_SRORC	PATN_SRCDRC	PATN_SCDRRC
 * -------------------------------------------------------------------------
 */					/* program NAME, root is ARG Nth*/
#define	ROOTCOM_GET(PNAME,ARGN)	/* every root followed by comm (6patt) */\
	if (( ! args(ARGN  ).is_real_scalar() )				\
/*	 || ( ! args(ARGN+1).is_real_scalar())){	*/		\
	 || ( ! MPITB_isOPtrTyp(args(ARGN+1)))){			\
	  error(							\
	    STRFY(PNAME) ": required args#%d-%d: root(int), comm(ptr)",	\
				ARGN+1,ARGN+2);				\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
int	 root =	args(ARGN  ).int_value();				\
MPI_Comm comm = MPITB_ptrcast(MPI_Comm, args(ARGN+1));


/* -------------------------------------------------------------------------
 * BLCK_OP_GET:
 * Take Op ARG Nth into source code (name Op always)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hColl.h (3 uses):	PATN_SROC	PATN_SRORC	PATN_SRCOC
 * 	   (4 funs)
 * -------------------------------------------------------------------------
 */					/* program NAME, root is ARG Nth*/
#define	BLCK_OP_GET(PNAME,ARGN)		/* common to 4 call patterns */	\
/*	if  ( ! args(ARGN  ).is_real_scalar()){	*/			\
	if  ( ! MPITB_isOPtrTyp(args(ARGN  ))){				\
	  error(STRFY(PNAME) ": required arg#%d: op(ptr)", ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
MPI_Op  Op = MPITB_ptrcast(MPI_Op, args(ARGN  ));


/* -------------------------------------------------------------------------
 * BLCK_intVEC:
 * Take int-vector ARG Nth/N+1th into source code (chosen name, prefixed)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hColl.h (3 uses):	BCDTYPE_GET (2)	PATN_SRCOC (1)
 * -------------------------------------------------------------------------
 */				/* this could be hint for BLCK_VECINT? */
#define BLCK_intVEC(PNAME,ARGN,P,NM)	/* P f/PREFIX, NM f/varNAME */	\
	if ( ! args(ARGN).is_real_matrix() ){				\
	  error(STRFY(PNAME)		": required arg#%d: "		\
		STRFY(P) STRFY(NM)	"(intvec)",	ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
double*	P##NM##data = const_cast<double*>(				\
			args(ARGN).matrix_value().data() );		\
int	P##NM##size =	args(ARGN).matrix_value().capacity(),		\
P##NM  [P##NM##size];							\
									\
	     for (int i=0; i<P##NM##size; i++)				\
P##NM[i] = static_cast<int>( P##NM##data [i] );


/* -------------------------------------------------------------------------
 * BCNTYPE_GET:
 * Take Buffer/count/type from ARG Nth into source code (names prefixed)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hColl.h (17 uses):	BCDTYPE_GET (1)
 * all other PATN_ except for PATN_SSSRRRC (which uses BCDTYPE_GET twice)
 * most PATN_ use this macro twice, for send / recv buffers, prefixed S/R
 * -------------------------------------------------------------------------
 */				/* this could be hint for BLCK_VECINT? */
					/* P for PREFIX, var is ARGNth */
#define BCNTYPE_GET(PNAME,ARGN,P,MKUNIQ)/* BCNTYPE for Buff/Count/Type*/\
	BLCK_NULSYM(PNAME,ARGN,P)	/* get symbol, no NULL check */	\
	int	P##CNT  =		args(ARGN).length();		\
MPI_Datatype	P##TYPE = get_MPI_Type (args(ARGN));			\
	void *	P##BUF  = P##sym.is_defined() ?					\
			get_MPI_Buff(P##sym,				\
				     P##CNT,false,/*CNT NOT in bytes */	\
				     P##TYPE,MKUNIQ)			\
			:	0;
// Just a thought... args().length() ?!? that's max dim value (see ov.cc)
// never tried this with 2D ?!? double-check if it should be capacity/nelem/etc
/*--------------------------------------------------------------------
 *  If we used _ONESYM, Octave user should provide a valid variable
 *  at every rank, not just at root. We could have asked for MPI_Comm_rank
 *  and check NULLsyms only when this is the root or cmd has no root.
 *  Alternatively, we can provide not-too-bad default values for the
 *  assigned variables in case the buffer is not a real variable
 *  -- ie, Octave user is only interested in indicating MPI_Type
 *  -- as happens with
 *  	reduce buffer	at non-root-ranks	in MPI_Reduce,
 *	send var	at non-root-ranks	in MPI_Scatter/Scatterv,
 *	recv var	at non-root-ranks	in MPI_Gather /Gatherv , etc
 *--------------------------------------------------------------------
 */


/* -------------------------------------------------------------------------
 * BCDTYPE_GET:
 * Take BCNTYPE ARG Nth and counts/displacements ARGs N+1/N+2th
 * into source code (names prefixed)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hColl.h (5 uses):	PATN_SRCDC	PATN_SRCDRC	PATN_SCDRRC
 * 	   (4 func)	PATN_SSSRRRC (2)
 * one PATN_ use this macro twice, for send/recv buff/cnt/disp: MPI_Alltoallv
 * -------------------------------------------------------------------------
 */				/* this could be hint for BLCK_VECINT? */
#define BCDTYPE_GET(PNAME,ARGN,  P,MKUNIQ)/* BCD for Buff/Counts/Disps */\
	BCNTYPE_GET(PNAME,ARGN,  P,MKUNIQ)				\
	BLCK_intVEC(PNAME,ARGN+1,P,cnts );				\
	BLCK_intVEC(PNAME,ARGN+2,P,disps);


/* -------------------------------------------------------------------------
 * BLCK_CHGTYPE:
 * Change already obtained STYPE (MPI type of source buffer)
 * from MPI_CHAR -> MPI_UNSIGNED_CHAR or MPI_BYTE
 * from MPI_CXX_BOOL -> MPI_BYTE
 * so that we can at least MPI_Reduce(MPI_REPLACE) with strings and bools
 * MPI_REPLACE is the only op that works with chars, and wants them unsigned
 * 						(alternatively, wants BYTEs)
 * bools accept the logicals (MPI_LAND...) so change only with MPI_REPLACE
 * see <LAM_SRC_DIR>/share/mpi/lamreduce.c
 * TODO: support reduce on complex types?
 * -------------------------------------------------------------------------
 * crossref: used just from here, reduce funcs
 * hColl.h (3 uses):	PATN_SROC	PATN_SRORC	PATN_SRCOC
 * 	   (4 func)	
 * -------------------------------------------------------------------------
 */
#define BLCK_CHGTYPE		/*CHAR->UCHAR, BOOL->BYTE, f/REPLACE*/	\
	if (Op==MPI_REPLACE)		/* BUG: rethink BOOL/CPLX */	\
	  if (	(STYPE==MPI_CXX_BOOL) ||				\
		(STYPE==MPI_CHAR)	)				\
		 STYPE =MPI_BYTE;

/* -------------------------------------------------------------------------
 * -------- Call Patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 * we make even individual patterns for just 1 MPI routine
 * it's easier to read them all together - MPITB easier to maintain
 *
 * all patterns carry the PNAME macro: code more reutilizable,
 * and allows for stringification at the lowest level (error messages)
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 * info = MPI_Bcast      ( var,             root,  comm)
 * ----------------
 * var  Octave variable used as msg src (at root) or msg dst (all others)
 *			=> MK_UNIQ at all ranks, don't decide at runtime
 * -------------------------------------------------------------------
 * int MPI_Bcast(void *buff, int count, MPI_Datatype datatype,
 *					int root, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_F_VRC( PNAME)		/* VRC -> var, root, comm */	\
{									\
	NARGCHK    (PNAME,3)		/*MKUNIQ on allrnks(but root)*/	\
	BCNTYPE_GET(PNAME,0,var,1)	/*varnam/sym, varCNT/TYPE/BUF*/	\
	ROOTCOM_GET(PNAME,1)		/*root, comm*/			\
									\
	int info =  PNAME (varBUF,varCNT,varTYPE, root,comm);		\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * info = MPI_Allgather  (svar, rvar,              comm)
 * --------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  Octave variable used as msg dst (at each rank) => MK_UNIQ at all ranks
 * -------------------------------------------------------------------
 * int MPI_Allgather(	void *sbuf, int scount, MPI_Datatype sdtype,
 *			void *rbuf, int rcount, MPI_Datatype rdtype,
 *						MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_F_SRC( PNAME)		/* SRC -> srcvar,rcvvar, comm */\
{									\
	NARGCHK    (PNAME,3)		/* MKUNIQ rvar / all ranks */	\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	RCNT = SCNT;		/* avoid SIGSEGV, never let RCNT>SCNT */\
	BLCK_ONECOM(PNAME,2  )		/* comm */			\
									\
	int info =  PNAME (SBUF,SCNT,STYPE,				\
			   RBUF,RCNT,RTYPE,				\
			   comm);					\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * info = MPI_Gather     (svar, rvar,       root,  comm)
 * info = MPI_Scatter    (svar, rvar,       root,  comm)
 * ------------------
 * rvar  Octave variable used as msg dst (at root), or meaningless (all others)
 * rvar  Octave variable used as msg dst (at each rank) => MK_UNIQ all ranks
 * -------------------------------------------------------------------
 * int MPI_Gather(	void *sbuf, int scount, MPI_Datatype sdtype,
 *			void *rbuf, int rcount, MPI_Datatype rdtype,
 *						int root, MPI_Comm comm)
 * int MPI_Scatter(	void *sbuf, int scount, MPI_Datatype sdtype,
 *			void *rbuf, int rcount, MPI_Datatype rdtype,
 *						int root, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_F_SRRC(PNAME)		/* SRRC -> src/rcvar,root,comm*/\
{									\
	NARGCHK    (PNAME,4)						\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	ROOTCOM_GET(PNAME,2  )		/* root, comm */		\
									\
	if (STRFY(PNAME)=="MPI_Gather")				\
		RCNT = SCNT;	/* R could be [] at non-root ranks */	\
  else	if (STRFY(PNAME)=="MPI_Scatter")				\
		SCNT = RCNT;	/* R could be [] at non-root ranks */	\
  else	warning_with_id("MPITB:internal-errors",			\
	STRFY(PNAME) "() -> using wrong code pattern PATN_F_SRRC()\n"	\
	"\t This should never happen");					\
									\
	int info =  PNAME (SBUF,SCNT,STYPE,				\
			   RBUF,RCNT,RTYPE,				\
			   root,comm);					\
	RET_1_ARG ( info )						\
}


/* -------------------------------------------------------------------
 * info = MPI_Alltoall   (svar, rvar, srcnt,       comm)
 * ------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  Octave variable used as msg dst (at each rank) => MK_UNIQ
 * srcnt  (int)  number of elements to (send to/recv from) each process
 * -------------------------------------------------------------------
 * int MPI_Alltoall(	void *sbuf, int scount, MPI_Datatype sdtype,
 *			void* rbuf, int rcount, MPI_Datatype rdtype,
 *						MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_F_SRCC(PNAME)		/* SRCC -> src/rcvar,cnt,comm*/	\
{									\
	NARGCHK    (PNAME,  4)						\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	BLCK_ONEINT(PNAME,2,srcnt)	/* srcnt */			\
	BLCK_ONECOM(PNAME,3)		/* comm */			\
									\
	SCNT = RCNT = srcnt;	/* See MPI man page. Braindamage!! */	\
									\
	int info =  PNAME (SBUF,SCNT,STYPE,				\
			   RBUF,RCNT,RTYPE,				\
			   comm);					\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * info = MPI_Allreduce  (svar, rvar, op,          comm)
 * info = MPI_Scan       (svar, rvar, op,          comm)
 * --------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  Octave variable used as red.dst (at each rank) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Allreduce(	void *sbuf, void *rbuf, int count, MPI_Datatype dtype,
 *				MPI_Op op, MPI_Comm comm)
 * int MPI_Scan(	void *sbuf, void *rbuf, int count, MPI_Datatype dtype,
 *				MPI_Op op, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SROC(  PNAME)		/* SROC -> src/rcvar, op,comm */\
{									\
	NARGCHK    (PNAME,4)						\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	BLCK_OP_GET(PNAME,2  )		/* Op   */			\
	BLCK_ONECOM(PNAME,3  )		/* comm */			\
									\
	BLCK_CHGTYPE		/*CHAR->UCHAR, BOOL->BYTE, f/REPLACE*/	\
									\
	int info =  PNAME (SBUF, RBUF,					\
			   SCNT,STYPE,					\
			   Op, comm);					\
	RET_1_ARG  (info);						\
}


/* -------------------------------------------------------------------
 * info = MPI_Reduce     (svar, rvar, op,   root,  comm)
 * -----------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  Oct.var. reduce buffer (at root), meaningless at others => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Reduce(void *sbuf, void* rbuf, int count, MPI_Datatype dtype,
 *				MPI_Op op, int root, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SRORC( PNAME)	/* SRORC -> src/rcvar, op, root,comm */	\
{									\
	NARGCHK    (PNAME,5)						\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	BLCK_OP_GET(PNAME,2  )		/* Op         */		\
	ROOTCOM_GET(PNAME,3  )		/* root, comm */		\
									\
	BLCK_CHGTYPE		/*CHAR->UCHAR, BOOL->BYTE, f/REPLACE*/	\
									\
	int info =  PNAME (SBUF, RBUF,					\
			   SCNT,STYPE,					\
			   Op, root, comm);				\
	RET_1_ARG  (info);						\
}


/* -------------------------------------------------------------------
 * info = MPI_Reduce_scatter(svar, rvar, cnts, op, comm)
 * -------------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  Octave variable used as red.dst (at each rank) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Reduce_scatter(
 *		void *sbuf, void *rbuf, int *rcounts, MPI_Datatype dtype,
 *				MPI_Op op, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SRCOC( PNAME)	/* SRCOC -> src/rcvar, cnt, op, comm */	\
{									\
	NARGCHK    (PNAME,5)						\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCNTYPE_GET(PNAME,1,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	BLCK_intVEC(PNAME,2,R,cnts)	/* Rdata,Rsize,Rcnts */		\
	BLCK_OP_GET(PNAME,3  )		/* Op   */			\
	BLCK_ONECOM(PNAME,4  )		/* comm */			\
									\
	BLCK_CHGTYPE		/*CHAR->UCHAR, BOOL->BYTE, f/REPLACE*/	\
									\
	int info =  PNAME(SBUF, RBUF,Rcnts,				\
			  STYPE,					\
			  Op, comm);					\
									\
	RET_1_ARG  (info);						\
}


/* -------------------------------------------------------------------
 * info = MPI_Allgatherv (svar, rvar, cnts, disps, comm)
 * ---------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  "..." as msg dst (at each rank) (room for all incoming) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Allgatherv(void *sbuf, int scount, MPI_Datatype sdtype,
 *		      void *rbuf, int*rcounts, int *disps, MPI_Datatype rdtype,
 *				MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SRCDC( PNAME)						\
{									\
	NARGCHK    (PNAME,  5)	/* SRCDC -> src/rcvar,cnts,disps,comm */\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCDTYPE_GET(PNAME,1,R,1)	/* Rnam/.../BUF, Rcnts/disps */	\
	BLCK_ONECOM(PNAME,4  )		/* comm */			\
									\
	int info =  PNAME (SBUF,SCNT,STYPE,				\
			   RBUF,Rcnts,Rdisps,RTYPE,			\
			   comm);					\
									\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * info = MPI_Gatherv    (svar, rvar, cnts, disps,
 *                                          root,  comm)
 * ------------------
 * svar  Octave variable used as msg src
 * rvar  "..." as msg dst (at root), or meaningless (all others) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Gatherv(void *sbuf, int scount, MPI_Datatype sdtype,
 *		void *rbuf, int *rcounts, int *disps, MPI_Datatype rdtype,
 *				int root, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SRCDRC(PNAME)						\
{									\
	NARGCHK    (PNAME,6)	/* SRCDRC->s/rvar,cnts,disps,root,comm */\
	BCNTYPE_GET(PNAME,0,S,0)	/* Snam,Ssym, SCNT/TYPE/BUF */	\
	BCDTYPE_GET(PNAME,1,R,1)	/* Rnam/.../BUF, Rcnts/disps */	\
	ROOTCOM_GET(PNAME,4  )	/* root, comm */			\
	int info =  PNAME (SBUF,SCNT,STYPE,				\
			   RBUF,Rcnts,Rdisps,RTYPE,			\
			   root,comm);					\
									\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * info = MPI_Scatterv   (svar,       cnts, disps,
 *                              rvar,       root,  comm)
 * -------------------
 * svar  Octave variable used as msg src (at root), or meaningless (all others)
 * rvar  "..." as msg dst (room for cnts[rank] elements) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Scatterv(void *sbuf, int *scounts, int *disps, MPI_Datatype sdtype,
 *			void* rbuf, int rcount, MPI_Datatype rdtype,
 *				int root, MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SCDRRC(PNAME)						\
{									\
	NARGCHK    (PNAME,6)	/* SCDRRC->svar,cnts,disps,rvar,root,comm */\
	BCDTYPE_GET(PNAME,0,S,0)	/* Snam/.../BUF, Scnts/disps */	\
	BCNTYPE_GET(PNAME,3,R,1)	/* Rnam,Rsym, RCNT/TYPE/BUF */	\
	ROOTCOM_GET(PNAME,4  )		/* root, comm */		\
									\
	int info =  PNAME (SBUF,Scnts,Sdisps,STYPE,			\
			   RBUF,RCNT,RTYPE,				\
			   root,comm);					\
									\
	RET_1_ARG  (info);						\
}


/* -------------------------------------------------------------------
 * info = MPI_Alltoallv  (svar,       scnts,sdisps,
 *                              rvar, rcnts,rdisps,comm)
 * --------------------
 * svar  Octave variable used as msg src (at each rank)
 * rvar  "..." as msg dst (at each rank) (room for all incoming) => MK_UNIQ
 * -------------------------------------------------------------------
 * int MPI_Alltoallv(
 *	void *sbuf, int *scounts, int *sdisps, MPI_Datatype sdtype,
 *	void *rbuf, int *rcounts, int *rdisps, MPI_Datatype rdtype,
 *							MPI_Comm comm)
 * ------------------------------------------------------------------- */

#define PATN_SSSRRRC(PNAME)						\
{									\
	NARGCHK    (PNAME,  7)	/*SSSRRRC->svar/cnt/dsp,rvar/cnt/dsp,comm*/\
	BCDTYPE_GET(PNAME,0,S,0)	/* Snam/.../BUF, Scnts/disps */	\
	BCDTYPE_GET(PNAME,3,R,1)	/* Rnam/.../BUF, Rcnts/disps */	\
	BLCK_ONECOM(PNAME,6  )		/* comm */			\
									\
	int info =  PNAME (SBUF,Scnts,Sdisps,STYPE,			\
			   RBUF,Rcnts,Rdisps,RTYPE,			\
			   comm);					\
									\
	RET_1_ARG  (info);						\
}

