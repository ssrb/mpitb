/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Reduce
/*
 * ----------------------------------------------------
 * Reduces values on all processes to a single value
 * info = MPI_Reduce (svar, rvar, op, root, comm)
 * ----------------------------------------------------
 */
#include "mpitb.h"		// mpi.h, oct.h
#include "hColl.h"		// PATN_SRORC

DEFUN_DLD(NAME, args, nargout,
"MPI_Reduce             Reduces values on all processes to a single value\n\
\n\
  info = MPI_Reduce (svar, rvar, op, root, comm)\n\
\n\
  svar  Octave variable used as msg src (at each rank)\n\
  rvar  Octave variable used as reduce buffer (at root), meaningless at others\n\
  op   (int) MPI_MAX / _MIN / _SUM / _PROD (for all but 1B int/char/logical)\n\
             MPI_LAND / _LOR  / _LXOR      (for bools/ints except 1B int)\n\
             MPI_BAND / _BOR  / _BXOR      (for all ints except uint8)\n\
	     MPI_REPLACE                   (for all, even for chars)\n\
	     MPI_OP_NULL                   (invalid,     see below)\n\
\n\
  root  (int)  rank of root process (destination of reduce op)\n\
  comm  (ptr)  communicator (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator (NULL?)\n\
     16 MPI_ERR_OTHER  No collective implementation found (intercommunicator?)\n\
      2 MPI_ERR_COUNT  Invalid count argument (internal to .oct file)\n\
      3 MPI_ERR_TYPE   Invalid datatype argument (internal to .oct file)\n\
      1 MPI_ERR_BUFFER Invalid buffer pointer (NULL?)\n\
      8 MPI_ERR_ROOT   Invalid root rank (0..Comm_size-1)\n\
     10 MPI_ERR_OP     Invalid operation (must be one of the predefined)\n\
\n\
  SEE ALSO: MPI_Barrier, MPI_Bcast, MPI_Scatter, MPI_Gather\n\
            collective\n\
\n\
  MPI_Reduce is a collective operation on comm (all ranks must call it)\n\
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
		ranges are treated as double[3], not sure if that's a good idea\n\
		but it's coherent with get_MPI_Buff() used in MPI_Pack/Send\n\
\n\
  it is enough that meaningless arguments be the same type at non-root ranks\n\
  for instance, MPI_Reduce(S,[],MPI_MIN,0,NEWORLD) if scattering doubles\n\
\n\
  If there are 4 or less ranks involved, the root  loops  over  receiving\n\
  from each rank, and then performs the final reduction locally.\n\
\n\
  If there are more than 4 ranks involved, a tree-based algorithm is used\n\
  to collate the reduced data at the root (the data is  reduced  at  each\n\
  parent  in  the tree so that the reduction operations are actually\n\
  distributed).\n\
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

	PATN_SRORC (NAME)	// SRORC -> src/rcvar, op, root,comm

	/* info = MPI_Reduce (svar, rvar, op, root, comm) */
