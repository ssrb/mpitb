/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Pack_size
/*
 * ----------------------------------------------------
 * Returns the upper bound on the amount of space needed to pack a message
 * [info size] = MPI_Pack_size (var, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"	// buildblocks NARGCHK, etc
#include "hType.h"	// type enums - (get_MPI_Type not used)
//#include <ov-cell.h>	// avoid errmsg "cell -- incomplete datatype"
//#include <Cell.h>
//#include <ov-struct.h>// avoid errmsg "Oct.map -- invalid use undef type"
#include <oct-map.h>	// already includes Cell.h


/* ----------------------------------------------------------------
 * global scope args vars, so that we can use them in aux functions
 *  info          = MPI_Pack(var, buf, pos, comm)
 * ---------------------------------------------------------------- */
MPI_Comm COM;		// so that we can use it in aux functions

/* ----------------------------------------------------------------
 * Compare to MPI_Pack.cc
 * ----------------------------------------------------------------
 * for simplicity, all recursive routines return
 * 	either a positive size value
 * 	or a negative MPI error code
 * see the last paragraph of code for an explanation
 * ---------------------------------------------------------------- */


/*----------------------------------*/		/* forward declaration */
int size_class(const octave_value &ov,		/* for recursive descend */
	       const std::string&name);		/* along the datatype */
/*----------------------------------*/


/* ----------------------------------------------------------------
 * In size_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:     return size_cell   (ov.cell_value   ());
 *    case ov_scalar:   return size_scalar (ov.scalar_value ());
 *    ...
 * }
 *
 * first we will define all those size_CASE(ov.access_value()) functions
 * ---------------------------------------------------------------- */


/*---------------
   1         cell           use MPI_Pack        \n\
 *---------------*/
/*----------------------------*/
int size_cell(const Cell &cell){	// compare to pack_cell()
/*----------------------------*/
  intDEX n = cell.capacity();
  int info, size;	/*n*/
  if((info=MPI_Pack_size(1,MPI_INTDEX,COM,	&size)))	return(-info);

  int sz;
  for (intDEX i=0; i<n; i++){
    const octave_value &ov = cell(i);
    if ((sz=size_class(ov,""))<0)	return(sz);	else size+=sz;
  }
  return(size);
}


/* ----------------------------------------------------------------
 * all scalars packed the same way, changing type -> make it a macro.
 * even though macro(3 arg), always define fun_CASE(1 arg ov.XXX_value())
 * fun suffix _CASE determines fun argtype(OCT_TYP) _and_ MPI_type
 * in general, argtype is NOT a C type, but an object
 * ----------------------------------------------------------------
 * BUG: should call get_MPI_Type instead of re-programming MPI_TYPE again
 * RATIONALE: any?
 * BUG: scalar size doesnot depend on scalar value, erase OCT_TYP & scalar
 * RATIONALE: anyways, Cells/structs/arrays do depend on dims etc, so help me
 * ---------------------------------------------------------------- */
#define SIZE_SCALAR(CASE,OCT_TYP,MPI_TYP)	\
/*-----------------------------------*/					\
int size_##CASE(const OCT_TYP &scalar){					\
/*-----------------------------------*/					\
  int info, size;	/* scalar */					\
  if((info=MPI_Pack_size(1,MPI_TYP,COM,	&size)))	return(-info);	\
  else							return( size);	\
}
/*---------------
   2         scalar         MPI_DOUBLE          \n\
   3 complex scalar         MPI_DOUBLE_COMPLEX  \n\
   7         bool           MPI_CXX_BOOL        \n\
 *---------------*/
  SIZE_SCALAR(scalar ,double ,MPI_DOUBLE        )
  SIZE_SCALAR(complex,Complex,MPI_DOUBLE_COMPLEX)
  SIZE_SCALAR(bool   ,bool   ,MPI_CXX_BOOL      )
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
  SIZE_SCALAR(i8 ,  octave_int8 , MPI_BYTE		)
  SIZE_SCALAR(i16,  octave_int16, MPI_SHORT		)
  SIZE_SCALAR(i32,  octave_int32, MPI_INT		)
  SIZE_SCALAR(i64,  octave_int64, MPI_LONG_LONG		)
  SIZE_SCALAR(u8 , octave_uint8 , MPI_UNSIGNED_CHAR	)
  SIZE_SCALAR(u16, octave_uint16, MPI_UNSIGNED_SHORT	)
  SIZE_SCALAR(u32, octave_uint32, MPI_UNSIGNED		)
  SIZE_SCALAR(u64, octave_uint64, MPI_UNSIGNED_LONG_LONG)



/* ----------------------------------------------------------------
 * all arrays packed the same way, changing type -> make it a macro.
 * ---------------------------------------------------------------- */
#define SIZE_MATRIX(CASE,OCT_TYP,MPI_TYP)       \
/*----------------------------------*/                                  \
int size_##CASE(const OCT_TYP &array){                                  \
/*----------------------------------*/                                  \
  int info, size;	/* n */						\
  if((info=MPI_Pack_size(1,MPI_INTDEX,COM,   &size)))	return(  -info);\
		/*  void*p = array.mex_get_data(); */			\
  int sz;	  intDEX n = array.capacity();				\
  if((info=MPI_Pack_size(n,MPI_TYP   ,COM,   &sz  )))	return(  -info);\
  else							return(sz+size);\
}
/*---------------
   4         matrix/array   MPI_DOUBLE * n      \n\
   5 complex matrix/array   MPI_DOUBLE_COMPLEX  \n\
   8    bool matrix/array   MPI_CXX_BOOL*n      \n\
 *---------------*/
  SIZE_MATRIX(sc_mat,       NDArray,MPI_DOUBLE        )
  SIZE_MATRIX(cx_mat,ComplexNDArray,MPI_DOUBLE_COMPLEX)
  SIZE_MATRIX(bl_mat,   boolNDArray,MPI_CXX_BOOL      )
/*---------------
   9    char matrix/array   MPI_CHAR * n        \n\
  10    (dq_)string         MPI_CHAR * n        \n\
  11      sq_string         MPI_CHAR * n        \n\
 *---------------*/
  SIZE_MATRIX(ch_mat,   charNDArray,MPI_CHAR)	// can't tell difference
//int size_string(const charNDArray &array){return(size_ch_mat(array));}
//int size_sq_str(const charNDArray &array){return(size_ch_mat(array));}
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
  SIZE_MATRIX( i8_mat,  int8NDArray , MPI_BYTE          )
  SIZE_MATRIX(i16_mat,  int16NDArray, MPI_SHORT         )
  SIZE_MATRIX(i32_mat,  int32NDArray, MPI_INT           )
  SIZE_MATRIX(i64_mat,  int64NDArray, MPI_LONG_LONG     )
  SIZE_MATRIX( u8_mat, uint8NDArray , MPI_UNSIGNED_CHAR )
  SIZE_MATRIX(u16_mat, uint16NDArray, MPI_UNSIGNED_SHORT)
  SIZE_MATRIX(u32_mat, uint32NDArray, MPI_UNSIGNED      )
  SIZE_MATRIX(u64_mat, uint64NDArray, MPI_UNSIGNED_LONG_LONG)



/*---------------
   6         range          MPI_DOUBLE * 3 / DBL*3+INT using MPI_Pack() \n\
 *---------------*/
/*-------------------------------*/
int size_range(const Range &range){	// compare to pack_range
/*-------------------------------*/
//double*d = (double*) &range;		// const_cast<double*>(reinterpret_cast
//intDEX*i = (intDEX*) & d[3];		//      <const double*>(&range))
  int info, size,sz;
  if((info=MPI_Pack_size(3,MPI_DOUBLE,COM,    &size)))	return(  -info);
  if((info=MPI_Pack_size(1,MPI_INTDEX,COM,	&sz)))	return(  -info);
  else							return(sz+size);
}



/* ----------------------------------------------------------------
 * all sparses packed the same way, changing type -> make it a macro.
 * ---------------------------------------------------------------- */
#define SIZE_SPARSE(CASE,OCT_TYP,MPI_TYP)	\
/*-----------------------------------*/					\
int size_##CASE(const OCT_TYP &sparse){					\
/*-----------------------------------*/					\
  int info, size;	/* n,c */					\
  if((info=MPI_Pack_size(2,MPI_INTDEX,COM,   &size)))	return(  -info);\
		/* void*dp= sparse.mex_get_data(), */			\
		/*     *rp= sparse.mex_get_ir  (), */			\
		/*     *cp= sparse.mex_get_jc  (); */			\
  int sz;     intDEX n = sparse.capacity(),				\
		     c = sparse.columns ();				\
  if((info=MPI_Pack_size(n,MPI_TYP   ,COM,	&sz)))	return(  -info);\
  else			/* dp[n] */			size+= sz;	\
  if((info=MPI_Pack_size(n,MPI_INTDEX ,COM,	&sz)))	return(  -info);\
  else			/* rp[n] */			size+= sz;	\
  if((info=MPI_Pack_size(c+1,MPI_INTDEX,COM,	&sz)))	return(  -info);\
  else			/* cp[c+1] */			return(sz+size);\
}
/*---------------
  28 sparse bool matrix     MPI_CXX_BOOL       * nzmax  \n\
  29 sparse matrix          MPI_DOUBLE         * nzmax  \n\
  30 sparse complex matrix  MPI_DOUBLE_COMPLEX * nzmax  \n\
 *---------------*/
  SIZE_SPARSE(bl_spr,   SparseBoolMatrix, MPI_CXX_BOOL      )
  SIZE_SPARSE(sc_spr,       SparseMatrix, MPI_DOUBLE        )
  SIZE_SPARSE(cx_spr,SparseComplexMatrix, MPI_DOUBLE_COMPLEX)



/*---------------
  31         struct         use MPI_Pack        \n\
 *---------------*/
/*-----------------------------------*/
int size_struct(const Octave_map &map){	// compare to pack_struct
/*-----------------------------------*/
  int info, sz, size;			intDEX n = map.nfields();
  if((info=MPI_Pack_size(1,MPI_INTDEX,COM,   &size)))	return(-info);

  string_vector key_list = map.keys();
  if	 ( n != key_list.length() ){	// double-check:
    error(STRNGFY(NAME) ": inconsistent map length\n");	return(-MPI_ERR_ARG);
  }					// iterate  through nfields()
  for(intDEX i=0; i<n; i++){		// strlen() is size_t in _Pack
    int len = key_list[i].length()+1;	// fieldname len, works for len==0
    if((info=MPI_Pack_size(1,MPI_INTDEX,COM,&sz))) return(-info); else size+=sz;
    if((info=MPI_Pack_size(len,MPI_CHAR,COM,&sz))) return(-info); else size+=sz;
//  printf("accounted fieldname: %s (%d)\n",key_list[i].c_str(),len);
  }

  dim_vector map_dims= map.dims();	// struct array dimensions (ND)
  Octave_map::				// each key stores ND field-values
  const_iterator   p = map.begin();	// iterate through keys(fnames)
  for	     (n=0; p!= map.end();   p++, n++){
//   std::string key = map.key     (p);	// field name (key) not packed again
//   printf("iterated fieldname= %s\n",	// trust on same key ordering when
//			  key.c_str());	// iterating map.begin/end() on _Unpack
   Cell conts	     = map.contents(p);	// Cell w/ND contents
    if (conts.dims()!= map_dims){	// double-check
      error(STRNGFY(NAME) ": inconsistent map dims\n");	return(-MPI_ERR_ARG);
    }					// each elemt should have same ND
    if((sz=size_class(conts,""))<0)		   return(   sz); else size+=sz;
  }					// recursively, size of contents

  if (n != map.nfields()){		// final double-check:
    error(STRNGFY(NAME) ": inconsistent map length\n");	return(-MPI_ERR_ARG);
  }					// iterated through nfields()
  return(size);
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
 * In size_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:     return size_cell   (ov.cell_value   ());
 *    case ov_scalar:   return size_scalar (ov.scalar_value ());
 *    ...
 * }
 *
 * first we defined all those size_CASE(ov.access_value()) functions
 * now we define this handy symbol to save typing all those cases
 * fortunately, we defined enum typeids and pack_* to share names and be short
 *      so we can now write ov_##NAME ... pack_##NAME
 * unfortunately, octave access-function names are too long and most times
 * we didn't keep a matching enum/pack_xxx name -- way too long
 *      can't write ov. ##NAME ## _value()
 * ---------------------------------------------------------------- */
#define CASE_SIZE(TID_SUFFIX,OV_ACCFUN) \
case ov_##TID_SUFFIX: return					(	\
	(sz=size_##TID_SUFFIX(ov.OV_ACCFUN())) <0 ? sz : size+sz );



/*-----------------------------------*/
int size_class(const octave_value &ov,	// compare to pack_class
	       const std::string&name){
/*-----------------------------------*/
    int	t_id =ov.type_id();
  if (	t_id< ov_MIN_SUPP		// base==0, cell==1==MIN_SUPP
//  ||	t_id==ov_range			// ov_range==6 can be packed
    ||	t_id> ov_MAX_SUPP		//        struc==31==MAX_SUPP
	){				// list==32, ...strmoff==41==MAX_TYPE
	warning_with_id("MPITB:Pack-invalid-type",
	STRNGFY(NAME) ": unsupported datatype:[%s], class:[%s]",
	    ov.type_name ().c_str(),	// both type & class?
	    ov.class_name().c_str());	// does c_str() work?
//	return (-MPI_ERR_TYPENOTSUP);	// later RET_1_ARG(info=pack..)
	return (-MPI_ERR_TYPE);		// TYPENOTSUP does not exist in OpenMPI
  }
  int info, size;	/* t_id */
  if((info=MPI_Pack_size(1,MPI_INT,COM,	&size)))	return(-info);
					 size*=3;	// for len/ndim below
  int sz, len = name.length()+1;			// works for len==0
//if((info=MPI_Pack_size(1,MPI_INT,COM,	  &sz)))	return(-info);
//else				     size+=sz;		//len already counted
  if((info=MPI_Pack_size(len,MPI_CHAR,COM,&sz)))	return(-info);
  else				     size+=sz;		// for name itself

		  int ndims = ov.ndims();	// just needed #dims
//	   dim_vector  dimv = ov. dims();	// dims vector not really req
			/* ndims */
//if((info=MPI_Pack_size(    1,MPI_INT   ,COM,	&sz)))	return(-info);
//else			/* dims[] */	   size+=sz;	// ND already counted
  if((info=MPI_Pack_size(ndims,MPI_INTDEX,COM,	&sz)))	return(-info);
  else					   size+=sz;	// for dims

  switch (t_id) {
//  case ov_unknown:
//  case   ov_cell: return( (sz=size_cell(ov.cell_value())) <0 ? sz : size+sz );
    CASE_SIZE(cell   ,         cell_value)

    CASE_SIZE(scalar ,       scalar_value)
    CASE_SIZE(complex,      complex_value)
    CASE_SIZE(sc_mat ,        array_value)
    CASE_SIZE(cx_mat ,complex_array_value)

    CASE_SIZE(range  ,        range_value)
                                                                                
    CASE_SIZE(bool   ,         bool_value)
    CASE_SIZE(bl_mat ,   bool_array_value)
                                                                                
    case ov_string:
    case ov_sq_str:
//  case ov_ch_mat:return((sz=size_ch_mat(ov.char_array_value()))<0?sz:size+sz);
    CASE_SIZE(ch_mat ,   char_array_value)

    CASE_SIZE( i8    ,  int8_scalar_value)
    CASE_SIZE(i16    , int16_scalar_value)
    CASE_SIZE(i32    , int32_scalar_value)
    CASE_SIZE(i64    , int64_scalar_value)
    CASE_SIZE( u8    , uint8_scalar_value)
    CASE_SIZE(u16    ,uint16_scalar_value)
    CASE_SIZE(u32    ,uint32_scalar_value)
    CASE_SIZE(u64    ,uint64_scalar_value)
                                                                                
    CASE_SIZE( i8_mat,   int8_array_value)
    CASE_SIZE(i16_mat,  int16_array_value)
    CASE_SIZE(i32_mat,  int32_array_value)
    CASE_SIZE(i64_mat,  int64_array_value)
    CASE_SIZE( u8_mat,  uint8_array_value)
    CASE_SIZE(u16_mat, uint16_array_value)
    CASE_SIZE(u32_mat, uint32_array_value)
    CASE_SIZE(u64_mat, uint64_array_value)

    CASE_SIZE(bl_spr ,    sparse_bool_matrix_value)
    CASE_SIZE(sc_spr ,         sparse_matrix_value)
    CASE_SIZE(cx_spr , sparse_complex_matrix_value)
                                                                                
                                                                                
    CASE_SIZE(struct ,          map_value)

//  Should not reach here, see if(t_id){} at the beginning
//  case ov_list: case ov_cs_list:
//  case ov_magic_colon: case ov_builtin: case ov_mapper: case ov_user_func:
//  case ov_dld_func: case ov_fcn_handle: case ov_fcn_inline: case ov_streamoff://  case ov_unknown:
//  default:		return MPI_ERR_TYPE;	// unreached below?
  }
	error(STRNGFY(NAME) ": internal error datatype: %s, class: %s",
		ov.type_name ().c_str(),	// both type & class?
		ov.class_name().c_str());	// does c_str() work?
	return (-MPI_ERR_TYPE);			// later RET_1_ARG(info=pack..)
}



/*--------------------------------------------------------------------------*/
/* MPI_Pack_size() ---------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
DEFUN_DLD(NAME, args, nargout,
"MPI_Pack_size  Returns upper bound on amount of space needed to pack a msg\n\
\n\
  [info size] = MPI_Pack_size (var, comm)\n\
\n\
  var        Octave variable to pack\n\
  comm (int) communicator for packed message (handle)\n\
\n\
  size (int) upper bound on size of packed message, in bytes\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator ('NULL' communicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument    (N/A under Octave)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (N/A under Octave)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Pack, MPI_Unpack\n\
\n\
")
{
	NARGCHK    (NAME,2)
/* ----------------------------------------------------------------
 * [info size] = MPI_Pack_size (var, comm)
 * first arg must be variable
 * ---------------------------------------------------------------- */
	BLCK_ONESYM(NAME,0,var)		// varnam, varsym
	BLCK_ONECOM(NAME,1)		// comm-> COM (global scope)

	COM = comm;
    int size = size_class (args(0), varnam);

    if (size>0){				/* if returned positive size */
	RET_2_ARG  (MPI_SUCCESS, size)		/* ...no error */
    }else{					/* don't take braces {} out */
	RET_2_ARG  (      -size,    0)		/* if negative, error code */
    }
}

