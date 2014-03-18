/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Unpack
/*
 % ----------------------------------------------------
 % Un-Packs most Octave datatypes from an MPITB message
 % [info retnams] = MPI_Unpack (buf, pos, comm [,'vname']... )
 % ----------------------------------------------------
 */
#include "mpitb.h"	// buildblocks NARGCHK, etc
#include "hBuff.h"	// type enums - declare vars - aux get_MPI_Buff
//#include <ov-cell.h>	// avoid errmsg "cell -- incomplete datatype"
//#include <Cell.h>
//#include <ov-struct.h>// avoid errmsg "Oct.map -- invalid use undef type"
#include <oct-map.h>


/* ----------------------------------------------------------------
 * global scope args vars, so that we can use them in aux functions
 * [info retnams] = MPI_Unpack (buf, pos, comm [,'vname']... )
 * ---------------------------------------------------------------- */
DECLARE_BUFF_VARS	// global scope args vars (BUF, SIZ, TYPE)
int      IDX;		// and IDX / COM
MPI_Comm COM;		// so that we can use them in aux functions

/* ----------------------------------------------------------------
 * Compare to MPI_Pack.c
 * ---------------------------------------------------------------- */


/*------------------------------*/
int unpack_class(octave_value &ov,		/* forward declaration */
	   const std::string&name,		/* for recursive descend */
	   string_vector &retnams);		/* along the datatype */
/*------------------------------*/


/* ----------------------------------------------------------------
 * In pack_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:     return pack_cell   (ov.cell_value   ());
 *    case ov_scalar:   return pack_scalar (ov.scalar_value ());
 *    ...
 * }
 *
 * first we will define all those pack_CASE(ov.access_value()) functions
 * ---------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * who is calling unpack_class can be told from the 3rd argument
 * to whom original packed var names are appended
 * Calls from main: arg#2 desired name or "", #3="main"
 * Recursive calls: arg#2          always "", #3="cell"/"strc"
 * ---------------------------------------------------------------- */


/*---------------
   1         cell           use MPI_Pack        \n\
 *---------------*/
/*------------------------------*/	// to unpack a cell, get nelems, then
int unpack_cell(octave_value &ov,	// create cell[dims], and then get
	    const dim_vector &dv){	// recursively the elmnts themselves
/*------------------------------*/	// & connect them here
  int info;		    intDEX	 n;
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	&n,1,MPI_INT,
					COM)))	return(info);
  if( (n != dv.numel()) )			return(MPI_ERR_ARG);
  Cell	    oc (dv);				// Create->Delete on ret?!?
  if( (n != oc.capacity()) )			return(MPI_ERR_ARG);

  string_vector rnams("cell");			// just to indicate recursion
  octave_value  celem;				// double-check constr/destr
  for (intDEX i=0; i<n; i++){
    if((info=unpack_class(celem,"",rnams)))	return(info);
    oc(i)=celem;
  }
  ov=oc;
  return(MPI_SUCCESS);
}



/* ----------------------------------------------------------------
 * all scalars unpacked the same way, changing type -> make it a macro.
 * even though macro(3 arg), always define fun_CASE(1 arg ov.XXX_value())
 * fun suffix _CASE determines fun argtype(OCT_TYP) _and_ MPI_type
 * in general, argtype is NOT a C type, but an object
 * ----------------------------------------------------------------
 * BUG: should call get_MPI_Type instead of re-programming MPI_TYPE again
 * RATIONALE: any?
 * BUG: ABUSE!!! protected scalar attributes (should use iptr() ?!?)
 * RATIONALE: anyways, Complex has no iptr(), so what the heck
 * ---------------------------------------------------------------- */
#define UPCK_SCALAR(CASE,OCT_TYP,MPI_TYP)	\
/*--------------------------------*/					\
int unpack_##CASE(octave_value &ov){					\
/*--------------------------------*/					\
  int info;		   OCT_TYP	 scl;	/* BUG: ABUSE!!! */	\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	&scl,1,MPI_TYP,	COM)))	return(info);\
  ov = scl;				/* uses default constructor */	\
  return(MPI_SUCCESS);                                                  \
}
/*---------------
   2         scalar         MPI_DOUBLE          \n\
   3 complex scalar         MPI_DOUBLE_COMPLEX  \n\
   7         bool           MPI_CXX_BOOL        \n\
 *---------------*/
  UPCK_SCALAR(scalar ,double ,MPI_DOUBLE        )
  UPCK_SCALAR(complex,Complex,MPI_DOUBLE_COMPLEX)
  UPCK_SCALAR(bool   ,bool   ,MPI_CXX_BOOL      )
/*---------------
  12    int8                MPI_BYTE            \n\
  13   int16                MPI_SHORT           \n\
  14   int32                MPI_INT             \n\
  15   int64                MPI_LONG_LONG       \n\
  16   uint8                MPI_UNSIGNED_CHAR   \n\
  17  uint16                MPI_UNSIGNED_SHORT  \n\
  18  uint32                MPI_UNSIGNED        \n\
  19  uint64                MPI_UNSIGNED_LONG_LONG\n\
 *---------------*/
  UPCK_SCALAR(i8 ,  octave_int8 , MPI_BYTE		)
  UPCK_SCALAR(i16,  octave_int16, MPI_SHORT		)
  UPCK_SCALAR(i32,  octave_int32, MPI_INT		)
  UPCK_SCALAR(i64,  octave_int64, MPI_LONG_LONG		)
  UPCK_SCALAR(u8 , octave_uint8 , MPI_UNSIGNED_CHAR	)
  UPCK_SCALAR(u16, octave_uint16, MPI_UNSIGNED_SHORT	)
  UPCK_SCALAR(u32, octave_uint32, MPI_UNSIGNED		)
  UPCK_SCALAR(u64, octave_uint64, MPI_UNSIGNED_LONG_LONG)



/* ----------------------------------------------------------------
 * all arrays unpacked the same way, changing type -> make it a macro.
 * ----------------------------------------------------------------
 * WRT strings: just change the ov=oa; line // create string, not char matrix
 * ---------------------------------------------------------------- */
#define UPCK_STRING(CASE,OCT_TYP,MPI_TYP)	\
/*--------------------------------*/					\
int unpack_##CASE(octave_value &ov,					\
	      const dim_vector &dv){					\
/*--------------------------------*/					\
  int info;		    intDEX	 n;				\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	&n,1,MPI_INTDEX,		\
					COM)))	return(info);		\
  if( (n != dv.numel()) )			return(MPI_ERR_ARG);	\
  OCT_TYP   oa (dv);							\
  if( (n != oa.capacity()) )			return(MPI_ERR_ARG);	\
				    void*p = oa.mex_get_data();		\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	 p,n,MPI_TYP,			\
					COM)))	return(info);
//ov=oa;
//return(MPI_SUCCESS);
//}
#define UPCK_MATRIX(CASE,OCT_TYP,MPI_TYP)	\
	UPCK_STRING(CASE,OCT_TYP,MPI_TYP)				\
  ov=oa;				/* chg this line for strings */	\
  return(MPI_SUCCESS);							\
}
/*---------------
   4         matrix/array   MPI_DOUBLE * n      \n\
   5 complex matrix/array   MPI_DOUBLE_COMPLEX * n\n\
   8    bool matrix/array   MPI_CXX_BOOL*n      \n\
 *---------------*/
  UPCK_MATRIX(sc_mat,       NDArray,MPI_DOUBLE	      )
  UPCK_MATRIX(cx_mat,ComplexNDArray,MPI_DOUBLE_COMPLEX)
  UPCK_MATRIX(bl_mat,   boolNDArray,MPI_CXX_BOOL      )
/*---------------
   9    char matrix/array   MPI_CHAR   * n      \n\
  10    (dq_)string         MPI_CHAR   * n      \n\
  11      sq_string         MPI_CHAR   * n      \n\
 *---------------*/
  UPCK_STRING(ch_mat,   charNDArray,MPI_CHAR)	ov=oa;	return(MPI_SUCCESS);}
  UPCK_STRING(string,   charNDArray,MPI_CHAR)	ov = octave_value(oa,true);
							return(MPI_SUCCESS);}
  UPCK_STRING(sq_str,   charNDArray,MPI_CHAR)	ov = octave_value(oa,true,'\'');
							return(MPI_SUCCESS);}
/*---------------
  20    int8 array          MPI_BYTE            * n \n\
  21   int16 array          MPI_SHORT           * n \n\
  22   int32 array          MPI_INT             * n \n\
  23   int64 array          MPI_LONG_LONG       * n \n\
  24   uint8 array          MPI_UNSIGNED_CHAR   * n \n\
  25  uint16 array          MPI_UNSIGNED_SHORT  * n \n\
  26  uint32 array          MPI_UNSIGNED        * n \n\
  27  uint64 array          MPI_UNSIGNED_LONG_LONG * n \n\
 *---------------*/
  UPCK_MATRIX( i8_mat,  int8NDArray , MPI_BYTE          )
  UPCK_MATRIX(i16_mat,  int16NDArray, MPI_SHORT         )
  UPCK_MATRIX(i32_mat,  int32NDArray, MPI_INT           )
  UPCK_MATRIX(i64_mat,  int64NDArray, MPI_LONG_LONG     )
  UPCK_MATRIX( u8_mat, uint8NDArray , MPI_UNSIGNED_CHAR )
  UPCK_MATRIX(u16_mat, uint16NDArray, MPI_UNSIGNED_SHORT)
  UPCK_MATRIX(u32_mat, uint32NDArray, MPI_UNSIGNED      )
  UPCK_MATRIX(u64_mat, uint64NDArray, MPI_UNSIGNED_LONG_LONG)



/*---------------
   6         range          MPI_DOUBLE * 3 / DBL*3+IDX using MPI_Pack() \n\
 *---------------*/
/*-------------------------------*/	// to unpack a range
int unpack_range(octave_value &ov){	// get base,limit,incr,nelem
/*-------------------------------*/	// just 3 doubles + 1 intDEX
  Range rv;
  double*d = (double*) & rv;
  intDEX*i = (intDEX*) & d[3];
  int info;
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	d,3,MPI_DOUBLE,	COM)))	return(info);
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	i,1,MPI_INTDEX,	COM)))	return(info);
  if(rv.nelem() != *i)					return(MPI_ERR_ARG);
  ov=rv;
  return(MPI_SUCCESS);
}



/* ----------------------------------------------------------------
 * all sparses unpacked the same way, changing type -> make it a macro.
 * ---------------------------------------------------------------- */
#define UPCK_SPARSE(CASE,OCT_TYP,MPI_TYP)	\
/*--------------------------------*/					\
int unpack_##CASE(octave_value &ov,					\
	      const dim_vector &dv){					\
/*--------------------------------*/					\
  int info;		    intDEX	 n,c;				\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,&n,1,MPI_INTDEX,COM))) return(info);	\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,&c,1,MPI_INTDEX,COM))) return(info);	\
									\
  if( (c !=	dv(1)) )			return(MPI_ERR_ARG);	\
  OCT_TYP   os (dv(0),dv(1),n);						\
  if( (n != os.capacity())||						\
      (c != os.columns ()) )			return(MPI_ERR_ARG);	\
			    void   *dp = os.mex_get_data(),		\
				   *rp = os.mex_get_ir  (),		\
				   *cp = os.mex_get_jc  ();		\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,dp,n  ,MPI_TYP   ,COM)))return(info);\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,rp,n  ,MPI_INTDEX,COM)))return(info);\
  if((info=MPI_Unpack(BUF,SIZ,&IDX,cp,c+1,MPI_INTDEX,COM)))return(info);\
  ov=os;								\
  return(MPI_SUCCESS);							\
  }
/*---------------
  28 sparse bool matrix     MPI_CXX_BOOL       * nzmax  \n\
  29 sparse matrix          MPI_DOUBLE         * nzmax  \n\
  30 sparse complex matrix  MPI_DOUBLE_COMPLEX * nzmax  \n\
 *---------------*/
  UPCK_SPARSE(bl_spr,   SparseBoolMatrix, MPI_CXX_BOOL      )
  UPCK_SPARSE(sc_spr,       SparseMatrix, MPI_DOUBLE        )
  UPCK_SPARSE(cx_spr,SparseComplexMatrix, MPI_DOUBLE_COMPLEX)



/*---------------
  31         struct         use MPI_Pack        \n\
 *---------------*/
/*-------------------------------*/	// to unpack a struc, get nkeys,
int unpack_struct(octave_value &ov,	// create map, and for each key
	     const dim_vector &dv){	// recursively get contents &
/*-------------------------------*/	// connect them here
  int info;		    intDEX	 n;
  if((info=MPI_Unpack(BUF,SIZ,&IDX,	&n,1,MPI_INTDEX,COM)))	return(info);
/* ----------------------------------------------------------------
 * map length is n#fields, each field is ND cell / same dims (dv)
 * however, we understand it as ND struct / each elemt has n fields
 * unpack newly added key_list -> to keep field order listing
 * ---------------------------------------------------------------- */
  string_vector	     key_list(n);	// to be used with map constructor
  for (intDEX i=0; i<n; i++){		// nkeys: foreach, get key
		int	 len;		// works for len==0
    if((info=MPI_Unpack(BUF,SIZ,&IDX, &len,1,MPI_INTDEX,COM)))	return(info);
		char key[len];		// field name
    if((info=MPI_Unpack(BUF,SIZ,&IDX,key,len,MPI_CHAR  ,COM)))	return(info);
//  printf("unpacked fieldname: %s\n",key);
		     key_list[i]=key;
  }

  string_vector rnams("strc");		// just to indicate recursion
    Octave_map om(dv,key_list);		// Create->Delete on ret?!?

for(Octave_map::
  const_iterator p = om.begin();	// iterate through keys(fnames)
		 p!= om.end();   p++){
// std::string key = om.key     (p);	// don't unpack field name (key) again
//  printf("iterated fieldname= %s\n",	// trust on same key ordering when
//			 key.c_str());	// _Pack'ng and _Unpack'ng
    octave_value conts;			// Cell w/ND contents
    if( (info=unpack_class(conts,"",rnams)) )		return(info);
    if (! conts.is_cell()){
      error(STRNGFY(NAME) ": non-cell map contents\n");	return(MPI_ERR_ARG);
    }
    if (conts.dims() != dv){		// contents dims should match
      error(STRNGFY(NAME) ": inconsistent map dims\n");	return(MPI_ERR_ARG);
    }
//  om.assign  (key,conts);		// CREDIT: BUG!! detected/patched by
//  om.assign  (key,conts.cell_value());// Breanndán Ó Nualláin, CS U.Amsterdam
//  om.contents(key)=conts.cell_value();// contents() advantage: (key) or (p)
    om.contents( p )=conts.cell_value();// Breanndán patch is still relevant
  }

  ov=om;
  return(MPI_SUCCESS);
}



/*---------------
  32 list                       unsupported     \n\
  33 cs-list                    unsupported     \n\
  34 magic-colon                unsupported     \n\
  35 built-in function          unsupported     \n\
  36 built-in mapper function   unsupported     \n\
  37 user-defined function      unsupported     \n\
  38 dynamically-linked functionunsupported     \n\
  39 function handle            unsupported     \n\
  40 inline function            unsupported     \n\
  41 streamoff                  unsupported     \n\
  42 overloaded function        unsupported     \
 *---------------*/



/* ----------------------------------------------------------------
 * In unpack_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:	info = unpack_cell   (ov,dv);	break;
 *    case ov_scalar:	info = unpack_scalar (ov   );	break;
 *    ...
 * }
 *
 * first we defined all those unpack_CASE(ov [,dv]) functions
 * now we define these handy symbols to save typing all those cases
 * fortunately, we defined enum typeids and unpack_* to share names & be short
 *      so we can now write ov_##NAME ... unpack_##NAME
 * unfortunately, creating non-scalars to unpack onto requires dv
 *      so we need 2 symbols
 * ---------------------------------------------------------------- */
#define CASE_SCLR(TID_SUFFIX) \
case ov_##TID_SUFFIX:   info = unpack_##TID_SUFFIX(ov   ); break;

#define CASE_DIMS(TID_SUFFIX) \
case ov_##TID_SUFFIX:   info = unpack_##TID_SUFFIX(ov,dv); break;



/*-------------------------------*/	// to unpack a class' header
int unpack_class(octave_value &ov,	// get type_id (0 is bypass)
	   const std::string&name,	// vnam-strlen, dims[ndim]
	   string_vector &retnams){	// and then specific part
/*-------------------------------*/
  int info, t_id;
  if((info=MPI_Unpack(BUF,SIZ,&IDX,  &t_id,1,MPI_INT,	COM)))	return(info);
  if (	t_id< ov_MIN_SUPP		// base==0, cell==1==MIN_SUPP
//  ||	t_id==ov_range			// ov_range==6 can be packed
    ||	t_id> ov_MAX_SUPP		//        struc==31==MAX_SUPP
	){				// list==32, ...strmoff==41==MAX_TYPE
	 warning_with_id("MPITB:Pack-invalid-type",
	 STRNGFY(NAME) ": unsupported datatype: %s, class: %s",
	    ov.type_name ().c_str(),	// both type & class?
	    ov.class_name().c_str());	// does c_str() work?
//	 return(MPI_ERR_TYPENOTSUP);	// warning avoids ret(nothing)
	 return(MPI_ERR_TYPE);		// TYPENOTSUP does not exist in OpenMPI
  }

  int        len;			// works for length()==0
  if((info=MPI_Unpack(BUF,SIZ,&IDX,   &len,1,MPI_INT,	COM)))	return(info);
  char pname[len];
  if((info=MPI_Unpack(BUF,SIZ,&IDX,pname,len,MPI_CHAR,	COM)))	return(info);

  int       ndim;
  if((info=MPI_Unpack(BUF,SIZ,&IDX,  &ndim,1,MPI_INT,	COM)))	return(info);
  int  dims[ndim];
  if((info=MPI_Unpack(BUF,SIZ,&IDX,dims,ndim,MPI_INTDEX,COM)))	return(info);

  dim_vector dv;			// no explicit constr. dv(ndims); ?!?
  dv.resize(ndim);
  for(int i=0; i<ndim; i++) dv(i)=dims[i];

  switch (t_id) {
//  case ov_unknown:
//  case ov_cell:	info = unpack_cell   (ov,dv);	break;
    CASE_DIMS(cell   )

//  case ov_scalar:	info = unpack_scalar (ov   );	break;
    CASE_SCLR(scalar )
    CASE_SCLR(complex)
    CASE_DIMS(sc_mat )
    CASE_DIMS(cx_mat )

    CASE_SCLR(range  )

    CASE_SCLR(bool   )
    CASE_DIMS(bl_mat )

    CASE_DIMS(ch_mat )
    CASE_DIMS(string )
    CASE_DIMS(sq_str )

    CASE_SCLR( i8    )		CASE_DIMS( i8_mat)
    CASE_SCLR(i16    )		CASE_DIMS(i16_mat)
    CASE_SCLR(i32    )		CASE_DIMS(i32_mat)
    CASE_SCLR(i64    )		CASE_DIMS(i64_mat)
    CASE_SCLR( u8    )		CASE_DIMS( u8_mat)
    CASE_SCLR(u16    )		CASE_DIMS(u16_mat)
    CASE_SCLR(u32    )		CASE_DIMS(u32_mat)
    CASE_SCLR(u64    )		CASE_DIMS(u64_mat)

    CASE_DIMS(bl_spr )
    CASE_DIMS(sc_spr )
    CASE_DIMS(cx_spr )

    CASE_DIMS(struct )

//  Should not reach here, see if(t_id){} at the beginning
//  case ov_list: case ov_cs_list:
//  case ov_magic_colon: case ov_builtin: case ov_mapper: case ov_user_func:
//  case ov_dld_func: case ov_fcn_handle: case ov_fcn_inline: case ov_streamoff:
//  case ov_unknown:
//  default:		return MPI_ERR_TYPE;	// unreached below?

    default:
	error(STRNGFY(NAME) ": internal error datatype: %s, class: %s",
                ov.type_name ().c_str(),	// both type & class?
                ov.class_name().c_str());	// does c_str() work?
        info = MPI_ERR_TYPE;			// later RET_1_ARG(info=pack..)
  }

  if (info) {} // delete ov;		/* double-check Create->Delete stuff */
  else if (retnams[0]=="main"){		/* MPI_SUCCESS, put var in workspace */

  retnams.append(std::string(pname));	/* write down original name */

  std::string oname =			/* octave name */
	(name.empty() ? pname : name );	/* no name given -> use its own */

  symbol_table::symbol_record varsym = symbol_table::find_symbol(oname);
  if(varsym.is_defined())
		varsym.varref() = octave_value(ov);
  else warning_with_id("MPITB:internal-errors",
	STRNGFY(NAME) ": unable to define variable %s"
			" in current symbol table", oname.c_str());
  }
  return(info);
}


/*--------------------------------------------------------------------------*/
/* MPI_Unpack() ------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
DEFUN_DLD(NAME, args, nargout,
"MPI_Unpack             Unpack an Octave variable from contiguous memory\n\
\n\
  [info retnam] = MPI_Unpack (buf, pos, comm [,'vname']... )\n\
\n\
  buf  (var) Octave variable used as buffer\n\
  pos  (int) current position in buffer, in bytes\n\
               MPI_Unpack starts unpacking from that position on\n\
               pos is incremented, can be reused on succesive unpacks\n\
  comm (int) communicator for packed message (handle)\n\
  vname(str) desired name(s) for the unpacked Octave variable(s)\n\
\n\
  retnam(str)original name(s) of packed variable(s)\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator ('NULL' communicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument    (N/A under Octave)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (N/A under Octave)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
     15 MPI_ERR_TRUNCATE       Message  truncated on receive\n\
\n\
  SEE ALSO: MPI_Pack, MPI_Pack_size\n\
\n\
")
{
//	NARGCHK    (NAME,3)		// nope, allow for 3,4,5...
	intDEX nargin = args.length();
	if (nargin < 3){
	  error(STRNGFY(NAME) ": 3 args required");
	  RET_1_ARG (MPI_ERR_ARG);	// error returns nothing
	}
/* ----------------------------------------------------------------
 * [info retnams] = MPI_Unpack (buf, pos, comm [,'vname']... )
 * first arg must be variable, 2nd must be updated if it's a var
 * ---------------------------------------------------------------- */
	BLCK_ONESYM(NAME,0,buf)		// bufnam, bufsym
	BLCK_NULSYM(NAME,1,pos)		// no NULL-check for possym
	BLCK_ONEINT(NAME,1,pos)		// pos -> IDX (global scope)
	BLCK_ONECOM(NAME,2)		// comm-> COM (global scope)

	BUF = get_MPI_Buff(bufsym, SIZ,true,	// SIZ in bytes
				  TYPE,false);	// buffer MK_UNIQ not required
	IDX = pos ;				// it won't be overwritten
	COM = comm;

  string_vector orgnames("main");	// original var names
  octave_value newov;			// empty initial newov?!?
					// double-check destructors
  int nvar=nargin-3, info=0, i;		// loopvar required outside
  if (nvar==0){				// no names given:
						
    for (i=0; IDX<SIZ && !info; i++){	// unpack everything in BUF
      info=unpack_class(newov, "", orgnames);	/* ------------------------
	* we'll get ERR_TRUNCATE unless finally IDX==SIZ
	* ie, buffer has no room (garbage) left after the last var
	* so either we got exactly i vars ok, and IDX==SIZ,
	*        or we got       i-1 vars ok, and i-th caused info ------- */
    }
    nvar=i;

  }else{				// unpack an exact no. vars

    for (i=0; (i<nvar) && !info; i++){

      if (! args(i+3).is_string() ){	// break on 1st error
        info = MPI_ERR_ARG;		// error returns nothing at all
	warning("MPITB:Pack-invalid-type",
	STRNGFY(NAME) ": args#4 and following must be strings `vname'");
	nvar=i+1;			// but there might be someting to ret
      }else{
	std::string name=args(i+3).string_value();
        info=unpack_class(newov, name, orgnames);
      }
    }
  }

   if (possym.is_defined())
       possym.varref() = octave_value(IDX);		// MPI updates pos, so do we

/* ----------------------------------------------------------------
 * [info retnams] = MPI_Unpack (buf, pos, comm [,'vname']... )
 * ---------------------------------------------------------------- */
  Matrix m (1,nvar,MPI_SUCCESS);	// "info"  retval
	 m (0,nvar-1) = info;		// possibly error on last var

  if (orgnames.length()>1){		// retnams retval
      orgnames[0]=orgnames[1];		// get rid of trailing "main"
      orgnames.uniq();
  }else{
      orgnames[0]="";			// no memberfun to delete "main"?!?
  }
 
	RET_2_ARG (m, orgnames)
}
