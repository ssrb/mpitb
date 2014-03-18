/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME  MPI_Errhandler_create
//#define   NAME  MPI_Errhandler_free
/*
 * ----------------------------------------------------
 * Create an MPI-style error handler on a communicator
 * [info eh] = MPI_Errhandler_create ('OctaveCMD')
 *
 * OctaveCMD must be an "Octave-feval()-able" function
 *   accepting comm(int), errclass(int), errcode(int), errmsg(str)
 * ---------------------------------------------------------------
 * Free an MPI-style error handler on a communicator
 *  info     = MPI_Errhandler_free   (eh)
 * ----------------------------------------------------
 */

 #include "mpitb.h"	// buildblocks NARGCHK, etc
/*---------------------------------------------------------------------
 * The space before #include is a trick to detect from Makefile
 * (using grep '^ \#include ...') that these files are linked.
 * Explicit rules will be given to compile just MPI_Errhandler_create
 *		and link MPI_Errhandler_free to MPI_Errhandler_create
 *---------------------------------------------------------------------
 * All other sources without space before #include are also detected
 * (using grep '^\#include ...', notice no leading space)
 * and compiled with normal .cc->.o->.oct rules
 *---------------------------------------------------------------------
 */

#include "hErr.h"	// buildblock  BLCK_ONEHND
#include <parse.h>	// feval()
#include <stdarg.h>	// va_start/arg/end used in wrappers


/*---------------------------------------------------------------------
 * User provides an feval()-able string
 * C-callbacks are defined here in the role of 'wrappers'
 * LAM/MPI will call wapper<N> which in turn will call assocd Octave callback
 * Can't code just 1 wapper here, it might be wished 1 wrapper per communicator
 * Below, under the 'ehfun' definition, more wrappers can be added, as required
 *---------------------------------------------------------------------
 */



#define WRAPPER(N)							\
/*-----------------------------------*/ /*wrapper called from LAM/MPI*/	\
void wrapper##N(MPI_Comm*cptr, int*eptr, ...){				\
/*-----------------------------------*/	/*wrapper's arglist mandatory*/	\
	   va_list  arglist;						\
	   va_start(arglist, eptr);					\
int  ercod=va_arg  (arglist, int );					\
char*ermsg=va_arg  (arglist, char *);					\
	   va_end  (arglist);						\
									\
octave_value_list args;							\
	args(0) = MPITB_intcast (					\
		  *cptr );						\
	args(1) = *eptr;						\
	args(2) = ercod;						\
	args(3) = ermsg;		/* ermsg becomes oct_string */	\
									\
	if ( ! feval(ehfun[N].OctaveCMD,args,0).empty() ){		\
	  error("MPITB wrapper" STRFY(N) ": error during feval()");	\
	}								\
}

MPI_Handler_function wrapper0x0;	/* forward decls so that they're */
MPI_Handler_function wrapper0x1;	/* known under mhfun[] definition*/
MPI_Handler_function wrapper0x2;
MPI_Handler_function wrapper0x3;
MPI_Handler_function wrapper0x4;
MPI_Handler_function wrapper0x5;
MPI_Handler_function wrapper0x6;
MPI_Handler_function wrapper0x7;
MPI_Handler_function wrapper0x8;
MPI_Handler_function wrapper0x9;
MPI_Handler_function wrapper0xA;
MPI_Handler_function wrapper0xB;
MPI_Handler_function wrapper0xC;
MPI_Handler_function wrapper0xD;
MPI_Handler_function wrapper0xE;	/* ADD additional desired wrappers */
MPI_Handler_function wrapper0xF;	/* both here and below twice */

struct ehfs{
	std::string OctaveCMD;		/* desired Octave callback */
	MPI_Errhandler	   eh;		/* LAM/MPI returned handle */
	MPI_Handler_function*wrapper;	/* MEX wrapper for calling-back */
};
struct ehfs ehfun[]=			/* should be static? */
{
	 {"" ,MPI_ERRHANDLER_NULL, wrapper0x0}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x1}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x2}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x3}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x4}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x5}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x6}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x7}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x8}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0x9}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xA}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xB}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xC}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xD}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xE}
	,{"" ,MPI_ERRHANDLER_NULL, wrapper0xF}
};			/* ADD as desired, here above and here below */
WRAPPER(0xF)
WRAPPER(0xE)
WRAPPER(0xD)
WRAPPER(0xC)
WRAPPER(0xB)
WRAPPER(0xA)
WRAPPER(0x9)
WRAPPER(0x8)
WRAPPER(0x7)
WRAPPER(0x6)
WRAPPER(0x5)
WRAPPER(0x4)
WRAPPER(0x3)
WRAPPER(0x2)		/* these are the real definitions */
WRAPPER(0x1)		/* for the previous forward decls */
WRAPPER(0x0)

#define	MAXEHF (sizeof(ehfun)/sizeof(struct ehfs))


#define BLCK_LOOKFOR(PNAME,IDX,TEH,FRMT)				\
	unsigned IDX=0;				/* Table index   */	\
	while(  (IDX< MAXEHF)						\
	&&(ehfun[IDX].eh != TEH))		/* Target errhnd */	\
		 IDX++;							\
	if      (IDX==MAXEHF){						\
	  error(STRFY(PNAME) ": " FRMT);				\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}



/*--------------------------------------------------------------------------
 * Create an MPI-style error handler on a communicator
 * [info eh] = MPI_Errhandler_create ('OctaveCMD')
 *
 * OctaveCMD must be an "Octave-feval()-able" function
 *   accepting comm(int), errclass(int), errcode(int), errmsg(str)
 *---------------------------------------------------------------------
 */
#define   NAME  MPI_Errhandler_create
DEFUN_DLD(NAME, args, nargout,
"MPI_Errhandler_create  Creates MPI_Errhandler on comm. based on an Octave cmd\n\
\n\
  [info eh] = MPI_Errhandler_create ('OctaveCMD')\n\
\n\
  OctaveCMD    user defined error handling procedure\n\
    prototype: OctaveCMD(comm, errclass, error, errmsg)\n\
               comm     (int)  communicator origin of the error\n\
               errclass (int)  error class\n\
               error    (int)  error code\n\
               errmsg   (str)  error message\n\
\n\
  eh   (int)   newly created MPI error handler based on OctaveCMD (handle)\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
     16 MPI_ERR_OTHER  Other error - use MPI_Error_string for more info\n\
\n\
  SEE ALSO: MPI_Errhandler_set, MPI_Errhandler_get, MPI_Errhandler_free\n\
            errors\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_create_errhandler\n\
\n\
")
{
    NARGCHK     (NAME,1)
    BLCK_ONESTR (NAME,0,OctaveCMD)	/* OctaveCMD */

    BLCK_LOOKFOR(NAME,i,MPI_ERRHANDLER_NULL,	"wrapper table exhausted")

    int info =   NAME ( ehfun[i].wrapper,
		       &ehfun[i].eh );
    if (info ==  MPI_SUCCESS)
			ehfun[i].OctaveCMD =
				 OctaveCMD ;
    RET_2_ARG (info, MPITB_intcast (
			ehfun[i].eh)      )
}



/*--------------------------------------------------------------------------
 * Free an MPI-style error handler on a communicator
 *  info     = MPI_Errhandler_free   (eh)
 *---------------------------------------------------------------------
 */
#undef    NAME
#define   NAME  MPI_Errhandler_free
DEFUN_DLD(NAME, args, nargout,
"MPI_Errhandler_free    Frees a previously created MPI_Errhandler object\n\
\n\
  info = MPI_Errhandler_free (eh)\n\
\n\
  eh   (int)   MPI error handler (handle from MPI_Errhandler_create)\n\
               Set to MPI_ERRHANDLER_NULL on exit.\n\
\n\
  info (int)   return code\n\
      0 MPI_SUCCESS    No error\n\
     13 MPI_ERR_ARG    Invalid argument\n\
\n\
  SEE ALSO: MPI_Errhandler_set, MPI_Errhandler_get, MPI_Errhandler_create\n\
            errors\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_free_errhandler\n\
\n\
  If an error occurs in an MPI function, the current MPI error handler is\n\
  called to handle it. By default, this error handler aborts the MPI job.\n\
  The error handler may be changed with MPI_Errhandler_set; the predefined\n\
  error handler MPI_ERRORS_RETURN may be used to cause error values to be\n\
  returned.\n\
\n\
  Note that MPI does not guarantee that an MPI program can continue\n\
  past an error.\n\
\n\
")
{
    NARGCHK     (NAME,1)
    BLCK_ONESYM (NAME,0,eh)		/* ehnam, ehsym */
    BLCK_ONEHND (NAME,0)		/* ehnd */

    BLCK_LOOKFOR(NAME,i,ehnd,		"handler not found in wrapper table")

    int info =   NAME( &ehfun[i].eh );	/* MPI forces it NULL */
    if (info ==  MPI_SUCCESS)
			ehfun[i].OctaveCMD = "";

    ehsym.varref() =  octave_value(MPITB_intcast(ehfun[i].eh));
    RET_1_ARG   (info)
}

//  if (ehfun[i].eh != MPI_ERRHANDLER_NULL)	/* forced? sure? */
//    printf("MPI_Errhandler_free: warning: didn't get MPI_ERRHANDLER_NULL\n");

/*--------------------------------------------------------------------------*/
