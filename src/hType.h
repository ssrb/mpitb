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
 * Type Details: enums and get_MPI_Type() used in hBuff.h:get_MPI_Buff()
 * ----------------------------------------------------
 */
/* ----------------------------------------------------------------
 * isn't this enum anywhere in Octave's includes ?!? dynamically registered
 * stolen from src/ov.cc:install_types() and octave command "typeinfo"
 * ----------------------------------------------------------------
 * cross-references: used by:		by including:
 * 		MPI_Pack.cc:		hBuff.h
 * 		MPI_Pack_size.cc:	hType.h
 * 		MPI_Unpack.cc:		hBuff.h
 * 		hBuff.h:		hType.h
 * ---------------------------------------------------------------- */
  enum ov_t_id
{
  ov_unknown=0,
  ov_cell,		 ov_MIN_SUPP=ov_cell,
  ov_scalar, ov_complex,
  ov_sc_mat, ov_cx_mat,
  ov_range,
  ov_bool,   ov_bl_mat,
  ov_ch_mat, ov_string,  ov_sq_str,
  ov_i8,     ov_i16,     ov_i32,     ov_i64,
  ov_u8,     ov_u16,     ov_u32,     ov_u64,
  ov_i8_mat, ov_i16_mat, ov_i32_mat, ov_i64_mat,
  ov_u8_mat, ov_u16_mat, ov_u32_mat, ov_u64_mat,
  ov_bl_spr, ov_sc_spr,  ov_cx_spr,
  ov_struct,		 ov_MAX_SUPP=ov_struct,
  ov_list, ov_cs_list,
  ov_magic_colon, ov_builtin, ov_mapper, ov_user_func,
  ov_dld_func, ov_fcn_handle, ov_fcn_inline, ov_streamoff,
  ov_MAX_TYPE
};


/* ----------------------------------------------------------------
 * this datatype fits perfectly for Octave's C++ bool
 * and it's not part of the MPI std, but of the LAM implementation.
 * #define stolen from LAM mpisys.h
 * ----------------------------------------------------------------
 * cross-references: used by:		by including:
 * 		MPI_Pack.cc:		hBuff.h
 * 		MPI_Pack_size.cc:	hType.h
 * 		MPI_Unpack.cc:		hBuff.h
 * 		hBuff.h:		hType.h
 * ---------------------------------------------------------------- */

#ifndef MPI_CXX_BOOL			// from LAM mpisys.h C++ bindings
 #if      LAM_MPI
  #define MPI_CXX_BOOL    ((MPI_Datatype) &lam_mpi_cxx_bool)
 #elif    OPEN_MPI
  #define MPI_CXX_BOOL    ((MPI_Datatype) &ompi_mpi_cxx_bool)
//#define MPI_CXX_BOOL	MPI_BOOL
 #else
  #error No bool type known (this is neither LAM/ nor Open-MPI)
 #endif	/* LAM_MPI / OPEN_MPI */
#endif	/* ndef MPI_CXX_BOOL */


/* ----------------------------------------------------------------
 * aux function to obtain decided compatible MPI type for an octave_value
 * ----------------------------------------------------------------
 * cross-references: used by:		by including:
 *		MPI_Get_elements.cc:	hType.h
 *		MPI_Pack_size.cc:	hType.h
 *		hColl.h:		hBuff.h
 * ---------------------------------------------------------------- */

MPI_Datatype get_MPI_Type (const octave_value & ov){
    switch (ov.type_id()){
//	case ov_unknown-> default: return(MPI_PACKED);
//	case ov_cell   ->
	case ov_scalar: return MPI_DOUBLE;
	case ov_complex:return MPI_DOUBLE_COMPLEX;
	case ov_sc_mat: return MPI_DOUBLE;
	case ov_cx_mat: return MPI_DOUBLE_COMPLEX;
	case ov_range:  return MPI_DOUBLE;
	case ov_bool:   return MPI_CXX_BOOL;
	case ov_bl_mat: return MPI_CXX_BOOL;
	case ov_ch_mat: return MPI_CHAR;
	case ov_string: return MPI_CHAR;
	case ov_sq_str: return MPI_CHAR;

//	See liboctave/oct-types.h for octave_idx_type and [u]intXX_t typedefs
	case ov_i8:	case ov_i8_mat:  return MPI_BYTE;
	case ov_i16:	case ov_i16_mat: return MPI_SHORT;
	case ov_i32:	case ov_i32_mat: return MPI_INT;
	case ov_i64:	case ov_i64_mat: return MPI_LONG_LONG;
	case ov_u8:	case ov_u8_mat:  return MPI_UNSIGNED_CHAR;
	case ov_u16:	case ov_u16_mat: return MPI_UNSIGNED_SHORT;
	case ov_u32:	case ov_u32_mat: return MPI_UNSIGNED;
	case ov_u64:	case ov_u64_mat: return MPI_UNSIGNED_LONG_LONG;

	case ov_bl_spr: return MPI_CXX_BOOL;
	case ov_sc_spr: return MPI_DOUBLE;
	case ov_cx_spr: return MPI_DOUBLE_COMPLEX;

	case ov_struct: return MPI_PACKED;

//	case ov_list:		case ov_cs_list:
//	case ov_magic_colon:	case ov_builtin:
//	case ov_mapper:		case ov_user_func:
//	case ov_dld_func:	case ov_fcn_handle:
//	case ov_fcn_inline:	case ov_streamoff:
//	case ov_unknown:
//	default:	return MPI_PACKED
    }
			return MPI_PACKED;
}
