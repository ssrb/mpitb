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
 * Point-to-point Recv functions: MPI_Recv      Sendrecv[_replace]
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------
 * [info stat] = MPI_ Recv  (buf, src,  tag, comm)
 *
 * [info stat] = MPI_Sendrecv        (sbuf, dest, stag, ...
 *                                    rbuf, src , rtag, comm)
 * [info stat] = MPI_Sendrecv_replace( buf, dest, stag, ...
 *                                          src , rtag, comm)
 * ------------------------------------------------------------------- */

#include "hSndRecv.h"		// BLCK_B2IC
#include "hStat.h"		// put_MPI_Stat()

/* -------------------------------------------------------------------
 * decided to segregate from hSend.h because of put_MPI_Stat
 * so that hStat was only needed here, not in hSend
 * previously, all (hSend/hRecv) was merged in hSndRecv
 * now hSndRecv keeps just the shared BLCK_B2IC buildblock
 * ------------------------------------------------------------------- */


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
 * ------------------------------------------------------------------------- */
                                                                                

/* -------------------------------------------------------------------
 * pattern for Recv: [int map] = f (sym, int * 2, comm)
 *
 * [info stat] = MPI_ Recv  (buf, src,  tag, comm)
 * ------------------------------------------------------------------- */

#define PATN_MAP_F_BUF_2INT_COM(PNAME)					\
{									\
	BLCK_B2IC (PNAME,1)/* BUF,SIZ,TYPE, srcdst,tag,comm, MK_UNIQ */	\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME (BUF,SIZ,TYPE, srcdst,tag,comm,		\
				       &stat);				\
									\
	Octave_map map = put_MPI_Stat  (stat);				\
	RET_2_ARG (info, map);						\
}


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 * basic buildblock for SendRecv
 *
 * [info stat] = MPI_Sendrecv        (sbuf, dest, stag, ...
 *                                    rbuf, src , rtag, comm)
 * [info stat] = MPI_Sendrecv_replace( buf, dest, stag, ...
 *                                          src , rtag, comm)
 * ------------------------------------------------------------------- */

#define BLCK_2_B2IC(PNAME,  RP)		/* twice BUF,2INT,COM */	\
	NARGCHK    (PNAME,7-RP)						\
									\
	BLCK_ONESYM(PNAME,0, snd)	/* sndnam, sndsym */		\
	BLCK_ONEINT(PNAME,1, dst)	/* dst            */		\
	BLCK_ONEINT(PNAME,2,stag)	/* stag           */		\
	BLCK_NULSYM(PNAME,3, rcv)	/* rcvnam, rcvsym */		\
				/* need special NULL checking, ...  */	\
    if((!RP) && (! rcvsym.is_defined())){	/*this !RP is why we don't  _ONESYM */	\
	  error(STRFY(PNAME) ": required arg#%d: rcvbuf(var)", 4);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
									\
	BLCK_ONEINT(PNAME,4-RP, src)	/* src            */		\
	BLCK_ONEINT(PNAME,5-RP,rtag)	/* rtag           */		\
	BLCK_ONECOM(PNAME,6-RP)		/* comm           */



/* -------------------------------------------------------------------------
 * -------- Call Patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 * [info stat] = MPI_Sendrecv        (sbuf, dest, stag, ...
 *                                    rbuf, src , rtag, comm)
 * ------------------------------------------------------------------- */

#define PATN_MAP_F_2_B2IC(PNAME)					\
{									\
	BLCK_2_B2IC  (PNAME,0)	/*ssym,dst,stag, rsym,src,rtag, comm*/	\
									\
	DECLARE_BUFF_VARS	/* BUF, SIZ, TYPE - SIZ not in bytes*/	\
	BUF = get_MPI_Buff(sndsym, SIZ,false, TYPE);			\
    if(!BUF)  return octave_value(MPI_ERR_ARG);				\
									\
	DECLARE_RECV_BUFF_VARS	/* RBUF,RSIZ, RTYPE - MK_UNIQ */	\
	RBUF = get_MPI_Buff(rcvsym,RSIZ,false,RTYPE,true);		\
    if(!RBUF)  return octave_value(MPI_ERR_ARG);			\
									\
	/* ---------------------------------------------------- */	\
	/* RECV_BUFF declares can't be included in BLCK_2_B2IC: */	\
	/* - if enclosed into "if (!RP){ }" -> out of scope now */	\
	/* - if enclosed into "#if !RP ... #endif" -> not valid */	\
	/*      cpp interprets #if as stringification           */	\
	/*      cannot include a cpp directive inside a macro   */	\
	/*      macro definitions are one-liners, and           */	\
	/*      directives require a line by themselves         */	\
	/* so for symmetry we didn't include _BUFF there either */	\
	/* ---------------------------------------------------- */	\
									\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info = PNAME (BUF, SIZ, TYPE, dst,stag,			\
			 RBUF,RSIZ,RTYPE, src,rtag,			\
				 comm, &stat);				\
									\
	Octave_map map = put_MPI_Stat  (stat);				\
	RET_2_ARG (info, map)						\
}


/* -------------------------------------------------------------------
 * [info stat] = MPI_Sendrecv_replace( buf, dest, stag, ...
 *                                          src , rtag, comm)
 * ------------------------------------------------------------------- */

#define PATN_MAP_F_2_B2IC_RP(PNAME)					\
{									\
	BLCK_2_B2IC(PNAME,1)	/*ssym,dst,stag, rsym,src,rtag, comm*/	\
									\
	DECLARE_BUFF_VARS			/* BUF, SIZ, TYPE */	\
	BUF = get_MPI_Buff(sndsym, SIZ,false,	/* SIZ not in bytes */	\
				  TYPE,true);	/* MK_UNIQ */		\
    if(!BUF)  return octave_value(MPI_ERR_ARG);				\
									\
	MPI_Status stat = {0,0,0,0};					\
									\
	int info =  PNAME (BUF,SIZ,TYPE, dst,stag,			\
					 src,rtag,			\
				comm, &stat);				\
									\
	Octave_map map = put_MPI_Stat (stat);				\
	RET_2_ARG (info, map)						\
}

