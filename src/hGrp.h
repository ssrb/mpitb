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
 * Group functions, Communicator functions: MPI_Grp_*, MPI_Comm_*
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *  info         = MPI_Comm_free         (comm)	// changes comm -> NULL
 *  info         = MPI_Comm_disconnect   (comm)	// changes comm
 *
 * [info grp ]   = MPI_Comm_group        (comm)
 * [info grp ]   = MPI_Comm_remote_group (comm)
 *
 * [info comm2]  = MPI_Comm_dup          (comm1)
 * [info comm2]  = MPI_Comm_create       (comm1, grp)
 *
 * [info result] = MPI_Comm_compare      (comm1, comm2)
 * [info comm  ] = MPI_Intercomm_create(l_comm,l_leader, r_comm,r_leader, tag)
 * [info comm2 ] = MPI_Comm_split        (comm1, color,  key)
 *
 * -------------------------------------------------------------------------
 *
 *  info         = MPI_Group_free        (grp)	// changes grp -> NULL
 *
 * [info rank]   = MPI_Group_rank        (grp)
 * [info size]   = MPI_Group_size        (grp)
 *
 * [info result] = MPI_Group_compare     (grp1, grp2)
 *
 * [info grp ]   = MPI_Group_union       (grp1, grp2)
 * [info grp ]   = MPI_Group_difference  (grp1, grp2)
 * [info grp ]   = MPI_Group_intersection(grp1, grp2)
 *
 * [info grp2]   = MPI_Group_incl        (grp, ranks)
 * [info grp2]   = MPI_Group_excl        (grp, ranks)
 *
 * [info grp2]   = MPI_Group_range_incl  (grp, ranges)
 * [info grp2]   = MPI_Group_range_excl  (grp, ranges)
 *
 * [info ranks2] = MPI_Group_translate_ranks (grp1, ranks1, grp2)
 * ------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * BLCK_ONEGRP:
 * Take one group argument ARG Nth into source code (name grp always)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (5 uses)	PATN_COM_F_COM_GRP	PATN_F_GRP
 * 			PATN_INT_F_GRP		PATN_GRP_F_GRP_RNK
 *			PATN_GRP_F_GRP_RNG
 * -------------------------------------------------------------------------
 */
#define BLCK_ONEGRP(PNAME,ARGN)		/* progNAME, Nth ARG in list  */\
/*	if ( ! args(ARGN).is_real_scalar() ){	*/			\
	if ( ! MPITB_isOPtrTyp(args(ARGN)) ){				\
	  error(STRFY(PNAME) ": required arg#%d: grp(ptr)", ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Group grp = MPITB_ptrcast ( MPI_Group, args(ARGN) );


/* -------------------------------------------------------------------------
 * BLCK_TWOGRP:
 * Take two group arguments ARG Nth-Mth into source code (names grp1/2 always)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (3 uses)	PATN_INT_F_2GRP		PATN_GRP_F_2GRP
 * 			PATN_GRP_F_G_RNK_G (not together ARG1-3)
 * -------------------------------------------------------------------------
 */
#define BLCK_TWOGRP(PNAME,ARGM,ARGN)		/*N & M-th ARG in list*/\
/*	if ((! args(ARGM).is_real_scalar() )	*/			\
	if ((! MPITB_isOPtrTyp(args(ARGM)) )				\
	  ||(! MPITB_isOPtrTyp(args(ARGN)))){				\
	  error(							\
	    STRFY(PNAME) ": required args#%d-%d: grp1(ptr), grp2(ptr)",	\
							ARGM,ARGN);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Group grp1 = MPITB_ptrcast(MPI_Group, args(ARGM) ),		\
		  grp2 = MPITB_ptrcast(MPI_Group, args(ARGN) );


/* -------------------------------------------------------------------------
 * BLCK_TWOCOM:
 * Take two comm arguments ARG Nth-Mth into source code (names comm1/2 always)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (1 uses)	PATN_INT_F_2COM (MPI_Comm_compare)
 * -------------------------------------------------------------------------
 */
#define BLCK_TWOCOM(PNAME,ARGM,ARGN)		/*N & M-th ARG in list*/\
/*	if ((! args(ARGM).is_real_scalar() )	*/			\
	if ((! MPITB_isOPtrTyp(args(ARGM)) )				\
	  ||(! MPITB_isOPtrTyp(args(ARGN)))){				\
	  error(							\
	   STRFY(PNAME) ": required args#%d-%d: comm1(ptr), comm2(ptr)",\
							ARGM,ARGN);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Comm comm1 = MPITB_ptrcast(MPI_Comm, args(ARGM) ),		\
		 comm2 = MPITB_ptrcast(MPI_Comm, args(ARGN) );


/* -------------------------------------------------------------------------
 * BLCK_COMLDR:
 * Take comm-int arguments ARG Nth & N+1th into source code (P_comm/P_lead)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (2 uses)	PATN_COM_F_2COMLDR_TAG
 *        (1 func)	both uses in MPI_Intercomm_create
 * -------------------------------------------------------------------------
 */
#define BLCK_COMLDR(PNAME,ARGN,P)		/* P for PREFIX */	\
	if ((! MPITB_isOPtrTyp(args(ARGN  )) )				\
	  ||(! args(ARGN+1).is_real_scalar())){				\
	  error(STRFY(PNAME)	": required args#%d-%d: "		\
		STRFY(P)	"_comm(ptr), "				\
		STRFY(P)	"_leader(int)",		ARGN+1,ARGN+2);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	MPI_Comm P##_comm = MPITB_ptrcast(MPI_Comm, args(ARGN) );	\
	   int   P##_lead =			    args(ARGN+1).int_value();
//	   int   P##_lead = reinterpret_cast<int> ( args(ARGN+1).int_value() );
// BUG: BRAINDAMAGE!! casting int to int, compile error w/gcc4.0.2 Ubuntu
// MPI_Intercomm_create.cc:49: error: invalid cast from type 'int' to type 'int'
// CREDIT: detected by Michael Creel, Econometrics UAB Spain


/* -------------------------------------------------------------------------
 * BLCK_GETRNKS:
 * Take int-row-vector ARG Nth into source code (chosen vector name VN)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (2 uses)	PATN_GRP_F_GRP_RNK	PATN_GRP_F_G_RNK_G
 * -------------------------------------------------------------------------
 */				/* this could be hint for BLCK_VECINT?*/
#define BLCK_GETRNKS(PNAME,ARGN,VN)		/* NAME of the vector */\
	if ((! args (ARGN).is_real_matrix() )				\
	 && (! args (ARGN).is_real_scalar())){				\
	  error(							\
	    STRFY(PNAME) ": required arg#%d: ranks(intvec)",	ARGN+1);\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
  const Matrix&om = args(ARGN).matrix_value();				\
	int	n = om.cols(),						\
		m = om.rows();						\
	if    ( m!= 1 ){						\
	  error(							\
	    STRFY(PNAME) ": required arg#%d: ranks(int row vector)",	\
								ARGN+1);\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
									\
	int VN[n];							\
	for   (int i=0; i<n; i++)					\
	    VN[i] = static_cast<int>( om(0,i) );/* works for scalar!!! */


/* -------------------------------------------------------------------------
 * BLCK_GETRNGS:
 * Take int-3row-vector ARG Nth into source code (chosen vector name VN)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hGrp.h (1 uses)	PATN_GRP_F_GRP_RNG
 * -------------------------------------------------------------------------
 */
#define BLCK_GETRNGS(PNAME,ARGN,VN)		/* NAME of the vector */\
	if ( ! args (ARGN).is_real_matrix() ){				\
	  error(							\
	    STRFY(PNAME) ": required arg#%d: ranges(int matrix)",	\
								ARGN+1);\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
  const Matrix&om = args(ARGN).matrix_value();				\
	int	n = om.cols(),						\
		m = om.rows();						\
	if    ( n!= 3 ){						\
	  error(							\
	    STRFY(PNAME) ": required arg#%d: ranges(int matrix nx3)",	\
								ARGN+1);\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
									\
	int VN[n][3];							\
	for   (int i=0; i<m; i++)					\
	  for (int j=0; j<n; j++)					\
	    VN[i][j] = static_cast<int>( om(i,j) );



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
 *  info         = MPI_Comm_free         (comm)	// changes comm -> NULL
 *  info         = MPI_Comm_disconnect   (comm)	// changes comm
 * ------------------------------------------------------------------- */

#define PATN_F_COMREF(PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONESYM(PNAME,0,  comm)	/* commnam, commsym */		\
	BLCK_ONECOM(PNAME,0)		/* copy/in  comm */		\
									\
	int info =  PNAME   (&comm);					\
	commsym ->define(MPITB_intcast(					\
			      comm )  );/* copy/out */			\
	RET_1_ARG  (info)						\
}
/* -------------------------------------------------------------------
 *  same happened in hTstWait.h: reqs should be passed by ref,
 *  but chosen internal representation under Octave is int->double:
 *  must copy in/out (comms, in this case)
 * ------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 * [info grp ]   = MPI_Comm_group        (comm)
 * [info grp ]   = MPI_Comm_remote_group (comm)
 * ------------------------------------------------------------------- */

#define PATN_GRP_F_COM(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONECOM   (PNAME,0)		/* comm */			\
	MPI_Group		grp0=0;					\
	int info= PNAME(comm,  &grp0 );					\
	RET_2_ARG(info,MPITB_intcast(					\
				grp0 ))					\
}


/* -------------------------------------------------------------------
 * [info comm2]  = MPI_Comm_dup          (comm1)
 * ------------------------------------------------------------------- */

#define PATN_COM_F_COM(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONECOM   (PNAME,0)		/* comm */			\
	MPI_Comm		comm0=0;				\
	int info= PNAME(comm,  &comm0 );				\
	RET_2_ARG(info,MPITB_intcast(					\
				comm0 ))				\
}


/* -------------------------------------------------------------------
 * [info comm2]  = MPI_Comm_create       (comm1, grp)
 * ------------------------------------------------------------------- */

#define PATN_COM_F_COM_GRP(PNAME)					\
{									\
	NARGCHK       (PNAME,2)						\
	BLCK_ONECOM   (PNAME,0)		/* comm */			\
	BLCK_ONEGRP   (PNAME,1)		/* grp  */			\
	MPI_Comm		   comm0=0;				\
	int info= PNAME(comm, grp,&comm0 );				\
	RET_2_ARG(info,MPITB_intcast(					\
				   comm0 ))				\
}


/* -------------------------------------------------------------------
 * [info result] = MPI_Comm_compare      (comm1, comm2)
 * ------------------------------------------------------------------- */

#define PATN_INT_F_2COM(PNAME)						\
{									\
	NARGCHK       (PNAME,2)						\
	BLCK_TWOCOM   (PNAME,0,1)	/* comm1, comm2 */		\
    int int0=0, info=PNAME (comm1, comm2,&int0);			\
	RET_2_ARG(info,			  int0)				\
}


/* -------------------------------------------------------------------
 * [info comm  ] = MPI_Intercomm_create(l_comm,l_leader, r_comm,r_leader, tag)
 * ------------------------------------------------------------------- */

#define PATN_COM_F_2COMLDR_TAG(PNAME)					\
{									\
	NARGCHK       (PNAME,5)						\
	BLCK_COMLDR   (PNAME,0,l)	/* l_comm, l_lead */		\
	BLCK_COMLDR   (PNAME,2,r)	/* r_comm, r_lead */		\
	BLCK_ONEINT   (PNAME,4,tag)	/* tag */			\
	MPI_Comm				 comm0=0;		\
	int       info=PNAME(l_comm,l_lead,				\
			     r_comm,r_lead, tag,&comm0 );		\
	RET_2_ARG(info, MPITB_intcast          ( comm0 ))		\
}


/* -------------------------------------------------------------------
 * [info comm2 ] = MPI_Comm_split        (comm1, color,  key)
 * ------------------------------------------------------------------- */

#define PATN_COM_F_COM_2INT(PNAME)					\
{									\
	NARGCHK       (PNAME,3)						\
	BLCK_ONECOM   (PNAME,0)		/* comm  */			\
	BLCK_ONEINT   (PNAME,1,color)	/* color */			\
	BLCK_ONEINT   (PNAME,2,key  )	/* key   */			\
	MPI_Comm			      comm0=0;			\
	int       info=PNAME(comm,color,key, &comm0 );			\
	RET_2_ARG(info,MPITB_intcast        ( comm0 ))			\
}



/* -------------------------------------------------------------------------
 * -------- Call Patterns --------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------
 *  info         = MPI_Group_free        (grp)
 * ------------------------------------------------------------------- */

#define PATN_F_GRP( PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONESYM(PNAME,0, grp)	/* grpnam, grpsym */		\
	BLCK_ONEGRP(PNAME,0)		/* copy/in grp    */		\
									\
	int info =  PNAME  (&grp);					\
	grpsym  ->define(MPITB_intcast(					\
			     grp     ));/* copy/out grp */		\
	RET_1_ARG  (info)						\
}
/* -------------------------------------------------------------------
 *  same happened in hTstWait.h: reqs should be passed by ref,
 *  but chosen internal representation under Octave is int->double:
 *  must copy in/out (groups, in this case)
 * ------------------------------------------------------------------- */


/* -------------------------------------------------------------------
 * [info rank]   = MPI_Group_rank        (grp)
 * [info size]   = MPI_Group_size        (grp)
 * ------------------------------------------------------------------- */

#define PATN_INT_F_GRP(PNAME)						\
{									\
	NARGCHK       (PNAME,1)						\
	BLCK_ONEGRP   (PNAME,0)		/* grp */			\
    int int0=0,   info=PNAME (grp,&int0);				\
	RET_2_ARG(info,		   int0);				\
}


/* -------------------------------------------------------------------
 * [info result] = MPI_Group_compare     (grp1, grp2)
 * ------------------------------------------------------------------- */

#define PATN_INT_F_2GRP(PNAME)						\
{									\
	NARGCHK        (PNAME,2)					\
	BLCK_TWOGRP    (PNAME,0,1)	/* grp1, grp2 */		\
    int int0=0,   info= PNAME (grp1, grp2,&int0);			\
	RET_2_ARG(info,			   int0);			\
}


/* -------------------------------------------------------------------
 * [info grp ]   = MPI_Group_union           (grp1, grp2)
 * [info grp ]   = MPI_Group_difference      (grp1, grp2)
 * [info grp ]   = MPI_Group_intersection    (grp1, grp2)
 * ------------------------------------------------------------------- */

#define PATN_GRP_F_2GRP(PNAME)						\
{									\
	NARGCHK       (PNAME,2)						\
	BLCK_TWOGRP   (PNAME,0,1)	/* grp1, grp2 */		\
	MPI_Group			      grp0=0;			\
	int       info=PNAME(grp1, grp2,     &grp0 );			\
	RET_2_ARG(info,MPITB_intcast	    ( grp0 ))			\
}


/* -------------------------------------------------------------------
 * [info grp2]   = MPI_Group_incl        (grp, ranks)
 * [info grp2]   = MPI_Group_excl        (grp, ranks)
 * ------------------------------------------------------------------- */

#define PATN_GRP_F_GRP_RNK(PNAME)					\
{									\
	NARGCHK          (PNAME,2)					\
	BLCK_ONEGRP      (PNAME,0)	/* grp       */			\
	BLCK_GETRNKS     (PNAME,1,rnks)	/* rnks,n,om */			\
	MPI_Group			       grp0=0;			\
	int info = PNAME  (grp, n,rnks,	      &grp0 );			\
	RET_2_ARG (info,MPITB_intcast	     ( grp0 ))			\
}


/* -------------------------------------------------------------------
 * [info grp2]   = MPI_Group_range_incl  (grp, ranges)
 * [info grp2]   = MPI_Group_range_excl  (grp, ranges)
 * ------------------------------------------------------------------- */

#define PATN_GRP_F_GRP_RNG(PNAME)					\
{									\
	NARGCHK          (PNAME,2)					\
	BLCK_ONEGRP      (PNAME,0)	/* grp       */			\
	BLCK_GETRNGS     (PNAME,1,rngs)	/* rngs,m,om */			\
	MPI_Group			       grp0=0;			\
	int info = PNAME  (grp, m,rngs,	      &grp0 );			\
	RET_2_ARG (info,MPITB_intcast	     ( grp0 ))			\
}


/* -------------------------------------------------------------------
 * [info ranks2] = MPI_Group_translate_ranks (grp1, ranks1, grp2)
 * ------------------------------------------------------------------- */

#define PATN_GRP_F_G_RNK_G(PNAME)					\
{									\
	NARGCHK       (PNAME,3)						\
	BLCK_TWOGRP   (PNAME,0,2)	/* grp1, grp2 */		\
	BLCK_GETRNKS  (PNAME,1,rnks1)	/* rnks1,n,om */		\
									\
	int		       rnks2[n],				\
	  info = PNAME (grp1,n,rnks1,					\
		        grp2,  rnks2);					\
									\
	Matrix ranks2 (1,n,0);						\
	for(int i=0; i<n; i++)ranks2(0,i) =				\
			      rnks2   [i] ;	/* promo int->double */	\
									\
	RET_2_ARG     (info,  ranks2)					\
}

