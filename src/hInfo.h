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
 * Info Functions: MPI_Info_ [create/delete/dup/free/set/get]
 * 		   MPI_Info_get [valuelen/nkeys/nthkey]
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * [info inf]      = MPI_Info_create
 *  info           = MPI_Info_delete        (inf, key)
 * [info newinf]   = MPI_Info_dup           (inf)
 *  info           = MPI_Info_free          (inf)
 *  info           = MPI_Info_set           (inf, key, val)
 * [info flag val] = MPI_Info_get           (inf, key)
 * [info nkeys]    = MPI_Info_get_nkeys     (inf)
 * [info key]      = MPI_Info_get_nthkey    (inf,      n  )
 * [info len flag] = MPI_Info_get_valuelen  (inf, key)
 *
 * -------------------------------------------------------------------------
 *
 * [info clcomm] = MPI_Comm_accept    (port, inf,      root, comm)
 * [info svcomm] = MPI_Comm_connect   (port, inf,      root, comm)
 *  info         = MPI_Publish_name   (svc,  inf, port)
 *  info         = MPI_Unpublish_name (svc,  inf, port)
 * [info port  ] = MPI_Lookup_name    (svc,  inf)
 *
 * [info port  ] = MPI_Open_port            (inf)
 *  info         = MPI_Close_port     (port)
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * BLCK_ONEINF:
 * Take one info argument ARG Nth into source code (name inf always)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hInfo.h (12 uses):	all PATN_ except for MPI_Info_create/MPI_Close_port
 * -------------------------------------------------------------------------
 */
#define BLCK_ONEINF(PNAME,ARGN)						\
	if (  !	MPITB_isOPtrTyp(args(ARGN)) ){				\
	  error(STRFY(PNAME) ": required arg#%d: inf(ptr)", ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Info inf = MPITB_ptrcast ( MPI_Info, args(ARGN) );



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
 * [info inf]      = MPI_Info_create
 * ------------------------------------------------------------------- */

#define PATN_INF_F_VOID(PNAME)						\
{									\
	NARGCHK   (PNAME,0)						\
	MPI_Info          inf=0;					\
	int info = PNAME(&inf );					\
	RET_2_ARG (info, MPITB_intcast(					\
			 inf )	      )					\
}


/* -------------------------------------------------------------------
 *  info           = MPI_Info_delete       (inf, key)
 * ------------------------------------------------------------------- */

#define PATN_F_INF_STR(PNAME)						\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONEINF(PNAME,0)	/* inf */				\
	BLCK_ONESTR(PNAME,1,key)/* key */				\
	int  info = PNAME ( inf, const_cast<char*>(			\
			    key.c_str() ));				\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * [info newinf]   = MPI_Info_dup          (inf)
 * ------------------------------------------------------------------- */

#define PATN_INF_F_INF(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONEINF   (PNAME,0)						\
	MPI_Info		 inf0=0;				\
	int info = PNAME (inf,  &inf0 );				\
	RET_2_ARG (info, MPITB_intcast(					\
				 inf0 ) )				\
}


/* -------------------------------------------------------------------
 *  info           = MPI_Info_free         (inf)
 * ------------------------------------------------------------------- */

#define PATN_F_INF( PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONESYM(PNAME,0, inf)	/* infnam, infsym */		\
	BLCK_ONEINF(PNAME,0)		/* copy/in inf */		\
									\
	int info  = PNAME  (&inf);					\
	infsym.varref() = octave_value( MPITB_intcast(				\
			    inf) );	/* copy/out inf */		\
	RET_1_ARG  (info)						\
}
/* -------------------------------------------------------------------
 *  same happened in hTstWait.h: reqs should be passed by ref,
 *  but chosen internal representation under Octave is int->double:
 *  must copy in/out (infos, in this case)
 * ------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 *  info           = MPI_Info_set          (inf, key, val)
 * ------------------------------------------------------------------- */

#define PATN_F_INF_STR_STR(PNAME)					\
{									\
	NARGCHK    (PNAME,3)						\
	BLCK_ONEINF(PNAME,0)		/* inf */			\
	BLCK_ONESTR(PNAME,1,key)	/* key */			\
	BLCK_ONESTR(PNAME,2,val)	/* val */			\
  									\
	int  info = PNAME ( inf,					\
	const_cast<char*>( key.c_str() ),				\
	const_cast<char*>( val.c_str() ));				\
  									\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * [info flag val] = MPI_Info_get          (inf, key)
 * from mpi.h:
 * #define MPI_MAX_INFO_VAL   256          / * max info value length * /
 * ------------------------------------------------------------------- */

#define PATN_INT_STR_F_INF_STR(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONEINF(PNAME,0)						\
	BLCK_ONESTR(PNAME,1,key)					\
									\
	char val   [MPI_MAX_INFO_VAL];					\
   for (int  i=0; i<MPI_MAX_INFO_VAL; i++)				\
	     val[i]=0;							\
	int  flag  =0,							\
	     info = PNAME (inf,						\
	const_cast<char*>( key.c_str() ),				\
	MPI_MAX_INFO_VAL,  val,						\
			  &flag);					\
	RET_3_ARG  (info,  flag, val)	/* val -> octave_string */	\
}


/* -------------------------------------------------------------------
 * [info nkeys]    = MPI_Info_get_nkeys    (inf)
 * ------------------------------------------------------------------- */

#define PATN_INT_F_INF(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONEINF   (PNAME,0)						\
	int nkeys=0,							\
		  info=PNAME(inf,&nkeys);				\
	RET_2_ARG(info,           nkeys)				\
}


/* -------------------------------------------------------------------
 * [info key]      = MPI_Info_get_nthkey   (inf,      n  )
 * from mpi.h:
 * #define MPI_MAX_INFO_KEY        36        / * max info key length * /
 * ------------------------------------------------------------------- */

#define PATN_STR_F_INF_INT(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONEINF(PNAME,0)	/* inf */				\
	BLCK_ONEINT(PNAME,1,n)	/*  n  */				\
									\
	char key   [MPI_MAX_INFO_KEY];					\
   for (int  i=0; i<MPI_MAX_INFO_KEY; i++)				\
	     key[i]=0;							\
									\
	int  info = PNAME(inf,n,key);					\
									\
	RET_2_ARG  (info,       key)					\
}


/* -------------------------------------------------------------------
 * [info len flag] = MPI_Info_get_valuelen (inf, key)
 * ------------------------------------------------------------------- */

#define PATN_2INT_F_INF_STR(PNAME)					\
{					/* BUG: MAX_INFO_VAL? */	\
	NARGCHK    (PNAME,2)						\
	BLCK_ONEINF(PNAME,0)		/* inf */			\
	BLCK_ONESTR(PNAME,1,  key)	/* key */			\
	int  len=0, flag=0,						\
	     info = PNAME  ( inf,					\
	 const_cast<char *>( key.c_str() ),				\
			    &len,    					\
			    &flag);					\
	RET_3_ARG  (info,len,flag)					\
}



/* -------------------------------------------------------------------------
 * -------- Call Patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 * [info clcomm] = MPI_Comm_accept    (port, inf,      root, comm)
 * [info svcomm] = MPI_Comm_connect   (port, inf,      root, comm)
 * ------------------------------------------------------------------- */

#define PATN_COM_F_PIRC(PNAME)	/* PIRC -> Port, Inf, Root, Comm */	\
{									\
	NARGCHK    (PNAME,4)						\
	BLCK_ONESTR(PNAME,0,port)/* port */				\
	BLCK_ONEINF(PNAME,1)	 /* inf  */				\
	BLCK_ONEINT(PNAME,2,root)/* root */	/*ROOTCOM_GET hColl */	\
	BLCK_ONECOM(PNAME,3)	 /* comm */				\
									\
	MPI_Comm				clcomm=0;		\
	int  info = PNAME  (						\
	 const_cast<char *>( port.c_str() ),				\
			     inf, root, comm,  &clcomm );		\
	RET_2_ARG  (info,	MPITB_intcast ( clcomm ))		\
}


/* -------------------------------------------------------------------
 *  info         = MPI_Publish_name   (svc,  inf, port)
 *  info         = MPI_Unpublish_name (svc,  inf, port)
 * ------------------------------------------------------------------- */

#define PATN_F_SIP(PNAME)	/* SIP -> Svc, Inf, Port */		\
{									\
	NARGCHK    (PNAME,3)						\
	BLCK_ONESTR(PNAME,0, svc)	/* svc  */			\
	BLCK_ONEINF(PNAME,1)		/* inf  */			\
	BLCK_ONESTR(PNAME,2, port)	/* port */			\
									\
	int  info = PNAME  (						\
	 const_cast<char *>( svc.c_str() ),				\
			     inf,					\
	 const_cast<char *>( port.c_str() ) );				\
	RET_1_ARG  (info)						\
}


/* -------------------------------------------------------------------
 * [info port  ] = MPI_Lookup_name    (svc,  inf)
 * from mpi.h:
 * #define MPI_MAX_NAME_LEN  MPI_MAX_PORT_NAME / * max port name length * /
 * #define MPI_MAX_PORT_NAME 36               / * max port name length * /
 * ------------------------------------------------------------------- */

#define PATN_STR_F_STR_INF(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONESTR(PNAME,0,svc)/* svc  */				\
	BLCK_ONEINF(PNAME,1)	/* inf  */				\
									\
	char port  [MPI_MAX_NAME_LEN];					\
   for (int  i=0; i<MPI_MAX_NAME_LEN; i++)				\
	     port[i]=0;							\
									\
	int  info = PNAME  (						\
	 const_cast<char *>( svc.c_str() ),				\
			     inf, port);				\
	RET_2_ARG  (info,	  port)	/* port -> octave_string */	\
}


/* -------------------------------------------------------------------
 * [info port  ] = MPI_Open_port           (inf)
 * ------------------------------------------------------------------- */

#define PATN_STR_F_INF(PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONEINF(PNAME,0)	/* inf  */				\
									\
	char port  [MPI_MAX_NAME_LEN];					\
   for (int  i=0; i<MPI_MAX_NAME_LEN; i++)				\
	     port[i]=0;							\
									\
	int  info = PNAME(inf, port);					\
									\
	RET_2_ARG  (info,      port)	/* port -> octave_string */	\
}


/* -------------------------------------------------------------------
 *  info         = MPI_Close_port     (port)
 * ------------------------------------------------------------------- */

#define PATN_F_STR( PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONESTR(PNAME,0,port)/* port  */				\
									\
	int  info = PNAME(						\
	const_cast<char*>( port.c_str() ) );				\
									\
	RET_1_ARG  (info)						\
}

