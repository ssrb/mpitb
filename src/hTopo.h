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
 * Topology Functions:	MPI_ [Dims/Cart/Graph] _create, MPI_Topo_test
 * 			MPI_Graph [dims_get/_get/_neighbors[_count]]
 * 			MPI_Cart  [dim_get/_get/_rank/_coords/_shift/_sub]
 * 			MPI_ [Cart/Graph] _map
 * ----------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * [info dims]          = MPI_Dims_create   (nnodes, ndims)
 * [info comm_cart ]    = MPI_Cart_create   (comm,    dims, periods, reorder)
 * [info comm_graph]    = MPI_Graph_create  (comm,   index, edges,   reorder)
 * [info topo]          = MPI_Topo_test     (comm)		// not here!!
 *
 * [info nnodes nedges] = MPI_Graphdims_get (comm)
 * [info index  edges ] = MPI_Graph_get     (comm)
 * [info nneighs]       = MPI_Graph_neighbors_count (comm, rank)
 * [info  neighs]       = MPI_Graph_neighbors       (comm, rank)
 *
 * [info ndims ]        = MPI_Cartdim_get   (comm)		// not here!!
 * [info dims periods coords] = MPI_Cart_get(comm)
 * [info rank  ]        = MPI_Cart_rank     (comm, coords)
 * [info coords]        = MPI_Cart_coords   (comm, rank  )
 * [info rsrc rdst]     = MPI_Cart_shift    (comm, dir,   disp)
 * [info comm_sub ]     = MPI_Cart_sub      (comm, dims  )
 *
 * [info nrank]         = MPI_Cart_map      (comm, dims, periods)
 * [info nrank]         = MPI_Graph_map     (comm, index, edges)
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
 * -------- Building Blocks ------------------------------------------------
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * BLCK_INTVEC: Take one int vector ARG Nth into source code (chosen name)
 * -------------------------------------------------------------------------
 * crossref: used just from here
 * hTopo.h: (10 uses)	PATN_COM_F_COM_2INTVEC_INT		(2 uses)
 *			PATN_COM_F_COM_INTVEC_INTVEC_INT	(2 uses)
 *			PATN_INT_F_COM_INTVEC			(1 uses)
 *			PATN_COM_F_COM_INTVEC			(1 uses)
 *			PATN_INT_F_COM_2INTVEC			(2 uses)
 *			PATN_INT_F_COM_INTVEC_INTVEC		(2 uses)
 * -------------------------------------------------------------------------
 */
#define BLCK_INTVEC(PNAME,ARGN,VN)		/* VN is Vector Name */	\
	if ( ! args(ARGN).is_real_matrix()){				\
	  error(STRFY(PNAME)	": required arg#%d: "			\
		STRFY(VN)	"(intvec)",		ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	if ( ! (args(ARGN).rows()   ==1)				\
	  && ! (args(ARGN).columns()==1)){				\
	  error(STRFY(PNAME)	": required arg#%d: "			\
		STRFY(VN)	"(row/col vec)",	ARGN+1);	\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
	double*p##VN = const_cast<double*>(				\
			args(ARGN).matrix_value().data() );		\
	int    n##VN =	args(ARGN).matrix_value().capacity(),		\
	          VN   [n##VN];						\
	for (int i=0; i<n##VN ; i++)					\
		  VN[i]=static_cast<int>(				\
			p##VN[i]	);


/* -------------------------------------------------------------------------
 * intVEC_PUT: translate C-int vector to Octave-row vector
 * -------------------------------------------------------------------------
 * crossref: used just once from here   IDENTICAL COPY @ hTstWait (-> mpitb.h ?)
 * hTopo.h: (8 uses)	PATN_INTVEC_F_2INT		(1 uses)
 *			PATN_2INTVEC_F_COM		(2 uses)
 *			PATN_INTVEC_NEIGH_F_COM_INT	(1 uses)
 *			PATN_3INTVEC_F_COM		(3 uses)
 *			PATN_INTVEC_COORD_F_COM_INT	(1 uses)
 * -------------------------------------------------------------------------
 */
#define intVEC_PUT(CNAM,SZ,OCTN) /* C-vector & Oct-matrix name & size */\
	Matrix OCTN  (1,SZ,0   );					\
	for (int i=0; i<SZ; i++)					\
	       OCTN(0,i) = CNAM[i];		/* promo int->double */



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

/* -------------------------------------------------------------------------
 * [info dims]          = MPI_Dims_create   (nnodes, ndims)
 * ------------------------------------------------------------------------- */
                                                                                
#define PATN_INTVEC_F_2INT(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONEINT(PNAME,0,nnodes)		/* get args */		\
	BLCK_ONEINT(PNAME,1,ndims )					\
									\
	int dims  [ndims];						\
    for(int i=0; i<ndims; i++) dims[i]=0;	/* SIGSEGV if not init 0 */\
									\
	int info  = PNAME(nnodes,ndims,dims);	/* dims filled */	\
									\
	intVEC_PUT (dims,ndims, octdims)	/* octdims */		\
	RET_2_ARG  (info,       octdims)				\
}


/* -------------------------------------------------------------------------
 * [info comm_cart ]    = MPI_Cart_create   (comm,    dims, periods, reorder)
 * ------------------------------------------------------------------------- */

#define PATN_COM_F_COM_2INTVEC_INT(PNAME)				\
{									\
	NARGCHK    (PNAME,4)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,dims)	/* dims,   ndims    */		\
	BLCK_INTVEC(PNAME,2,periods)	/* periods,nperiods */		\
	BLCK_ONEINT(PNAME,3,reorder)					\
									\
	if (ndims != nperiods){						\
	  error(STRFY(PNAME) ": required args#2-3"			\
		" (dims-periods) (intvec) of same size");		\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
									\
	MPI_Comm	   comm_cart=0;					\
	int info =  PNAME (comm,	ndims,dims,periods,reorder,	\
			  &comm_cart );					\
									\
	RET_2_ARG  (info,  MPITB_intcast(				\
			   comm_cart )	)				\
}


/* -------------------------------------------------------------------------
 * [info comm_graph]    = MPI_Graph_create  (comm,   index, edges,   reorder)
 * ------------------------------------------------------------------------- */

#define PATN_COM_F_COM_INTVEC_INTVEC_INT(PNAME)				\
{									\
	NARGCHK    (PNAME,4)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,index)	/* index, nindex */		\
	BLCK_INTVEC(PNAME,2,edges)	/* edges, nedges */		\
	BLCK_ONEINT(PNAME,3,reorder)					\
									\
	MPI_Comm	   comm_graph=0;				\
	int info =  PNAME (comm,nindex,index,edges,reorder,		\
			  &comm_graph );				\
									\
	RET_2_ARG  (info,  MPITB_intcast(				\
			   comm_graph )	)				\
}


/* -------------------------------------------------------------------------
 * [info topo] = MPI_Topo_test (comm)
 * PATN_INT_F_COM already defined in mpitb.h
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * [info nnodes nedges] = MPI_Graphdims_get (comm)
 * ------------------------------------------------------------------------- */

#define PATN_2INT_F_COM(PNAME)						\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONECOM(PNAME,0)						\
	int int1=0, int2=0,						\
	    info =  PNAME (comm, &int1, &int2);				\
	RET_3_ARG  (info,         int1,  int2)				\
}


/* -------------------------------------------------------------------------
 * [info index  edges ] = MPI_Graph_get     (comm)
 * ------------------------------------------------------------------------- */

#define PATN_2INTVEC_F_COM(PNAME)					\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONECOM(PNAME,0)						\
									\
	int nindex=0, nedges=0,						\
	    info  =MPI_Graphdims_get (comm, &nindex, &nedges);		\
	if (info){			/* scope-out retval decl */	\
	  RET_3_ARG(info, Matrix(), Matrix())				\
	}								\
									\
	int index[nindex],						\
	    edges[nedges];						\
	    info =  PNAME (comm, nindex,nedges, index,edges);		\
									\
	intVEC_PUT(index,nindex,octidx)					\
	intVEC_PUT(edges,nedges,octedg)					\
	RET_3_ARG  (info,octidx,octedg)		/*retval decl*/		\
}


/* -------------------------------------------------------------------------
 * [info nneighs]       = MPI_Graph_neighbors_count (comm, rank)
 * ------------------------------------------------------------------------- */

#define PATN_INT_F_COM_INT(PNAME)					\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)		/* comm */			\
	BLCK_ONEINT(PNAME,1,rank)					\
									\
	int nneighs=0,							\
	    info   =PNAME ( comm, rank, &nneighs );			\
									\
	RET_2_ARG  (info,                nneighs)			\
}


/* -------------------------------------------------------------------------
 * [info  neighs]       = MPI_Graph_neighbors       (comm, rank)
 * [info  coords]       = MPI_Cart_coords           (comm, rank) // below
 * ------------------------------------------------------------------------- */

#define PATN_INTVEC_NEIGH_F_COM_INT(PNAME)				\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)		/* comm */			\
	BLCK_ONEINT(PNAME,1,rank)					\
									\
	int nneighs=0,							\
	    info   =MPI_Graph_neighbors_count (comm, rank, &nneighs);	\
	if (info){			/* scope-out retval decl */	\
	  RET_2_ARG(info, Matrix())					\
	}								\
									\
	int neighs [nneighs];						\
	    info =  PNAME (comm, rank,					\
		    nneighs,neighs);					\
									\
	intVEC_PUT (neighs,nneighs,octngh)				\
	RET_2_ARG  (info,          octngh)	/*retval decl*/		\
}


/* -------------------------------------------------------------------------
 * [info ndims ]        = MPI_Cartdim_get   (comm)
 * PATN_INT_F_COM already defined in hGrp.h
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * [info dims periods coords] = MPI_Cart_get(comm)
 * ------------------------------------------------------------------------- */

#define PATN_3INTVEC_F_COM(PNAME)					\
{									\
	NARGCHK    (PNAME,1)						\
	BLCK_ONECOM(PNAME,0)						\
									\
	int ndims  =0,							\
	    info   =MPI_Cartdim_get (comm, &ndims);			\
	if (info){			/* scope-out retval decl */	\
	  RET_4_ARG(info, Matrix(), Matrix(), Matrix())			\
	}								\
									\
	int dims   [ndims],						\
	    periods[ndims],						\
	    coords [ndims];						\
	    info =  PNAME (comm, ndims, dims,periods,coords);		\
									\
	intVEC_PUT (dims   ,ndims,octdim)				\
	intVEC_PUT (periods,ndims,octprd)				\
	intVEC_PUT (coords ,ndims,octcrd)				\
	RET_4_ARG  (info,  octdim,octprd,octcrd) /*retval decl*/	\
}


/* -------------------------------------------------------------------------
 * [info rank  ]        = MPI_Cart_rank     (comm, coords)
 * ------------------------------------------------------------------------- */

#define PATN_INT_F_COM_INTVEC(PNAME)					\
{									\
	NARGCHK    (PNAME,2)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,coords)	/* coords, ncoords */		\
									\
	int rank=0,							\
	    info=   PNAME (comm, coords, &rank);			\
									\
	RET_2_ARG  (info,                 rank)				\
}


/* -------------------------------------------------------------------------
 * [info  neighs]       = MPI_Graph_neighbors       (comm, rank)
 * [info  coords]       = MPI_Cart_coords           (comm, rank)
 * ------------------------------------------------------------------------- */

#define PATN_INTVEC_COORD_F_COM_INT(PNAME)				\
{									\
	NARGCHK    (PNAME,2)						\
	BLCK_ONECOM(PNAME,0)		/* comm */			\
	BLCK_ONEINT(PNAME,1,rank)					\
									\
	int ndims  =0,							\
	    info   =MPI_Cartdim_get (comm, &ndims);			\
	if (info){			/* scope-out retval decl */	\
	  RET_2_ARG(info, Matrix())					\
	}								\
									\
	int coords [ndims];						\
	    info =  PNAME (comm, rank, ndims,coords);			\
									\
	intVEC_PUT(coords,ndims,octcrd)					\
	RET_2_ARG  (info,       octcrd)		/*retval decl*/		\
}


/* -------------------------------------------------------------------------
 * [info rsrc rdst]     = MPI_Cart_shift    (comm, dir,   disp)
 * ------------------------------------------------------------------------- */

#define PATN_2INT_F_COM_2INT(PNAME)					\
{									\
	NARGCHK    (PNAME,3)						\
	BLCK_ONECOM(PNAME,0)		/* comm */			\
	BLCK_ONEINT(PNAME,1,dir )					\
	BLCK_ONEINT(PNAME,2,disp)					\
									\
	int rsrc=0, rdst=0,						\
	    info =  PNAME (comm, dir,disp, &rsrc, &rdst);		\
									\
	RET_3_ARG  (info,                   rsrc,  rdst)		\
}


/* -------------------------------------------------------------------------
 * [info comm_sub ]     = MPI_Cart_sub      (comm, dims  )
 * ------------------------------------------------------------------------- */

#define PATN_COM_F_COM_INTVEC(PNAME)					\
{									\
	NARGCHK    (PNAME,2)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,dims)	/* dims, ndims */		\
									\
	MPI_Comm	   comm_sub=0;					\
	int info =  PNAME (comm, dims,					\
			  &comm_sub );					\
									\
	RET_2_ARG  (info,  MPITB_intcast(				\
			   comm_sub )	)				\
}


/* -------------------------------------------------------------------------
 * [info nrank]         = MPI_Cart_map      (comm, dims, periods)
 * ------------------------------------------------------------------------- */

#define PATN_INT_F_COM_2INTVEC(PNAME)				\
{									\
	NARGCHK    (PNAME,3)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,dims)	/* dims,   ndims    */		\
	BLCK_INTVEC(PNAME,2,periods)	/* periods,nperiods */		\
									\
	if (ndims != nperiods){						\
	  error(STRFY(PNAME) ": required args#2-3"			\
		" (dims-periods) (intvec) of same size");		\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}								\
									\
	int nrank=  0,							\
	    info =  PNAME (comm, ndims,dims,periods,			\
			  &nrank);					\
									\
	RET_2_ARG  (info,  nrank)					\
}


/* -------------------------------------------------------------------------
 * [info nrank]         = MPI_Graph_map     (comm, index, edges)
 * ------------------------------------------------------------------------- */

#define PATN_INT_F_COM_INTVEC_INTVEC(PNAME)				\
{									\
	NARGCHK    (PNAME,3)		/* get args */			\
	BLCK_ONECOM(PNAME,0)		/* comm     */			\
	BLCK_INTVEC(PNAME,1,index)	/* index, nindex */		\
	BLCK_INTVEC(PNAME,2,edges)	/* edges, nedges */		\
									\
	int nrank=  0,							\
	    info =  PNAME (comm, nindex,index,edges,			\
			  &nrank);					\
									\
	RET_2_ARG  (info,  nrank)					\
}

