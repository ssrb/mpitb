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
 * Point-to-point functions: MPI_ [I][-bsr]Send/Recv      Sendrecv[_replace]
 * Persistent req functions: MPI_ [I][-bsr]Send/Recv _init
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------
 * info        = MPI_ Send  (buf, dest, tag, comm)
 * info        = MPI_Bsend  (buf, dest, tag, comm)
 * info        = MPI_Ssend  (buf, dest, tag, comm)
 * info        = MPI_Rsend  (buf, dest, tag, comm)
 *
 * [info stat] = MPI_ Recv  (buf, src,  tag, comm)
 *
 * [info req ] = MPI_Irecv  (buf, src,  tag, comm)
 *
 * [info req ] = MPI_I send (buf, dest, tag, comm)
 * [info req ] = MPI_Ibsend (buf, dest, tag, comm)
 * [info req ] = MPI_Issend (buf, dest, tag, comm)
 * [info req ] = MPI_Irsend (buf, dest, tag, comm)
 *
 * [info req ] = MPI_ Send_init (buf, dest, tag, comm)
 * [info req ] = MPI_Bsend_init (buf, dest, tag, comm)
 * [info req ] = MPI_Ssend_init (buf, dest, tag, comm)
 * [info req ] = MPI_Rsend_init (buf, dest, tag, comm)
 *
 * [info req ] = MPI_ Recv_init (buf, src,  tag, comm)
 *
 * [info stat] = MPI_Sendrecv        (sbuf, dest, stag, ...
 *                                    rbuf, src , rtag, comm)
 * [info stat] = MPI_Sendrecv_replace( buf, dest, stag, ...
 *                                          src , rtag, comm)
 * ------------------------------------------------------------------- */


#include "hBuff.h"		// type enums - declare vars - aux get_MPI_Buff


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 * aux basic patterns for more complex patterns. Think of
 *  info             = MPI_Send   (buf, dest, tag, comm)
 * [info flag stat ] = MPI_Iprobe       (src, tag, comm)
 * ------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * BLCK_B2IC: take BUF(SIZ/TYPE), src/dst, tag, comm
 * -------------------------------------------------------------------------
 * crossref: used twice from here
 * hRecv.h: (1 uses)	PATN_MAP_F_BUF_2INT_COM (Recv)		=>  MK_UNIQ
 * hSend.h: (2 uses)	PATN_F_B2IC        (all  Sends)		=> !MK_UNIQ
 * 			PATN_REQ_F_B2IC    (all Isends/_inits)	=>  MK_UNIQ
 * -------------------------------------------------------------------------
 */

#define BLCK_B2IC(PNAME,MK_UNIQ) /* B2IC->BUF, 2INT (srcdst,tag), COM */\
	NARGCHK     (PNAME,4)						\
									\
	BLCK_ONESYM (PNAME,0,var)	/* varnam, varsym */		\
	BLCK_ONEINT (PNAME,1,srcdst)	/* srcdst         */		\
	BLCK_ONEINT (PNAME,2,tag)	/* tag            */		\
	BLCK_ONECOM (PNAME,3)		/* comm           */		\
									\
	DECLARE_BUFF_VARS		/* BUF, SIZ, TYPE */		\
	BUF = get_MPI_Buff(varsym,SIZ,false,TYPE,MK_UNIQ);/*SIZ!=bytes*/\
    if(!BUF)				/* possibly MK_UNIQ for _Recv */\
	return octave_value(MPI_ERR_ARG);/*errmsg from get_MPI_Buff()*/

