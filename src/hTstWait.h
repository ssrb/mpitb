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
 * Request completion: Test/Wait All/Some/Any  [Test_]Cancel[led] Start[all]
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------
 * [info flag     stat ] = MPI_Test     (req)
 * [info flag     stats] = MPI_Testall  (req [,req]...)
 * [info idxs     stats] = MPI_Testsome (req [,req]...)
 * [info idx flag stat ] = MPI_Testany  (req [,req]...)
 *
 *  info                 = MPI_Cancel         (req )
 * [info     flag stat ] = MPI_Test_cancelled (stat)
 *
 * [info     flag stat ] = MPI_Iprobe   (src, tag, comm)
 * [info          stat ] = MPI_Probe    (src, tag, comm)
 *
 * [info          stat ] = MPI_Wait     (req)
 * [info          stats] = MPI_Waitall  (req [,req]...)
 * [info idxs     stats] = MPI_Waitsome (req [,req]...)
 * [info idx      stat ] = MPI_Waitany  (req [,req]...)
 *
 *  info                 = MPI_Start    (req)
 *  info                 = MPI_Startall (req [,req]...)
 *
 *  info		 = MPI_Request_free (req)
 * ------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 * all those "req" args must be passed by reference (pointer) to LAM/MPI,
 * which reserves for itself the right to modify the req parameter.
 * Octave user migth find his right-hand-side req arg changed on return
 * That's why they must be real variables, not literal values
 *
 * Additionally:
 * We have to copy in/out reqs, instead of passing the real address to MPI
 *
 * Rationale:
 * It seemed natural to cast the pointer "req" argument to an octave scalar
 * (which is double), ie, double scalar <- (int) typecast <- MPI_Request req
 * 	ie "MPITB_intcast( req )" (see PATN_REQ_F_B2IC in hSend)
 *
 * but then we cannot pass the octave scalar address directly to MPI,
 * who expects a pointer and is offered a double... D-oh. you know.
 * for instance, integer "1" ie 0x00 00 00 01
 *    is cast to double         0x0000 0000 0000 F03F
 * 3FF0 == 0|011 1111 1111|0000)2
 *    sign +|biased exp 0 |mantissa 0, implicit 1, ie, 1x2^0 == 1
 *
 * you don't want MPI to see that (nil) pointer starting at &octave_value_rep
 *
 * Second thought:
 * Anyways, for multiple requests, the octave user cannot place all the
 * pointers into contiguous memory... they're objects (scalars)
 * Even if right-hand-arguments were a vector, sizeof(*)==4 < 8==sizeof(dbl)
 * ie, they wouldn't be contiguous as seen from MPI
 *
 * Concluding:
 * so, not big deal, we copy in/out. Hope this explanation doesn't make it
 * more obscure than it is (which is really not)
 * ------------------------------------------------------------------- */

#include "hStat.h"              // put/get/is_MPI_Stat(), put_MPI_Stats()


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/*
 * Notice: all patterns have at least 1 req, except for MPI_Test_cancelled
 * handy to have nargin defined here, saves typing
 */

/* -------------------------------------------------------------------------
 * REQ_ONE: Take (the) one req argument into source code (name req always)
 * REQ_ONE_END: companion macro, copies req out (must use both in pairs)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hTstWait.h: (3 uses)	PATN_INT_STA_F_REQ	PATN_STA_F_REQ
 * 			PATN_F_REQ		
 * -------------------------------------------------------------------------
 */
#define REQ_ONE(    PNAME)						\
	NARGCHK    (PNAME,1)		/* nargin */			\
	BLCK_ONESYM(PNAME,0,req)	/* reqnam, reqsym */		\
									\
	if(! MPITB_isOPtrTyp(args(0))){	/* this could be BLCK_ONEREQ? */\
	   error(STRFY(PNAME) ": required arg req(ptr)");		\
	   return octave_value(MPI_ERR_ARG); /* error returns nothing */\
	}								\
	MPI_Request req = MPITB_ptrcast ( MPI_Request, args(0) );
						/* req copied in */
				

#define REQ_ONE_END				/* req copied out */	\
	reqsym->define( MPITB_intcast( req ) );
					/* MPI updates req, so do we */


/* -------------------------------------------------------------------------
 * REQ_MANY: Take nargin req arguments into source code (name reqs always)
 * REQ_MANY_END: companion macro, copies reqs out (must use both in pairs)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hTstWait.h: (6 uses)	PATN_INT_STS_F_RQS	PATN_STS_F_RQS
 * 			PATN_2INT_STA_F_REQ	PATN_INT_STA_F_RQS
 * 			PATN_INTS_STS_F_RQS	PATN_F_RQS
 * -------------------------------------------------------------------------
 */

#define REQ_MANY(PNAME)							\
	int nargin = args.length();	/* too peculiar for NARGCHK */	\
									\
	if (nargin<1){							\
	   error(STRFY(PNAME) ": at least 1 arg required");		\
	   return octave_value(MPI_ERR_ARG); /* error returns nothing */\
	}								\
									\
	for (int i=0; i<nargin; i++)	/*this could be BLCK_MANYREQS?*/\
	 if (! MPITB_isOPtrTyp(args(i))){				\
	   error(STRFY(PNAME) ": req arguments should be pointers");	\
	   return octave_value(MPI_ERR_ARG); /* error returns nothing */\
	}								\
									\
	MPI_Request reqs[nargin];					\
	for (int i=0;  i<nargin; i++){					\
	   reqs[i] = MPITB_ptrcast ( MPI_Request, args(i) );		\
	}					/* copy reqs in */


#define REQ_MANY_END				/* copy reqs out */	\
	string_vector reqstrvec = args.name_tags();			\
	for (int i=0; i<nargin; i++){					\
	  symbol_record*reqsym= curr_sym_tab -> lookup ( reqstrvec (i) );\
			reqsym->define( MPITB_intcast( reqs[i] ) );	\
	}
					/* MPI updates req, so do we */


/* -------------------------------------------------------------------------
 * intVEC_PUT: translate C-int vector to Octave-row vector
 * -------------------------------------------------------------------------
 * crossref: used just once from here	IDENTICAL COPY @ hTopo (-> mpitb.h ?)
 * hTstWait.h: (1 uses)	PATN_INTS_STS_F_RQS
 * -------------------------------------------------------------------------
 */
#define intVEC_PUT(CNAM,SZ,OCTN) /* C-vector & Oct-matrix name & size */\
	Matrix OCTN  (1,SZ,0   );					\
	for (int i=0; i<SZ; i++)					\
	     OCTN(0,i) = CNAM[i];		/* promo int->double */


/* -------------------------------------------------------------------------
 * BLCK_2IC: take src, tag, comm
 * -------------------------------------------------------------------------
 * crossref: used twice from here
 * hTstWait.h: (2 uses)	PATN_INT_STA_F_2IC	PATN_STA_F_2IC
 * -------------------------------------------------------------------------
 */
#define BLCK_2IC(   PNAME)	/* 2IC -> 2INT (src,tag), 1 Comm */	\
	NARGCHK    (PNAME,3)		/* handy argchk here */		\
	BLCK_ONEINT(PNAME,0,src)	/* src  */			\
	BLCK_ONEINT(PNAME,1,tag)	/* tag  */			\
	BLCK_ONECOM(PNAME,2)		/* comm */



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
 * [info flag stat] = MPI_Test (req)
 * ------------------------------------------------------------------- */

#define PATN_INT_STA_F_REQ(PNAME)					\
{									\
	REQ_ONE   (PNAME)						\
	int        flag =        0 ;					\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME(&req,						\
			 &flag,            &stat);			\
									\
	REQ_ONE_END							\
	RET_3_ARG (info, flag, put_MPI_Stat(stat));			\
}


/* -------------------------------------------------------------------
 * [info      stat] = MPI_Wait (req)
 * ------------------------------------------------------------------- */

#define PATN_STA_F_REQ(PNAME)						\
{									\
	REQ_ONE   (PNAME)						\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME(&req,             &stat);			\
									\
	REQ_ONE_END							\
	RET_2_ARG (info,       put_MPI_Stat(stat));			\
}


/* -------------------------------------------------------------------
 * [info flag stats] = MPI_Testall (req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_INT_STS_F_RQS(PNAME)					\
{									\
	REQ_MANY  (PNAME)						\
	int        flag =     0 ;					\
	MPI_Status stats[nargin];	/* not initialized -> MPI */	\
									\
	int info = PNAME (nargin, reqs,					\
			&flag,			    stats);		\
									\
	REQ_MANY_END							\
	RET_3_ARG (info, flag, put_MPI_Stats(nargin,stats));		\
}


/* -------------------------------------------------------------------
 * [info      stats] = MPI_Waitall (req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_STS_F_RQS(PNAME)						\
{									\
	REQ_MANY  (PNAME)						\
	MPI_Status stats[nargin];	/* not initialized -> MPI */	\
									\
	int info = PNAME(nargin, reqs,              stats);		\
									\
	REQ_MANY_END							\
	RET_2_ARG (info,       put_MPI_Stats(nargin,stats));		\
}


/* -------------------------------------------------------------------
 * [info idx flag stat] = MPI_Testany (req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_2INT_STA_F_REQ(PNAME)					\
{									\
	REQ_MANY  (PNAME)						\
	int idx=0, flag =  0;						\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME(nargin, reqs,					\
			&idx, &flag,		 &stat);		\
									\
	REQ_MANY_END							\
	RET_4_ARG (info, idx,  flag, put_MPI_Stat(stat));		\
}


/* -------------------------------------------------------------------
 * [info idx      stat] = MPI_Waitany (req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_INT_STA_F_RQS(PNAME)					\
{									\
	REQ_MANY  (PNAME)						\
	int        idx  =  0;						\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME(nargin, reqs,					\
			&idx,			 &stat);		\
									\
	REQ_MANY_END							\
	RET_3_ARG (info, idx,        put_MPI_Stat(stat));		\
}


/* -------------------------------------------------------------------
 * [info idxs stats] = MPI_Testsome(req [,req]...)
 * [info idxs stats] = MPI_Waitsome(req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_INTS_STS_F_RQS(PNAME)					\
{									\
	REQ_MANY  (PNAME)						\
	int        outcount = 0 ,					\
		   idxs [nargin];					\
	MPI_Status stats[nargin];	/* not initialized -> MPI */	\
									\
	int info = PNAME(nargin, reqs,  &outcount, idxs, stats);	\
									\
	intVEC_PUT(idxs,outcount,					\
		       	oct_idxs)					\
	REQ_MANY_END							\
	RET_3_ARG(info, oct_idxs, put_MPI_Stats(outcount,stats))	\
}


/* -------------------------------------------------------------------
 * info = MPI_Cancel      (req)
 * info = MPI_Start       (req)
 * info = MPI_Request_free(req)
 * ------------------------------------------------------------------- */

#define PATN_F_REQ(PNAME)						\
{									\
	REQ_ONE   (PNAME)						\
									\
	int info = PNAME (&req);					\
									\
	REQ_ONE_END							\
	RET_1_ARG (info);						\
}


/* -------------------------------------------------------------------
 * info = MPI_Startall    (req [,req]...)
 * ------------------------------------------------------------------- */

#define PATN_F_RQS(PNAME)						\
{									\
	REQ_MANY  (PNAME)						\
	int info = PNAME(nargin, reqs);					\
									\
	REQ_MANY_END							\
	RET_1_ARG (info);						\
}


/* -------------------------------------------------------------------
 * [info flag stat] = MPI_Test_cancelled (stat)
 * ------------------------------------------------------------------- */

#define PATN_INT_STA_F_STA(PNAME)					\
{									\
	int flag   = 0,							\
	    nargin = args.length();					\
	if((nargin != 1)						\
	 ||(! is_MPI_Stat(args(0)) )){	/* this could be BLCK_ONESTAT?*/\
	   error(STRFY(PNAME) ": required arg#1: stat(struct)");	\
	   return octave_value(MPI_ERR_ARG); /* error returns nothing */\
	}								\
	MPI_Status	  stat = get_MPI_Stat(args(0));			\
									\
	int info = PNAME(&stat, &flag);					\
									\
	RET_3_ARG (info,  flag,  put_MPI_Stat(stat));			\
}


/* -------------------------------------------------------------------
 * [info     flag stat ] = MPI_Iprobe   (src, tag, comm)
 * ------------------------------------------------------------------- */

#define PATN_INT_STA_F_2IC(PNAME)	/* 2IC -> 2ints,1comm */	\
{									\
	BLCK_2IC  (PNAME)		/* src, tag, comm */		\
									\
	int	   flag =  0;						\
	MPI_Status stat = {0,0,0,0};					\
	int info = PNAME( src,tag,comm, &flag,&stat );			\
									\
	Octave_map           os = put_MPI_Stat(stat);			\
	RET_3_ARG (info,flag,os)					\
}


/* -------------------------------------------------------------------
 * [info          stat ] = MPI_Probe    (src, tag, comm)
 * ------------------------------------------------------------------- */

#define PATN_STA_F_2IC(PNAME)		/* 2IC -> 2ints,1comm */	\
{									\
	BLCK_2IC  (PNAME)		/* src, tag, comm */		\
									\
	MPI_Status stat = {0,0,0,0};					\
	int info = PNAME( src,tag,comm,       &stat );			\
									\
	Octave_map           os = put_MPI_Stat(stat);			\
	RET_2_ARG (info,     os)					\
}

