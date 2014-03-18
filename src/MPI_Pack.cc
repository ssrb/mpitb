/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Pack
/*
 % ----------------------------------------------------
 % Packs most Octave datatypes into contiguous memory
 % info = MPI_Pack(var, buf, pos, comm)
 % ----------------------------------------------------
 */
#include "mpitb.h"	// buildblocks NARGCHK, etc
#include "hBuff.h"	// type enums - declare vars - aux get_MPI_Buff
//#include <ov-cell.h>	// avoid errmsg "cell -- incomplete datatype"
//#include <Cell.h>
//#include <ov-struct.h>// avoid errmsg "Oct.map -- invalid use undef type"
#include <oct-map.h>	// already includes Cell.h


/* ----------------------------------------------------------------
 * global scope args vars, so that we can use them in aux functions
 *  info          = MPI_Pack(var, buf, pos, comm)
 * ---------------------------------------------------------------- */
DECLARE_BUFF_VARS	// global scope args vars (BUF, SIZ, TYPE)
int      IDX;		// and IDX / COM (IDX int required by LAM/MPI)
MPI_Comm COM;		// so that we can use them in aux functions

/* ----------------------------------------------------------------
 * Compare to MPI_Pack_size.cc / MPI_Unpack.cc
 * ----------------------------------------------------------------
 * See load-save.cc:[read|save|write]_[mat[5]_]binary_data()
 * We should possibly follow std octave conventions for MPITB Packed data
 * (including headers and such...)
 * For now, we simply interpret all these Octave classes
 * ----------------------------------------------------------------
 * Sources of information:
 * typeinfo octave command (registered Octave types)
 * $OCTAVE/src/ov.cc:install_types (same registered types)
 * several .h/.cc files in $OCTAVE/src and $OCTAVE/liboct
 * ----------------------------------------------------------------
 * <unknown type>	octave_base_value	ov[-base].h/.cc
 * cell			octave_cell		ov-cell, ov-base-matrix, Cell
 * scalar		octave_scalar		ov-scalar, ov-base-scalar
 * complex scalar	octave_complex		ov-complex,  " , Complex
 * matrix		octave_matrix		ov-re-mat,ov-base-mat,dMatrix
 * complex matrix	octave_complex_matrix	ov-cx-mat,  " , CMatrix
 * range		octave_range		ov-range, ov-base
 * bool			octave_bool		ov-bool, ov-base-scalar
 * bool matrix		octave_bool_matrix	ov-bool-mat, ov-base-mat, boolM
 * char matrix		octave_char_matrix	ov-ch-mat,   " , chMatrix
 * string		octave_char_matrix_string	ov-str-mat, ov-ch-mat
 * sq_string		octave_char_matrix_sq_string	ov-str-mat, ov-ch-mat
 *
 * int8 scalar		octave_int8_scalar		ov-int8.h/.cc
 * int16 scalar		octave_int16_scalar		ov-int16.h/.cc
 * int32 scalar		octave_int32_scalar		ov-int32.h/.cc
 * int64 scalar		octave_int64_scalar		ov-int64.h/.cc
 * uint8 scalar		octave_uint8_scalar		ov-uint8.h/.cc
 * uint16 scalar	octave_uint16_scalar		ov-uint16.h/.cc
 * uint32 scalar	octave_uint32_scalar		ov-uint32.h/.cc
 * uint64 scalar	octave_uint64_scalar		ov-uint64.h/.cc
 *
 * int8 matrix		octave_int8_matrix /int8NDArray		ov-int8.h /.cc
 * int16 matrix		octave_int16_matrix/int16NDArray	ov-int16.h/.cc
 * int32 matrix		octave_int32_matrix/int32NDArray	ov-int32.h/.cc
 * int64 matrix		octave_int64_matrix/int64NDArray	ov-int64.h/.cc
 * uint8 matrix		octave_uint8_matrix /uint8NDArray	ov-uint8.h /.cc
 * uint16 matrix	octave_uint16_matrix/uint16NDArray	ov-uint16.h/.cc
 * uint32 matrix	octave_uint32_matrix/uint32NDArray	ov-uint32.h/.cc
 * uint64 matrix	octave_uint64_matrix/uint64NDArray	ov-uint64.h/.cc
 * 
 * sparse bool matrix	octave_sparse_bool_matrix/SparseBoolMatrix
 * sparse matrix	octave_sparse_matrix/SparseMatrix	ov-*-sparse.h
 * sparse complex matrix octave_sparse_complex_matrix/SparseComplexMatrix
 *
 * struct			octave_struct		ov-struct, ov-base
 * list				octave_list		-
 * cs-list			octave_cs_list		-
 * magic-colon			octave_magic_colon	-
 * built-in function		octave_builtin		-
 * built-in mapper function	octave_mapper		-
 * user-defined function	octave_user_function	-
 * dynamically-linked function	octave_dld_function	-
 * function handle		octave_fcn_handle	-
 * inline function		octave_fcn_inline	-
 * streamoff			octave_streamoff	-
 * overloaded function		(typeinfo)
 * ---------------------------------------------------------------- */


/*----------------------------------*/		/* forward declaration */
int pack_class(const octave_value &ov,		/* for recursive descend */
	       const std::string&name);		/* along the datatype */
/*----------------------------------*/


/* ----------------------------------------------------------------
 * In pack_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:	return pack_cell   (ov.cell_value   ());
 *    case ov_scalar:   return pack_scalar (ov.scalar_value ());
 *    ...
 * }
 *
 * first we will define all those pack_CASE(ov.access_value()) functions
 * ---------------------------------------------------------------- */


/*---------------
   1         cell           use MPI_Pack	\n\
 *---------------*/
/*----------------------------*/	// to pack a cell
int pack_cell(const Cell &cell){	// we first store nelems and then
/*----------------------------*/	// recursively the elmnts themselves
  intDEX n = cell.capacity();
  int info;
  if((info=MPI_Pack(&n,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info);

  for (intDEX i=0; i<n; i++)
    if((info=pack_class(cell(i),""))) return(info);	// recursive descend

  return  (MPI_SUCCESS);
}



/* ----------------------------------------------------------------
 * all scalars packed the same way, changing type -> make it a macro.
 * even though macro(3 arg), always define fun_CASE(1 arg ov.XXX_value()) 
 * fun suffix _CASE determines fun argtype(OCT_TYP) _and_ MPI_type
 * in general, argtype is NOT a C type, but an object
 * ----------------------------------------------------------------
 * BUG: should call get_MPI_Type instead of re-programming MPI_TYPE again
 * RATIONALE: any?
 * BUG: ABUSE!!! protected scalar attributes (should use iptr() ?!?)
 * RATIONALE: anyways, Complex has no iptr(), so what's the complaint
 * ---------------------------------------------------------------- */
#define PACK_SCALAR(CASE,OCT_TYP,MPI_TYP)	\
/*-----------------------------------*/					\
int pack_##CASE(const OCT_TYP &scalar){					\
/*-----------------------------------*/					\
  int info;    void*p = & ( const_cast<OCT_TYP &>(scalar) );		\
  if((info=MPI_Pack(p,	1,MPI_TYP,	BUF,SIZ,&IDX,COM)))	return(info);\
  return(MPI_SUCCESS);							\
}
/*---------------
   2         scalar         MPI_DOUBLE		\n\
   3 complex scalar         MPI_DOUBLE_COMPLEX	\n\
   7         bool           MPI_CXX_BOOL	\n\
 *---------------*/
  PACK_SCALAR(scalar ,double ,MPI_DOUBLE        )
  PACK_SCALAR(complex,Complex,MPI_DOUBLE_COMPLEX)
  PACK_SCALAR(bool   ,bool   ,MPI_CXX_BOOL      )
/*---------------
  12    int8                MPI_BYTE		\n\
  13   int16                MPI_SHORT		\n\
  14   int32                MPI_INT		\n\
  15   int64                MPI_LONG_LONG	\n\
  16   uint8                MPI_UNSIGNED_CHAR	\n\
  17  uint16                MPI_UNSIGNED_SHORT	\n\
  18  uint32                MPI_UNSIGNED	\n\
  19  uint64                MPI_UNSIGNED_LONG_LONG\n\
 *---------------*/
  PACK_SCALAR(i8 ,  octave_int8 , MPI_BYTE		)
  PACK_SCALAR(i16,  octave_int16, MPI_SHORT		)
  PACK_SCALAR(i32,  octave_int32, MPI_INT		)
  PACK_SCALAR(i64,  octave_int64, MPI_LONG_LONG		)
  PACK_SCALAR(u8 , octave_uint8 , MPI_UNSIGNED_CHAR	)
  PACK_SCALAR(u16, octave_uint16, MPI_UNSIGNED_SHORT	)
  PACK_SCALAR(u32, octave_uint32, MPI_UNSIGNED		)
  PACK_SCALAR(u64, octave_uint64, MPI_UNSIGNED_LONG_LONG)



/* ----------------------------------------------------------------
 * all arrays packed the same way, changing type -> make it a macro.
 * ----------------------------------------------------------------
 * WRT qdirt: LAM raises MPI_ERR_ARG if IDX@EOBUF even for n=0
 * ---------------------------------------------------------------- */
#define PACK_MATRIX(CASE,OCT_TYP,MPI_TYP)	\
/*----------------------------------*/					\
int pack_##CASE(const OCT_TYP &array){					\
/*----------------------------------*/					\
  int info;   intDEX n = array.capacity();				\
  if((info=MPI_Pack(&n,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info); \
		void*p = array.mex_get_data();				\
  bool qdirt = ( (n==0) && (IDX==SIZ) ? SIZ++, true : false );		\
  if((info=MPI_Pack( p,	n,MPI_TYP,	BUF,SIZ,&IDX,COM)))	return(info); \
					SIZ-=qdirt;			\
  return  (MPI_SUCCESS);						\
}
/*---------------
   4         matrix/array   MPI_DOUBLE * n	\n\
   5 complex matrix/array   MPI_DOUBLE_COMPLEX * n\n\
   8    bool matrix/array   MPI_CXX_BOOL*n	\n\
 *---------------*/
  PACK_MATRIX(sc_mat,       NDArray,MPI_DOUBLE        )
  PACK_MATRIX(cx_mat,ComplexNDArray,MPI_DOUBLE_COMPLEX)
  PACK_MATRIX(bl_mat,   boolNDArray,MPI_CXX_BOOL      )
/*---------------
   9    char matrix/array   MPI_CHAR * n	\n\
  10    (dq_)string         MPI_CHAR * n        \n\
  11      sq_string         MPI_CHAR * n        \n\
 *---------------*/
  PACK_MATRIX(ch_mat,   charNDArray,MPI_CHAR)	// can't tell difference
//int pack_string(const charNDArray &array) {return(pack_ch_mat(array));}
//int pack_sq_str(const charNDArray &array) {return(pack_ch_mat(array));}
/*---------------
  20    int8 array          MPI_BYTE		* n \n\
  21   int16 array          MPI_SHORT		* n \n\
  22   int32 array          MPI_INT		* n \n\
  23   int64 array          MPI_LONG_LONG	* n \n\
  24   uint8 array          MPI_UNSIGNED_CHAR	* n \n\
  25  uint16 array          MPI_UNSIGNED_SHORT	* n \n\
  26  uint32 array          MPI_UNSIGNED	* n \n\
  27  uint64 array          MPI_UNSIGNED_LONG_LONG * n \n\
 *---------------*/
  PACK_MATRIX( i8_mat,  int8NDArray , MPI_BYTE		)
  PACK_MATRIX(i16_mat,  int16NDArray, MPI_SHORT		)
  PACK_MATRIX(i32_mat,  int32NDArray, MPI_INT		)
  PACK_MATRIX(i64_mat,  int64NDArray, MPI_LONG_LONG	)
  PACK_MATRIX( u8_mat, uint8NDArray , MPI_UNSIGNED_CHAR	)
  PACK_MATRIX(u16_mat, uint16NDArray, MPI_UNSIGNED_SHORT)
  PACK_MATRIX(u32_mat, uint32NDArray, MPI_UNSIGNED	)
  PACK_MATRIX(u64_mat, uint64NDArray, MPI_UNSIGNED_LONG_LONG)



/*---------------
   6         range          MPI_DOUBLE * 3 / DBL*3+IDX using MPI_Pack()	\n\
 *---------------*/
/*-------------------------------*/	// to pack a range
int pack_range(const Range &range){	// put base,limit,incr,nelem
/*-------------------------------*/	// just 3 doubles + 1 intDEX
  double*d = (double*) &range;		// quick & dirty way, instead of
  intDEX*i = (intDEX*) & d[3];		// const_cast<double*>(reinterpret_cast
  int info;				//     <const double*>(&range))
  if((info=MPI_Pack( d,	3,MPI_DOUBLE,	BUF,SIZ,&IDX,COM)))	return(info);
  if((info=MPI_Pack( i,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info);
  return  (MPI_SUCCESS);
}



/* ----------------------------------------------------------------
 * all sparses packed the same way, changing type -> make it a macro.
 * ---------------------------------------------------------------- */
#define PACK_SPARSE(CASE,OCT_TYP,MPI_TYP)	\
/*-----------------------------------*/					\
int pack_##CASE(const OCT_TYP &sparse){					\
/*-----------------------------------*/					\
  int info;   intDEX n = sparse.capacity(),				\
		     c = sparse.columns ();				\
  if((info=MPI_Pack(&n,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info); \
  if((info=MPI_Pack(&c,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info); \
									\
		void*dp= sparse.mex_get_data(),				\
		    *rp= sparse.mex_get_ir  (),				\
		    *cp= sparse.mex_get_jc  ();				\
  if((info=MPI_Pack(dp,n  ,MPI_TYP,	BUF,SIZ,&IDX,COM)))	return(info); \
  if((info=MPI_Pack(rp,n  ,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info); \
  if((info=MPI_Pack(cp,c+1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info); \
									\
  return  (MPI_SUCCESS);						\
}
/*---------------
  28 sparse bool matrix     MPI_CXX_BOOL       * nzmax  \n\
  29 sparse matrix          MPI_DOUBLE         * nzmax  \n\
  30 sparse complex matrix  MPI_DOUBLE_COMPLEX * nzmax  \n\
 *---------------*/
  PACK_SPARSE(bl_spr,   SparseBoolMatrix, MPI_CXX_BOOL      )
  PACK_SPARSE(sc_spr,       SparseMatrix, MPI_DOUBLE        )
  PACK_SPARSE(cx_spr,SparseComplexMatrix, MPI_DOUBLE_COMPLEX)



/*---------------
  31         struct         use MPI_Pack	\n\
 *---------------*/
/*-----------------------------------*/	// to pack a struct, store nkeys,
int pack_struct(const Octave_map &map){	// keynam-strlength 1st, store
/*-----------------------------------*/	// recursively contents themselves
  int info;   intDEX n = map.nfields();	// n#fields in each elemt
  if((info=MPI_Pack(&n,	1,MPI_INTDEX,	BUF,SIZ,&IDX,COM)))	return(info);
/* ----------------------------------------------------------------
 * pack newly added key_list -> to keep field order listing
 * additional advantage: constructor map(dims,key_list)and then assign/contents
 * cannot iterate through private: key_list, no public begin()/end() methods
 * not using string_vector key_list = map.keys(); but its c_str_vec() directly
 * ---------------------------------------------------------------- */
      char**  csv=map.keys().c_str_vec(); intDEX i=0;
  for(char**p=csv;   *p!=0; p++, i++){	// strlen() is size_t
  intDEX len = strlen(*p)+1;		// fieldname len, works for len==0
    if((info=MPI_Pack(&len,1,MPI_INTDEX,BUF,SIZ,&IDX,COM)))	return(info);
    if((info=MPI_Pack(*p,len,MPI_CHAR,	BUF,SIZ,&IDX,COM)))	return(info);
//  printf("packed fieldname: %s\n",*p);
  }		  map.keys().delete_c_str_vec(csv);
  if (i != n){				// double-check:
    error(STRNGFY(NAME) ": inconsistent map length\n");	return(MPI_ERR_ARG);
  }					// iterated through length() fields

  dim_vector map_dims= map.dims();	// struct array dimensions (ND)
  Octave_map::				// each key stores ND field-values
  const_iterator   p = map.begin();	// iterate through keys(fnames)
  for	     (n=0; p!= map.end();   p++, n++){
//   std::string key = map.key     (p);	// don't pack field name (key) again
//   printf("iterated fieldname= %s\n",	// trust on same key ordering when
//			  key.c_str());	// iterating map.begin/end() on _Unpack
   Cell	conts	     = map.contents(p);	// Cell w/ND contents
    if (conts.dims()!= map_dims){	// double-check:
      error(STRNGFY(NAME) ": inconsistent map dims\n");	return(MPI_ERR_ARG);
    }					// every elemt should have same ND
    if((info=pack_class(conts,"")))	return(info);
  }					// recursively pack cell contents

  if (n != map.nfields()){		// final double-check:
    error(STRNGFY(NAME) ": inconsistent map length\n");	return(MPI_ERR_ARG);
  }					// iterated through nfields()
  return(MPI_SUCCESS);
}



/*---------------
  32 list                       unsupported	\n\
  33 cs-list                    unsupported	\n\
  34 magic-colon                unsupported	\n\
  35 built-in function          unsupported	\n\
  36 built-in mapper function   unsupported	\n\
  37 user-defined function      unsupported	\n\
  38 dynamically-linked functionunsupported	\n\
  39 function handle		unsupported	\n\
  40 inline function		unsupported	\n\
  41 streamoff			unsupported	\n\
  42 overloaded function        unsupported     \
 *---------------*/



/* ----------------------------------------------------------------
 * In pack_class() there is a long
 *
 *  switch (ov.type_id()) {
 *  //case ov_unknown:
 *    case ov_cell:	return pack_cell   (ov.cell_value   ());
 *    case ov_scalar:   return pack_scalar (ov.scalar_value ());
 *    ...
 * }
 *
 * first we defined all those pack_CASE(ov.access_value()) functions
 * now we define this handy symbol to save typing all those cases
 * fortunately, we defined enum typeids and pack_* to share names and be short
 * 	so we can now write ov_##NAME ... pack_##NAME
 * unfortunately, octave access-function names are too long and most times
 * we didn't keep a matching enum/pack_xxx name -- way too long
 *	can't write ov. ##NAME ## _value()
 * ---------------------------------------------------------------- */
#define CASE_PACK(TID_SUFFIX,OV_ACCFUN)	\
case ov_##TID_SUFFIX:	return pack_##TID_SUFFIX(ov.OV_ACCFUN());



/*-----------------------------------*/	// to pack any octave_value
int pack_class(const octave_value &ov,	// store type_id (can't be 0)
	       const std::string&name){	// varname-strlength 1st, dims[ndim]
/*-----------------------------------*/	// and then appropriate specific info
    int t_id =ov.type_id();
  if (	t_id< ov_MIN_SUPP		// base==0, cell==1==MIN_SUPP
//  ||	t_id==ov_range			// ov_range==6 can be packed
    ||	t_id> ov_MAX_SUPP		//        struc==31==MAX_SUPP
	){				// list==32, ...strmoff==41==MAX_TYPE
	warning_with_id("MPITB:Pack-invalid-type",
	STRNGFY(NAME) ": unsupported datatype:[%s], class:[%s]",
	    ov.type_name ().c_str(),	// both type & class?
	    ov.class_name().c_str());	// does c_str() work?
//	return (MPI_ERR_TYPENOTSUP);	// later RET_1_ARG(info=pack..)
	return (MPI_ERR_TYPE);		// TYPENOTSUP does not exist in OpenMPI
  }
  int info;
  if((info=MPI_Pack(&t_id,  1,MPI_INT,	BUF,SIZ,&IDX,COM)))	return(info);

  int len = name.length()+1;		// works for length()==0
  if((info=MPI_Pack( &len,  1,MPI_INT,	BUF,SIZ,&IDX,COM)))	return(info);
  if((info=MPI_Pack(const_cast<char*>( name.c_str() ),
			  len,MPI_CHAR,	BUF,SIZ,&IDX,COM)))	return(info);

  int	  ndims=ov.ndims();
dim_vector dimv=ov. dims();
    intDEX dims   [ndims];			// there we go, static dims[]
  for (int i=0;  i<ndims; i++) dims[i]=dimv(i);
  if((info=MPI_Pack(&ndims,    1,MPI_INT   ,BUF,SIZ,&IDX,COM)))	return(info);
  if((info=MPI_Pack(  dims,ndims,MPI_INTDEX,BUF,SIZ,&IDX,COM)))	return(info);

  switch (t_id) {
//  case ov_unknown:
//  case   ov_cell: return pack_cell(ov.cell_value());
    CASE_PACK(cell   ,         cell_value)

    CASE_PACK(scalar ,       scalar_value)
    CASE_PACK(complex,      complex_value)
    CASE_PACK(sc_mat ,        array_value)
    CASE_PACK(cx_mat ,complex_array_value)

    CASE_PACK(range  ,        range_value)

    CASE_PACK(bool   ,         bool_value)
    CASE_PACK(bl_mat ,   bool_array_value)

    case ov_string:
    case ov_sq_str:
//  case   ov_ch_mat:	 return pack_ch_mat(ov.char_array_value());
    CASE_PACK(ch_mat ,   char_array_value)

    CASE_PACK( i8    ,  int8_scalar_value)
    CASE_PACK(i16    , int16_scalar_value)
    CASE_PACK(i32    , int32_scalar_value)
    CASE_PACK(i64    , int64_scalar_value)
    CASE_PACK( u8    , uint8_scalar_value)
    CASE_PACK(u16    ,uint16_scalar_value)
    CASE_PACK(u32    ,uint32_scalar_value)
    CASE_PACK(u64    ,uint64_scalar_value)

    CASE_PACK( i8_mat,   int8_array_value)
    CASE_PACK(i16_mat,  int16_array_value)
    CASE_PACK(i32_mat,  int32_array_value)
    CASE_PACK(i64_mat,  int64_array_value)
    CASE_PACK( u8_mat,  uint8_array_value)
    CASE_PACK(u16_mat, uint16_array_value)
    CASE_PACK(u32_mat, uint32_array_value)
    CASE_PACK(u64_mat, uint64_array_value)

    CASE_PACK(bl_spr ,    sparse_bool_matrix_value)
    CASE_PACK(sc_spr ,         sparse_matrix_value)
    CASE_PACK(cx_spr , sparse_complex_matrix_value)


    CASE_PACK(struct ,          map_value)

//  Should not reach here, see if(t_id){} at the beginning
//  case ov_list: case ov_cs_list:
//  case ov_magic_colon: case ov_builtin: case ov_mapper: case ov_user_func:
//  case ov_dld_func: case ov_fcn_handle: case ov_fcn_inline: case ov_streamoff:
//  case ov_unknown:
//  default:		return MPI_ERR_TYPE;	// unreached below?
  }
	error(STRNGFY(NAME) ": internal error datatype: %s, class: %s",
		ov.type_name ().c_str(),	// both type & class?
		ov.class_name().c_str());	// does c_str() work?
	return (MPI_ERR_TYPE);			// later RET_1_ARG(info=pack..)
}


/*--------------------------------------------------------------------------*/
/* MPI_Pack() --------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
DEFUN_DLD(NAME, args, nargout,
"MPI_Pack               Pack an Octave variable into contiguous memory\n\
\n\
  info = MPI_Pack (var, buf, pos, comm)\n\
\n\
  var        Octave variable to pack\n\
  buf  (var) Octave variable used as buffer\n\
  pos  (int) current position in buffer, in bytes\n\
               MPI_Pack starts packing from that position on\n\
               pos is incremented, can be reused on succesive packs\n\
  comm (int) communicator for packed message (handle)\n\
\n\
  info (int) return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator ('NULL'?)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (N/A under Octave)\n\
      2 MPI_ERR_COUNT  Invalid count argument    (N/A under Octave)\n\
     13 MPI_ERR_ARG    Invalid argument (typically a NULL pointer?)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
     15 MPI_ERR_TRUNCATE       Message  truncated on receive (?!?)\n\
\n\
  SEE ALSO: MPI_Pack_size, MPI_Unpack\n\
\n\
  Under MPITB, packing is the only option for sending\n\
  structured-type vars like cells and structs.\n\
  Correspondence between Octave <-> MPI datatypes:\n\
\n\
t_id, Octave type_name, and corresponding MPI type\n\
   1         cell           use MPI_Pack	\n\
\n\
   2         scalar         MPI_DOUBLE		\n\
   3 complex scalar         MPI_DOUBLE_COMPLEX	\n\
   4         matrix/array   MPI_DOUBLE * n	\n\
   5 complex matrix/array   MPI_DOUBLE_COMPLEX * n\n\
   6         range          MPI_DOUBLE * 3 / DBL*3+IDX using MPI_Pack()\n\
   7         bool           MPI_CXX_BOOL	\n\
   8    bool matrix/array   MPI_CXX_BOOL * n	\n\
   9    char matrix/array   MPI_CHAR * n	\n\
  10    (dq_)string         MPI_CHAR * n	\n\
  11      sq_string         MPI_CHAR * n	\n\
\n\
  12    int8                MPI_BYTE		\n\
  13   int16                MPI_SHORT		\n\
  14   int32                MPI_INT		\n\
  15   int64                MPI_LONG_LONG	\n\
  16   uint8                MPI_UNSIGNED_CHAR	\n\
  17  uint16                MPI_UNSIGNED_SHORT	\n\
  18  uint32                MPI_UNSIGNED	\n\
  19  uint64                MPI_UNSIGNED_LONG_LONG\n\
\n\
  20    int8 array          MPI_BYTE		* n \n\
  21   int16 array          MPI_SHORT		* n \n\
  22   int32 array          MPI_INT		* n \n\
  23   int64 array          MPI_LONG_LONG	* n \n\
  24   uint8 array          MPI_UNSIGNED_CHAR	* n \n\
  25  uint16 array          MPI_UNSIGNED_SHORT	* n \n\
  26  uint32 array          MPI_UNSIGNED	* n \n\
  27  uint64 array          MPI_UNSIGNED_LONG_LONG * n \n\
\n\
  28 sparse bool matrix     MPI_CXX_BOOL       * nzmax	\n\
  29 sparse matrix          MPI_DOUBLE         * nzmax	\n\
  30 sparse complex matrix  MPI_DOUBLE_COMPLEX * nzmax	\n\
\n\
  31         struct         use MPI_Pack	\n\
\n\
  32 list                       unsupported	\n\
  33 cs-list                    unsupported	\n\
  34 magic-colon                unsupported	\n\
  35 built-in function          unsupported	\n\
  36 built-in mapper function   unsupported	\n\
  37 user-defined function      unsupported	\n\
  38 dynamically-linked functionunsupported	\n\
  39 function handle		unsupported	\n\
  40 inline function		unsupported	\n\
  41 streamoff			unsupported	\n\
  42 overloaded function	unsupported	\n\
\n\
")
{
	NARGCHK    (NAME,4)
/* ----------------------------------------------------------------
 * info = MPI_Pack(var, buf, pos, comm)
 * first two must be (named) variables, 3rd must be updated if it's a var
 * ---------------------------------------------------------------- */
	BLCK_ONESYM(NAME,0,var)		// varnam, varsym
	BLCK_ONESYM(NAME,1,buf)		// bufnam, bufsym
	BLCK_NULSYM(NAME,2,pos)		// no NULL-check for possym
	BLCK_ONEINT(NAME,2,pos)		// pos -> IDX (global scope)
	BLCK_ONECOM(NAME,3)		// comm-> COM (global scope)

	BUF = get_MPI_Buff (bufsym, SIZ, true,	// SIZ in bytes
				 TYPE, true);	// buffer MK_UNIQ
	IDX = pos ;				// it will be overwritten
	COM = comm;

  int info = pack_class ( args(0), varnam );

  if (possym.is_defined()) {
	   possym.varref() = octave_value(IDX);	// MPI updates pos, so do we
  }
	RET_1_ARG (info)
}
