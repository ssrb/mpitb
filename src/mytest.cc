/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  mytest
/*
 * ----------------------------------------------------
 * mytest-uniq: explore octave-value array representation / lazycopy
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc
#include "hType.h"		// type enums

DEFUN_DLD(NAME, args, nargout,
"test-uniq: explore octave-value array representation / lazycopy\n\
try with:\n\
	MPI_Init, A=A1=A2=A3=3\n\
	print_info(A), [info adr]=MPI_Address(A)\n\
	mytest(A)\n\
	print_info(A1), [info adr]=MPI_Address(A1)\n\
\n\
try with varying A type:\n\
 A=A1=A2=A3= 3		% scalar	 A=A1=A2=A3= 3i		% cplx scalar\n\
 A=A1=A2=A3=[3  4]	% scalar array	 A=A1=A2=A3=[3i 4]	% cplx array\n\
 A=A1=A2=A3= 1:10	% range\n\
 A=A1=A2=A3= true	% bool		 A=A1=A2=A3=[true false]% bool array\n\
 A=A1=A2=A3=[\"ab\";\"cd\"]\t% str\t	 A=A1=A2=A3=['ab';'cd']	% sngl-quot str\n\
 A=A1=A2=A3=int8( 3)	% int scalar	 A=A1=A2=A3=int8([3 4])	% int array\n\
\n\
 I=[1,1,2,30], J=[1,2,4,40], S=[1,2,3,4]\n\
 A=A1=A2=A3=sparse(I,J,S)	% scalar sparse\n\
 A3(1,2)+=i; A=A1=A2=A3		% complex sparse\n\
 A=A1=A2=A3=sparse(A~=0)	% bool sparse\n\
\n\
")
{
	NARGCHK    (NAME,1)
	BLCK_ONESYM(NAME,0,var)		// varnam, varsym
					// from hBuff.h
octave_value ov = varsym.varval();	// fast return for invalid t_id
	 int t_id = ov.type_id();	// see hType.h

//switch (t_id){			// currently easier with if()
  if (	t_id<=ov_MIN_SUPP		// base==0, cell==1==MIN_SUPP
    ||	t_id>=ov_MAX_SUPP		//        struc==31==MAX_SUPP
	){				// list==32, ...strmoff==41==MAX_TYPE
	printf("try supported types - not datatype:[%s], class:[%s]\n",
		ov.type_name ().c_str(),        // both type & class?
		ov.class_name().c_str());       // does c_str() work?
	RET_1_ARG()
  }

	ov.print_info(std::cout);

  int ov_count = ov.get_count();
  if (ov_count > 2){
	ov.make_unique();
	printf("ov.make_unique: %d -> %d copies\n",ov_count, ov.get_count());

    switch (t_id){
    case ov_scalar:
    case ov_complex:
    case ov_range:
    case ov_bool:
    case ov_i8: case ov_i16: case ov_i32: case ov_i64:
    case ov_u8: case ov_u16: case ov_u32: case ov_u64:
	printf("that should be it with scalars (& ranges)\n");	break;

    case ov_bl_spr: case ov_sc_spr: case ov_cx_spr:
	ov.print_info(std::cout);
	printf("Sparse<T>.make_unique rep->count 2->1\n");	break;

    default:
	ov.print_info(std::cout);
	printf("Array<T>.make_unique rep->count 2->1\n");	break;
    }


    switch (t_id){
case ov_sc_mat:{       NDArray a=ov.        array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_cx_mat:{ComplexNDArray a=ov.complex_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_bl_mat:{   boolNDArray a=ov.   bool_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_ch_mat:{   charNDArray a=ov.   char_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_string:{   charNDArray a=ov.   char_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a,true);} break;
case ov_sq_str:{   charNDArray a=ov.   char_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a,true,'\'');} break;
case ov_i8_mat:{   int8NDArray a=ov.   int8_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_i16_mat:{ int16NDArray a=ov.  int16_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_i32_mat:{ int32NDArray a=ov.  int32_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_i64_mat:{ int64NDArray a=ov.  int64_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_u8_mat:{  uint8NDArray a=ov.  uint8_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_u16_mat:{uint16NDArray a=ov. uint16_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_u32_mat:{uint32NDArray a=ov. uint32_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;
case ov_u64_mat:{uint64NDArray a=ov. uint64_array_value();	a(0)=a(0);
			varsym.varref() = octave_value(a);}	break;


case ov_bl_spr:{   SparseBoolMatrix a=ov.   sparse_bool_matrix_value();
			void  *d  __attribute__((unused))  =  a.data();
			varsym.varref() = octave_value(a);  } break;
case ov_sc_spr:{       SparseMatrix a=ov.        sparse_matrix_value();
			void  *d  __attribute__((unused))  =  a.data();
			varsym.varref() = octave_value(a);  } break;
case ov_cx_spr:{SparseComplexMatrix a=ov.sparse_complex_matrix_value();
			void  *d  __attribute__((unused))  =  a.data();
			varsym.varref() = octave_value(a);  } break;
    }


	ov.print_info(std::cout);
  }

	RET_1_ARG()
}

