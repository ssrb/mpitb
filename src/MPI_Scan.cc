/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Scan
/*
 * ----------------------------------------------------
 * Computes the scan (partial reductions) of data on a collection of processes
 * info = MPI_Scan (svar, rvar, op, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_SROC

DEFUN_DLD(NAME, args, nargout,
"MPI_Scan               Computes partial reductions of data from lesser ranks\n\
\n\
  info = MPI_Scan (svar, rvar, op, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
  rvar  Octave variable used as red.dst (at each rank)\n\
  op   (int) MPI_MAX / _MIN / _SUM / _PROD (for all but 1B int/char/logical)\n\
             MPI_LAND / _LOR  / _LXOR      (for bools/ints except 1B int)\n\
             MPI_BAND / _BOR  / _BXOR      (for all ints except uint8)\n\
             MPI_REPLACE                   (for all, even for chars)\n\
             MPI_OP_NULL                   (invalid,     see below)\n\
\n\
  comm  (int)  communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
     10 MPI_ERR_OP     Invalid operation (must be one of the predefined)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Gather, MPI_Reduce\n\
            collective\n\
\n\
  MPI_Scan is a collective operation on comm (all ranks must call it)\n\
\n\
  MAXLOC & MINLOC operations are not Octave-compatible\n\
  char   types: string, char matrix (can  _REPLACE)\n\
  bool   types: bool,   bool matrix (can  _REPLACE, _LAND/_LOR/_LXOR)\n\
  1B int types: int8,   uint8       (can  _REPLACE, int8 can _BAND/OR/XOR too)\n\
  nB int types: [u]int16 / 32 / 64  (can  use all operators)\n\
  double types: scalar, complex,... (can  _REPLACE, _MAX, _MIN, _SUM, _PROD)\n\
                scalar & complex matrix, range\n\
		complex are treated as double[2],\n\
		ie, MPI_MAX would return [max(reals), max(imags)]\n\
		ranges are treated as double[4], not sure if that's a good idea\n\
		but it's coherent with get_MPI_Buff() used in MPI_Pack/Send\n\
\n\
  Global reduced value is left in last rank\n\
\n\
  The reduction functions ( MPI_Op ) do not return an error value.  As  a\n\
  result,  if  the  functions  detect an error, all they can do is either\n\
  call MPI_Abort or silently skip the problem.  Thus, if you  change  the\n\
  error  handler  from  MPI_ERRORS_ARE_FATAL  to  something  else  (e.g.,\n\
  MPI_ERRORS_RETURN ), then no error may be indicated.\n\
\n\
  The reason for this is the performance problems that arise in  ensuring\n\
  that all collective routines return the same error value.\n\
\n\
")

	PATN_SROC (NAME)	// SROC -> src/rcvar, op,comm

	/* info = MPI_Scan (svar, rvar, op, comm) */
