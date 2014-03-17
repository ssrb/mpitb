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
 * mytest: do something
 * ----------------------------------------------------
 */
#include "mpitb.h"		// buildblocks NARGCHK, etc

DEFUN_DLD(NAME, args, nargout,
"test: skeleton to test Octave API")
{
	NARGCHK(NAME,1)

  octave_value ov = args(0);
//---------------------------------------          dim_vector object layout
// dim_vector   dim_vector_rep  dim_array
// -----------  --------------  ---------
// | ------- |  |  --------  |  | rows  |
// | |*rep | |  |  |*dims |  |  | cols  |
// | ------- |  |  --------  |  | pages |
// |   ...   |  |  | ndims|  |  | ...   |
// ----------   |  --------  |  ---------
//              |  | count|  |
//              |  --------  |
//              --------------
//---------------------------------------          dim_vector object layout
	int ndims = ov.ndims();
dim_vector   dims = ov. dims();

  printf("ndims(): %d\n",ndims);
  printf(" dims[]: %d, %d...\n",dims(0),dims(1));

	int***dptr = (int***) &dims;	// dptr is location of "rep" in drawing
	int  *ptr1 = (int  *)  dptr;
  printf("dim_vector     @ %p: %p | %x %x %x %x -- %x %x %x %x\n",
			dptr   ,dptr[0],
			ptr1[0],ptr1[1],ptr1[2],ptr1[3],
			ptr1[4],ptr1[5],ptr1[6],ptr1[7]);

	int**rptr =       dptr[0];	// rptr is called      "rep" in drawing
	int *ptr2 = (int*)rptr;		// rptr is location of"dims" in drawing
	int *ptr3 =(int*)&rptr[1];	//	 location of "ndims" in drawing
  printf("dim_vector_rep @ %p: %p %d %d | %x %x %x %x -- %x %x %x %x\n",
			rptr   ,rptr[0],ptr3[0],ptr3[1],
			ptr2[0],ptr2[1],ptr2[2],ptr2[3],
			ptr2[4],ptr2[5],ptr2[6],ptr2[7]);

	int *ptr4 =       rptr[0];	// ptr4 is called     "dims" in drawing
	    ndims =       ptr3[0];	// ptr4 is location of"rows" in drawing
  printf("dims   [ ] @ %p: ",ptr4);
  for (int i=0; i<ndims; i++)
  printf("%d ", ptr4[i]);
  printf("\n");

	int***dimsptr = reinterpret_cast<int***>(&dims);
        int  *ndimptr = reinterpret_cast<int*>  (&(*dimsptr)[1]);
                        /* BUG: ABUSE!!! protected dim_vector::rep->dims */
        if ( (*ndimptr)    != ndims ||
            (**dimsptr)[0] != ov.rows() ||
            (**dimsptr)[1] != ov.columns() )
         printf("MPI_Pack_size: dim_vector representation has changed!!!\n");
	else
         printf("MPI_Pack_size: OK!!!\n");

  return octave_value();
}
