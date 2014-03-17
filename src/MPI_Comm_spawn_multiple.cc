/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
#define   NAME  MPI_Comm_spawn_multiple
/*
 * ----------------------------------------------------
 * Spawn a dynamic MPI process from multiple executables
 * [info children errs] = MPI_Comm_spawn_multiple (cprg,carg,cnprc,cinf, ...
 *							root, parents)
 * ----------------------------------------------------
 */
#include "mpitb.h"	// buildblocks NARGCHK, etc
#include <ov-cell.h>	// avoid errmsg "cell -- incomplete datatype"
#include <quit.h>	// OCTAVE_QUIT

DEFUN_DLD(NAME, args, nargout,
"MPI_Comm_spawn_multiple	Spawn MPI processes from multiple executables\n\
\n\
  [info children errs] = MPI_Comm_spawn_multiple (cprg,carg,cnprc,cinf, ...\n\
							root, parents)\n\
  cprg(cellstr) commands to be executed (only significant at root)\n\
  carg(clclstr) arguments for commands  (only significant at root)\n\
  cnpr(cellint) number of processes for each program(only significant at root)\n\
  cinf(cellptr) startup hints for each command\n\
\n\
	All previous cell-arrays must be column-cells of same #rows\n\
\n\
  root    (int) rank of process to perform the spawn\n\
  parents (ptr) parent intracommunicator (handle)\n\
\n\
  children(ptr) child intercommunicator containing spawned processes\n\
		 (local group->parents) (remote group->children)\n\
  errs (intvec) return codes, one code per process (column vector)\n\
\n\
  info (int)	 global return code\n\
      0 MPI_SUCCESS	No error\n\
      5 MPI_ERR_COMM	Invalid communicator\n\
     27 MPI_ERR_SPAWN	Spawn error - Check the returned error code vector\n\
     13 MPI_ERR_ARG	Invalid argument\n\
      8 MPI_ERR_ROOT	Invalid root (0..Comm_size-1)\n\
     16 MPI_ERR_OTHER	Other error - use MPI_Error_string for more info\n\
     17 MPI_ERR_INTERN	This is fatal - send a bug report to LAM\n\
\n\
  SEE ALSO: MPI_Comm_get_parent, MPI_Intercomm_merge, MPI_Comm_spawn,\n\
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
  schema(5) can be provided to MPI_Comm_spawn via the `lam_file' info key\n\
\n\
  See the LAM man page for MPI_Comm_spawn_multiple for more information\n\
\n\
")
{					// instead of expanding NARGCHK()
    int nargin = args.length();		// rather unusual args, better handmade
    /*
     * [info children errs] = MPI_Comm_spawn_multiple(cprg,carg,cnprc,cinf,...
     *							root, parents)
     */
    if((nargin !=  6)
	|| (! args(0).is_cell       () )	/***************/
	|| (! args(1).is_cell       () )	/* N-ARG-CHECK */
	|| (! args(2).is_cell       () )	/***************/
	|| (! args(3).is_cell       () )
	|| (! args(4).is_real_scalar() )
	|| (! MPITB_isOPtrTyp(args(5)))){
//	|| (! args(5).is_real_scalar())){
	error(STRNGFY(NAME) ": required cprg(cellstr), carg(cell-cellstr), "
		"cnprc(cellint), cinf(cellptr), root(int), parents(ptr)");
	return octave_value(MPI_ERR_ARG);	// error returns nothing
    }

    const Cell	&cprg    = args(0).cell_value(),/***************/
    		&carg    = args(1).cell_value(),/* ARG-TYP-CHK */
    		&cnpr    = args(2).cell_value(),/***************/
    		&cinf    = args(3).cell_value();
    int   root           = args(4). int_value();
 MPI_Comm parents        = MPITB_ptrcast(MPI_Comm, args(5));

						/***************/
						/* DIMS-CHECK  */
						/***************/

    int   nprg = cprg.rows();			/* cprg: Nx1 */
    if          (cprg.cols() != 1){
	error(STRNGFY(NAME) ": required arg#1: cprg(cell Nx1)");
	return octave_value(MPI_ERR_ARG);
    }
						/* cprg,carg,cnpr,cinf: Nx1 */
    if (   (nprg != cprg.rows() )	|| (cprg.cols() != 1)
	|| (nprg != carg.rows() )	|| (carg.cols() != 1)
	|| (nprg != cnpr.rows() )	|| (cnpr.cols() != 1)
	|| (nprg != cinf.rows() )	|| (cinf.cols() != 1) ){
	error(STRNGFY(NAME) ": required equal-sized cells args#1-4: "
		"cprg, carg, cnprc, cinf (cell Nx1)");
	return octave_value(MPI_ERR_ARG);
    }
						/***************/
						/*CELL-TYPE-CHK*/
						/***************/
    int   errflg =  0;
    char* errstr = "";				/* cprg: cellstr */
    if (!errflg) for (int i=0; i<nprg; i++) if (! cprg(i,0).is_string() ){
      errstr=STRNGFY(NAME) ": required arg#1: cprg(cell of strings)";
      errflg= 1; break;
    }						/* carg: cell-cellstr */
    if (!errflg) for (int i=0; i<nprg; i++) if (! carg(i,0).is_cell  () ){
      errstr=STRNGFY(NAME) ": required arg#2: carg(cell of cells)";
      errflg= 2; break;
    }						/* cnpr: cellint */
    if (!errflg) for (int i=0; i<nprg; i++) if (! cnpr(i,0).is_real_scalar() ){
      errstr=STRNGFY(NAME) ": required arg#3: cnprc(cell of ints)";
      errflg= 3; break;
    }						/* cinf: cellptr */
    if (!errflg) for (int i=0; i<nprg; i++)//if (! cinf(i,0).is_real_scalar() ){
					     if (! MPITB_isOPtrTyp(cinf(i,0))){
      errstr=STRNGFY(NAME) ": required arg#4: cinf (cell of pointers)";
      errflg= 4; break;
    }
    if ( errflg){				/* any of the above fail */
	error(errstr);
	return octave_value(MPI_ERR_ARG);
    }
						/***************/
						/*ARGV-TYPE-CHK*/
						/***************/

    for (int i=0; i<nprg; i++){		/* (carg: cell of -) cells of strings */
      const Cell & arg_i = carg(i,0).cell_value();
      int         narg_i = arg_i.cols();
      if		  (arg_i.rows() > 1){	// allowed 0x0
	error(STRNGFY(NAME) ": arg#2: carg (cell of cells 1xM_n)");
	return octave_value(MPI_ERR_ARG);
      }
      for (int j=0; j<narg_i; j++){
	if (! arg_i(0,j).is_string()){
	error(STRNGFY(NAME) ": arg#2: "
			"carg (cell of cells of strings)");
	return octave_value(MPI_ERR_ARG);
	}
      }
    }
						/***************/
						/* CALLOC->MPI */
						/***************/
    
    char * prg [nprg];				/* cprg -> prg */
    for (int i=0; i<nprg; i++){
	std::string str = cprg(i,0).string_value();
	int         len = str.length();		// calloc -> free
	prg [i] = static_cast<char*>( calloc (len+1, sizeof(char)) );
	str.copy(prg[i], std::string::npos);	// see also MPI_Comm_spawn
		 prg[i][len] = 0;		// NULL-terminated C string
    }

    char** argv [nprg];
    for   (int i=0; i<nprg;   i++){
      const Cell & arg_i = carg(i,0).cell_value();
      int         narg_i = arg_i.cols();	// calloc -> free
      argv[i] = static_cast<char**>( calloc(narg_i+1,sizeof(char*)) );
      argv[i]                 [narg_i] = 0;	// NULL-terminated C-str-vector
      for (int j=0; j<narg_i; j++){
	std::string str = arg_i(0,j).string_value();
	int         len = str.length();		// calloc -> free
	argv[i][j] = static_cast<char*>( calloc(len+1,sizeof(char)) );
	str.copy(argv[i][j], std::string::npos);// NULL-terminated C string
		 argv[i][j][len] = 0;
      }
    }

    int nproc [nprg],				NPROC = 0;
	for (int i=0; i<nprg; i++)		NPROC+=(
	  nproc[i] = static_cast<int>( cnpr(i,0).double_value() ) );

MPI_Info inf  [nprg];
	for (int i=0; i<nprg; i++)
	  inf  [i] = MPITB_ptrcast( MPI_Info, cinf(i,0) );

						/***************/
						/* MPI BINDING */
						/***************/
MPI_Comm children;
    int  errs [NPROC];
    int  info = NAME (nprg, prg, argv, nproc, inf,
				root, parents, &children, errs);
	Matrix merr(NPROC,1,0);
    for (int i=0; i<NPROC; i++) merr(i,0) = errs[i];	// promo int->double

							/********
							 * Free *
							 ********/
    for (  int i=0; i<nprg;  i++){
      free  (prg [i]); int narg_i = carg(i,0).cell_value().cols();
      for (int j = 0;  j < narg_i; j++)
	free(argv[i][j]);
      free  (argv[i]);
    }
						/***************/
						/* RETURN VAL  */
						/***************/
    octave_value_list retval;
	   retval (0) = info;
	   retval (1) = MPITB_intcast(children);
	   retval (2) = merr;
    return retval;
}

