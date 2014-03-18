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
 * Point-to-point functions: MPI_ [I][-bsr]Send
 * Persistent req functions: MPI_ [I][-bsr]Send/Recv _init
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------
 * info        = MPI_ Send  (buf, dest, tag, comm)
 * info        = MPI_Bsend  (buf, dest, tag, comm)
 * info        = MPI_Ssend  (buf, dest, tag, comm)
 * info        = MPI_Rsend  (buf, dest, tag, comm)
 *
 * [info req ] = MPI_I send (buf, dest, tag, comm)
 * [info req ] = MPI_Ibsend (buf, dest, tag, comm)
 * [info req ] = MPI_Issend (buf, dest, tag, comm)
 * [info req ] = MPI_Irsend (buf, dest, tag, comm)
 *
 * [info req ] = MPI_Irecv  (buf, src,  tag, comm)
 *
 * [info req ] = MPI_ Send_init (buf, dest, tag, comm)
 * [info req ] = MPI_Bsend_init (buf, dest, tag, comm)
 * [info req ] = MPI_Ssend_init (buf, dest, tag, comm)
 * [info req ] = MPI_Rsend_init (buf, dest, tag, comm)
 *
 * [info req ] = MPI_ Recv_init (buf, src,  tag, comm)
 * ------------------------------------------------------------------- */

#include "hSndRecv.h"		// BLCK_B2IC


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
 * pattern for sends: int = f (sym, int * 2, comm)
 *
 * info        = MPI_ Send  (buf, dest, tag, comm)
 * info        = MPI_Bsend  (buf, dest, tag, comm)
 * info        = MPI_Ssend  (buf, dest, tag, comm)
 * info        = MPI_Rsend  (buf, dest, tag, comm)
 * ------------------------------------------------------------------- */
#define PATN_F_B2IC(PNAME)						\
{				/* ! MK_UNIQ */				\
	BLCK_B2IC  (PNAME,0)						\
	int info =  PNAME (BUF,SIZ,TYPE, srcdst,tag,comm);		\
	RET_1_ARG  (info);						\
}



/* -------------------------------------------------------------------
 * pattern for immediates/inits: [int req] = f (sym, int * 2, comm)
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
 * ------------------------------------------------------------------- */

#define PATN_REQ_F_B2IC(PNAME)	/* req=>better not to play w/lazycopy */\
{				/* _Recv_init => MK_UNIQ for all */	\
	BLCK_B2IC (PNAME,1)	/* BUF,SIZ,TYPE, srcdst,tag,comm */	\
									\
	MPI_Request	 req=0;						\
	int info = PNAME (BUF,SIZ,TYPE, srcdst,tag,comm,		\
			&req);						\
									\
	RET_2_ARG (info, MPITB_intcast(					\
			 req )	       );				\
}

