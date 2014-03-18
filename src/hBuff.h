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
 * Buffer Functions used elsewhere (basically get_MPI_Buff())
 * ----------------------------------------------------
 */
#include "hType.h"		// datatype enums for switch

/* ----------------------------------------------------------------
 * This define is handy to later choose to declare vars global/local
 * Compare MPI_Pack/Unpack to MPI_Send/Recv
 * ----------------------------------------------------------------
 * cross-references: used by:
 * MPI_Address.cc	MPI_Buffer_attach.cc	MPI_Buffer_detach.cc
 * MPI_Pack.cc		MPI_Unpack.cc
 * ----------------------------------------------------------------
 * hSndRecv.h (2 uses):
 *     BLCK_B2IC    -> used by PATN_F_B2IC/PATN_MAP_F_B2IC/PATN_REQ_F_B2IC
 *     BLCK_2_B2IC  -> used by PATN_MAP_F_2_B2IC [_RP]
 *     ie, used by all hSndRecv functions
 * ---------------------------------------------------------------- */

#define DECLARE_BUFF_VARS						\
    void	*BUF ;	/* buf arg, and its...			*/	\
    int		 SIZ ;	/* (Octave-declared) size		*/	\
    MPI_Datatype TYPE;	/* type ignored if want size "inbytes"	*/


/* ----------------------------------------------------------------
 * This define is just for MPI_Sendrecv
 * ----------------------------------------------------------------
 * cross-references: used by:
 * hSndRecv.h (1 uses):
 *     PATN_MAP_F_2_B2IC
 * ---------------------------------------------------------------- */

#define DECLARE_RECV_BUFF_VARS						\
    void	*RBUF ;	/* buf arg, and its...			*/	\
    int		 RSIZ ;	/* (Octave-declared) size		*/	\
    MPI_Datatype RTYPE;	/* type ignored if want size "inbytes"	*/



/* ----------------------------------------------------------------
 * void* get_MPI_Buff(symbol_record *varsym, int &SIZ, bool inbytes,
 *				MPI_Datatype &TYPE, bool mk_uniq=false){
 * aux function to get storage pointer given arg name (not oct_value)
 * SIZ/TYPE also updated, TYPE frequently ignored (when size in bytes)
 * typical use: assign returned val to declared BUF (see DECLARE_BUFF_VARS)
 * ----------------------------------------------------------------
 * cross-references: used by:
 * MPI_Address.cc	MPI_Buffer_attach.cc
 * MPI_Pack.cc		MPI_Unpack.cc
 * ----------------------------------------------------------------
 * hColl.h   (1 uses):
 *     BCNTYPE_GET -> used by BCDTYPE_GET ->
 *     ie, used by all hColl functions except for PATN_F_COM (MPI_Barrier)
 * hSndRecv.h(1 uses):
 *     BLCK_B2IC   -> used by PATN_F_B2IC/PATN_MAP_F_B2IC/PATN_REQ_F_B2IC
 * hRecv.h(2 uses):
 *     PATN_MAP_F_2_B2IC (for both send & recv buffers)
 *     ie, used by all hSndRecv functions
 * ---------------------------------------------------------------- */
#define CASE_UNIQUE_MAT(OV_T_ID, OV_T_NAM, OV_T_FUN)			\
case OV_T_ID: {	OV_T_NAM a = ov.OV_T_FUN();		a(0)=a(0);	\
		varsym.varref() = octave_value(a); }	break;

#define CASE_UNIQUE_SPR(OV_T_ID, OV_T_NAM, OV_T_FUN)			\
case OV_T_ID: {	OV_T_NAM a = ov.OV_T_FUN();	dummy = a.data();	\
		varsym.varref() = octave_value(a); }	break;

//		void *dummy __attribute__((unused))  =  a.data();

#define CASE_MATRIX_BUFF(OV_T_ID)					\
case OV_T_ID:	TYPE=get_MPI_Type(ov);					\
		SIZ = ( inbytes ? ov.byte_size() : ov.capacity());	\
		return		 (ov.mex_get_data());


#define CASE_SCALAR_BUFF(OV_T_ID)					\
	CASE_MATRIX_BUFF(OV_T_ID)	// capacity()==1 for a scalar
					// capacity()==nnz() for sparse
					// byte_sz includes ir&jc sizes
#define CASE_SPARSE_BUFF(OV_T_ID)					\
case OV_T_ID:	TYPE=get_MPI_Type(ov);					\
		SIZ = ( inbytes ? ov.byte_size()-sizeof(intDEX)		\
				*(ov.nzmax()+ov.columns()+1)		\
				: ov.capacity());			\
		return		 (ov.mex_get_data());

/* -------------------------------------- from Sparse.h (2.9.10) --
  size_t byte_size (void) const { return 
	(cols () + 1) *		     sizeof (octave_idx_type) +
	 capacity () * (sizeof (T) + sizeof (octave_idx_type)); }
 * ----------------------------------------------------------------
  // Note that nzmax and capacity are the amount of storage for
  // non-zero elements, while nnz is the actual number of non-zero
  // terms.
 * ----------------------------------------------------------------
  class OCTAVE_API SparseRep{
    public:
    T *d;			// d [nzmx]
    octave_idx_type *r;		// r [nzmx]
    octave_idx_type *c;		// c [ncols+1]
    octave_idx_type nzmx;
    octave_idx_type nrows;
    octave_idx_type ncols;
    int count;
 * ---------------------------------------------------------------- */

/*----------------------------------------------------------------*/
void* get_MPI_Buff(symbol_table::symbol_record &varsym, int &SIZ, bool inbytes,
				MPI_Datatype &TYPE, bool mk_uniq=false){
/*----------------------------------------------------------------*/
  void * dummy ;				// data() sideffect in CASE_SPR
  octave_value &ov = varsym.varref();		// fast return for invalid t_id
           int t_id = ov.type_id();		// see hType.h

	//switch (t_id){				// currently easier with if()
  if (  t_id<=ov_MIN_SUPP			// base==0, cell==1==MIN_SUPP
	//  ||	t_id==ov_range				// ov_range==6... range too?
						// MPITB_ST_offset due to range
    ||	t_id>=ov_MAX_SUPP			//        struc==31==MAX_SUPP
	){				// list==32, ...strmoff==41==MAX_TYPE
	warning_with_id("MPITB:getBuff-invalid-type",
		"get_MPI_Buff: invalid buffer datatype:[%s], class:[%s]",
		ov.type_name ().c_str(),	// both type & class?
		ov.class_name().c_str());	// does c_str() work?
	return (0);				// NULL ptr
  }
/* ----------------------------------------------------------------
 * See test-copies.cc: normally 2 copies: varsym->def() & args()
 * args(i) would construct another copy, lazy copies a=buff would incr count
 * also under functions
 * MPI might overwrite there if BUF is wanted for MPI_Recv/_Pack...
 * ----------------------------------------------------------------
 */
#define OV_LIMIT 2
   int ov_count = ov.get_count();

   if (ov_count > OV_LIMIT){
     if(mk_uniq){

     	ov.make_unique();

		warning_with_id("MPITB:getBuff-mk-unique",
			STRNGFY(NAME) "(%s...) -> %s.make_unique(): %d -> %d copies\n",
				varsym.name().c_str(),
				varsym.name().c_str(),	ov_count, ov.get_count());

		switch (t_id){
			CASE_UNIQUE_MAT(ov_sc_mat,       NDArray,           array_value)
			CASE_UNIQUE_MAT(ov_cx_mat, ComplexNDArray,  complex_array_value)
			CASE_UNIQUE_MAT(ov_bl_mat,    boolNDArray,     bool_array_value)
			CASE_UNIQUE_MAT(ov_ch_mat,    charNDArray,     char_array_value)
				case    ov_string:{   charNDArray a=ov.char_array_value();
					a(0)=a(0);  varsym.varref() = octave_value(a,true);}
				break;
				case    ov_sq_str:{   charNDArray a=ov.char_array_value();
					a(0)=a(0); varsym.varref() = octave_value(a,true,'\'');}
				break;

			CASE_UNIQUE_MAT(ov_i8_mat,   int8NDArray,     int8_array_value)
			CASE_UNIQUE_MAT(ov_i16_mat,  int16NDArray,    int16_array_value)
			CASE_UNIQUE_MAT(ov_i32_mat,  int32NDArray,    int32_array_value)
			CASE_UNIQUE_MAT(ov_i64_mat,  int64NDArray,    int64_array_value)
			CASE_UNIQUE_MAT( ov_u8_mat,  uint8NDArray,    uint8_array_value)
			CASE_UNIQUE_MAT(ov_u16_mat, uint16NDArray,   uint16_array_value)
			CASE_UNIQUE_MAT(ov_u32_mat, uint32NDArray,   uint32_array_value)
			CASE_UNIQUE_MAT(ov_u64_mat, uint64NDArray,   uint64_array_value)

		   CASE_UNIQUE_SPR(ov_bl_spr,    SparseBoolMatrix,    sparse_bool_matrix_value)
		   CASE_UNIQUE_SPR(ov_sc_spr,        SparseMatrix,         sparse_matrix_value)
		   CASE_UNIQUE_SPR(ov_cx_spr, SparseComplexMatrix, sparse_complex_matrix_value)
		}

     } else {
		warning_with_id("MPITB:getBuff-above-cnt",
			STRNGFY(NAME) "(%s...) -> there are %d copies of %s right now (much)",
				varsym.name().c_str(),	ov_count,
				varsym.name().c_str());
     }

  } else {
		//	message_with_id("name","MPITB:id",fmt...);	// m-filename ?!?
	   if (ov_count == OV_LIMIT){
			warning_with_id("MPITB:getBuff-normalcnt",
				STRNGFY(NAME) "(%s...) -> there are exactly %d copies of %s (fine)",
					varsym.name().c_str(), OV_LIMIT,
					varsym.name().c_str());
		} else if (ov_count < OV_LIMIT){				// should not happen
			warning_with_id("MPITB:getBuff-below-cnt",
				STRNGFY(NAME) "(%s...) -> there are %d copies of %s right now (few)\n"
				"\t This should never happen, at least copied as function(argument)",
					varsym.name().c_str(),	ov_count, varsym.name().c_str());
		}
	}

  switch (t_id) {
	//case		   ov_unknown-> default: error(); return(NIL); break;
	//case		   ov_cell   -> default:

	CASE_SCALAR_BUFF(ov_scalar )
	CASE_SCALAR_BUFF(ov_complex)

	CASE_MATRIX_BUFF(ov_sc_mat )
	CASE_MATRIX_BUFF(ov_cx_mat )


	  case ov_range:TYPE=get_MPI_Type(ov);
			SIZ = ( inbytes ? ov.byte_size() : 3 );
			return //(ov.mex_get_data());	// Ouch! not implemented
/*C-style cast!*/ (void*) & ( ((double*)ov.internal_rep()) [MPITB_ST_offset] );
/* - should be ----------------------------------------------------
	return static_cast<void*>( & (
			reinterpret_cast<double*>     (
			const_cast<octave_value*>(&ov) )	) [1] );
			// BUG: ABUSE!!! abusing private range attrs
 * ----------------------------------------------------------------
//  size_t byte_size (void) const { return 3 * sizeof (double); }
 * ---------- from src/ov-range.h and liboctave/Range.h (2.9.10) --
// private:
//  double rng_base;
//  double rng_limit;
//  double rng_inc;
//  ---------------
//  octave_idx_type rng_nelem;
//  mutable Matrix cache;
 * ---------------------------------------------------------------- */


	CASE_SCALAR_BUFF(ov_bool  )
	CASE_MATRIX_BUFF(ov_bl_mat)

	case ov_string:
	case ov_sq_str:
	CASE_MATRIX_BUFF(ov_ch_mat)		// Works? all three?

	CASE_SCALAR_BUFF(ov_i8 )	CASE_MATRIX_BUFF(ov_i8_mat )
	CASE_SCALAR_BUFF(ov_i16)	CASE_MATRIX_BUFF(ov_i16_mat)
	CASE_SCALAR_BUFF(ov_i32)	CASE_MATRIX_BUFF(ov_i32_mat)
	CASE_SCALAR_BUFF(ov_i64)	CASE_MATRIX_BUFF(ov_i64_mat)
	CASE_SCALAR_BUFF(ov_u8 )	CASE_MATRIX_BUFF(ov_u8_mat )
	CASE_SCALAR_BUFF(ov_u16)	CASE_MATRIX_BUFF(ov_u16_mat)
	CASE_SCALAR_BUFF(ov_u32)	CASE_MATRIX_BUFF(ov_u32_mat)
	CASE_SCALAR_BUFF(ov_u64)	CASE_MATRIX_BUFF(ov_u64_mat)

	CASE_SPARSE_BUFF(ov_bl_spr)
	CASE_SPARSE_BUFF(ov_sc_spr)
	CASE_SPARSE_BUFF(ov_cx_spr)

//case		ov_struct -> default: error(); return(NIL); break;
//case		ov_list,	ov_cs_list, 
//case		ov_magic_colon, ov_builtin, ov_mapper, ov_user_func,
//case		ov_dld_func, ov_fcn_handle, ov_fcn_inline, ov_streamoff,
//default:	return (0);	// unreached below?
	}

	error("get_MPI_Buff: internal error datatype:[%s], class:[%s]",
		ov.type_name ().c_str(),	// both type & class?
		ov.class_name().c_str());	// does c_str() work?

	return (0);				// NULL ptr
}

/* - Questions / MailList -----------------------------------------
 * ----------------------------------------------------------------
 * Sparse:
	byte_size only d[nzmx]
	rationale byte_size ranges = 3*double (not including nelem)
//  octave_idx_type rng_nelem;
	explanation: perhaps used for saving HDF5/mat format?
	perhaps for very very big arrays whos a Bytes Class
 * Sparse:
	assuming r[ncols+1] is same cols==columns=dim2()
 * ----------------------------------------------------------------
 * Range:
	capacity only 3 ... or perhaps cached zone
	mex_get_data defined? would be nice, but as_mxArray builds... what?
	also mutable Matrix cache; matrix_value returns a copy, not pointer
 * ----------------------------------------------------------------
   Struct:
	added key_list and iterators, but... begin() end() ?!?
	should we pack/send key_list? howto? see ov.map_keys()
 * ----------------------------------------------------------------
 * Copies:
	lazy copying: why 2 copies interactive (symbol table & arglist)
	but 3 copies inside a function? (what's fbi_symbol_table?)
  -	additionally, a=b=c, print_info(c) adds copies (see mytests)
  -	TODO: smarter MPI_Recv/get_MPI_Buff() who knows if 2 is ok, or 3 is ok
  -	which are the symbol tables? 
  -	TODO: allow MPI_Send(3.14,dst,tag,world), ie, not buff required
  -	ARGH!: NPB/EP problems with copies...
	difference between public ov.make_unique and private array.make_unique
	ov.get_count does not discover a deep copy of which
		somebody could have done ov.make_unique()
		but not deep a(0)=a(0) (NDArrays) or dum=data() (sparses)
 * ---------------------------------------------------------------- */

