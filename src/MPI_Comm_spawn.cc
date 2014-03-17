/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_spawn
/*
 * ----------------------------------------------------
 * Spawn a dynamic MPI process
 * [info children errs] = MPI_Comm_spawn (prog, args, nproc, inf, root, parents)
 * ----------------------------------------------------
 */
#include "mpitb.h"	// buildblocks NARGCHK, etc
#include <ov-cell.h>	// avoid errmsg "cell -- incomplete datatype"
#include <quit.h>	// OCTAVE_QUIT

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_spawn         Spawn a dynamic MPI process\n\
\n\
  [info children errs] = MPI_Comm_spawn (prog, args, nproc, inf, root, parents)\n\
\n\
  prog    (str) Name of program to spawn (only significant at root)\n\
  args(cellstr) arguments to command     (only significant at root)\n\
  nproc   (int) number of processes to start (only significant at root)\n\
  inf     (ptr) startup hints - MPI_Info object handle (or MPI_INFO_NULL)\n\
  root    (int) rank of process to perform the spawn (the one reading args)\n\
                other ranks in 'parents' only notice 'root' and do so nothing\n\
  parents (ptr) parent intracommunicator (handle)\n\
\n\
  children(ptr) child intercommunicator containing spawned processes\n\
                (local group->parents) (remote group->children)\n\
  errs (intvec) return codes, one code per process\n\
\n\
  info (int)    global return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
     27 MPI_ERR_SPAWN  Spawn error - Check the returned error code vector\n\
     13 MPI_ERR_ARG    Invalid argument\n\
      8 MPI_ERR_ROOT   Invalid root (0..Comm_size-1)\n\
     16 MPI_ERR_OTHER  Other error - use MPI_Error_string for more info\n\
     17 MPI_ERR_INTERN This is fatal - send a bug report to LAM\n\
     35 MPI_ERR_NO_MEM free space is exhausted\n\
\n\
  SEE ALSO: MPI_Comm_get_parent, MPI_Intercomm_merge, MPI_Comm_spawn_multiple,\n\
            MPI_Info_create, MPI_Info_set, MPI_Info_delete, MPI_Info_free,\n\
            MPI_Init, lamboot, mpirun\n\
            process\n\
\n\
  This function is a collective operation over the parent comm\n\
  The processes must begin by calling MPI_Init, after which the predefined\n\
  communicator, MPI_COMM_WORLD, may be used. This world communicator\n\
  contains only the child processes. It is distinct from the\n\
  MPI_COMM_WORLD of the parent processes.\n\
\n\
  Both groups can decide to merge the intercommunicator into an\n\
  intracommunicator (with the MPI_Intercomm_merge function) and take\n\
  advantage of other MPI collective operations. They can then use the\n\
  merged intracommunicator to create new communicators and reach other\n\
  processes in the MPI application.\n\
\n\
  MPI_Comm_spawn_multiple  is used to manually specify a group of different\n\
  executables and arguments to  spawn.   MPI_Comm_spawn  is  used  to\n\
  specify  one  executable  and set of arguments (although a LAM/MPI app-\n\
  schema(5) can be provided to MPI_Comm_spawn via the 'lam_file' info key\n\
\n\
  See the LAM man page for MPI_Comm_spawn for more information\n\
\n\
")
{					// instead of expanding NARGCHK()
    int nargin = args.length();		// rather unusual args, better handmade
/*
 * [info children errs] = MPI_Comm_spawn (prog, args, nproc, inf, root, parents)
 */
    if((nargin !=  6)
	|| (! args(0).is_string     () )
	|| (! args(1).is_cell       () )
	|| (! args(2).is_real_scalar() )
	|| (! args(4).is_real_scalar() )
//	|| (! args(3).is_real_scalar() )
//	|| (! args(5).is_real_scalar())){
	|| (! MPITB_isOPtrTyp (args(3)))
	|| (! MPITB_isOPtrTyp (args(5)))){
	error(STRNGFY(NAME) ": required prog(str), args(cellstr), "
		"nproc(int), inf(ptr), root(int), parents(ptr)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
    }

//  char*  prog    = const_cast<char*>(		// c_str() didn't work here
//		     args(0).string_value().c_str() );	// prog corrupted later

   std::string str = args(0).string_value();
    int        len = str.length();
    char*     prog = static_cast<char*>(
	    calloc  (len+1,sizeof(char)) );	// calloc -> free
     str.copy(prog,  std::string::npos);	// c_str() didn't work here
	      prog  [len] = 0;			// so we must copy prog

    Cell   argcell = args(1).  cell_value();
    int    nproc   = args(2).   int_value();
    int    root    = args(4).   int_value();
  MPI_Info inf     = MPITB_ptrcast(MPI_Info, args(3));
  MPI_Comm parents = MPITB_ptrcast(MPI_Comm, args(5));

    if (argcell.rows() > 1){
	error(STRNGFY(NAME) ": required arg#2: args(cellstr 1xN)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
    }
    int argc = argcell.cols();			// prog corrupted here
    for (int i=0; i<argc; i++){			// if we used c_str()
      if (! argcell(0,i).is_string() ){		// instead of str.copy()
	error(STRNGFY(NAME) ": required arg#2: args(cell of strings)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
      }
    }

    char* argv [argc+1];
	  argv [argc  ] = 0;			// NULL-terminated argv
    for (int i=0; i<argc; i++){
	std::string str = argcell(0,i).string_value();
	int         len = str.length();
	   argv[i]=static_cast<char*>(
		  calloc(len+1,sizeof(char)) );	// calloc -> free
  str.copy(argv[i], std::string::npos);		// c_str() didn't work here
	   argv[i][len] = 0;			// so we must copy argv
    }
//  for (int i=0; i<argc; i++){			// c_str() repeated args
//	printf("argv[i] %s\n",argv[i]);		// corrupting ith when
//  }						// doing c_str() on i+1th

    MPI_Comm children;
    int errs [nproc];
//  OCTAVE_QUIT;				// might hang...
    int info = NAME    (prog, argv,     nproc,    inf,
			root, parents, &children, errs);

	Matrix m (1,nproc, MPI_SUCCESS);	// errs retcodes
    for (int i=0; i<nproc; i++)	m(0,i) = errs[i];
    for (int i=0; i<argc ; i++)	free    (argv[i]);
				free	(prog);

//  RET_3_ARG(info, (int)children, m)
    octave_value_list retval;
	   retval(2) =     m;
	   retval(1) =  MPITB_intcast(children);
	   retval(0) =  info;
    return retval;
}
