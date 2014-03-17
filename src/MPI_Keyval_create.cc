/*
## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------
 */
//#define   NAME  MPI_Keyval_create
//#define   NAME  MPI_Keyval_free
//#define   NAME  MPI_Attr_put
/*
 * ----------------------------------------------------
 * Generates a new communicator attribute key
 * [info kv] = MPI_Keyval_create (copy_fn, del_fn, extra_st)
 *
 * copy/del_fn must be "Octave-feval()-able" functions with prototypes:
 *	[attr_out flag] = copy_fn (OLDcomm, keyval, extra_st, attr_in)
 *       info           =  del_fn    (comm, keyval, attr, extra_st)
 * ---------------------------------------------------------------------
 * Frees attribute key for communicator cache attribute
 *  info     = MPI_Keyval_free   (kv)
 * ---------------------------------------------------------------------
 * Stores a communicator attribute value associated with a key (comm,kv)
 * info = MPI_Attr_put (comm, kv, attr)
 * ----------------------------------------------------
 */

 #include "mpitb.h"		// buildblocks NARGCHK, etc
/*---------------------------------------------------------------------
 * The space before #include is a trick to detect from Makefile
 * (using grep '^ \#include ...') that these files are linked.
 * Explicit rules will be given to compile just MPI_Keyval_create.oct
 * and link MPI_Keyval_free and MPI_Attr_put to MPI_Keyval_create.oct
 *---------------------------------------------------------------------
 * All other sources without space before #include are also detected
 * (using grep '^\#include ...', notice no leading space)
 * and compiled with normal .cc->.o->.oct rules
 *---------------------------------------------------------------------
 */

/*---------------------------------------------------------------------
 * it's not evident from the call patterns above
 * that attribute keys and values are stored together
 * (search _attr in LAM/MPI source file $LAM/include/mpisys.h)
 *
 * struct _attrkey {
 *         void_fn         *ak_copy;               / * copy function * /
 *         void_fn         *ak_del;                / * delete function * /
 *         void            *ak_extra;              / * extra info * /
 *         int             ak_refcount;            / * reference count * /
 *         int             ak_flags;               / * properties * /
 * };
 *
 * struct _attr {
 *         int             a_key;                  / * attribute key * /
 *         void            *a_value;               / * attribute value * /
 * };
 *
 * that is, when an attrkey is created it is not attached to any comm,
 * it's simply that room is prepared to store an attrval (which is void*)
 * and a number (attrkey) assigned to that room (kv in the call patterns)
 * look 5-6 lines above to see the (a_key,a_value) pair
 * 
 * later, an attrval can be put on that room (a_value) and the pair (kv,val)
 * attached to a comm with MPI_Attr_put. There is another MPI_Attr_delete
 * call which should remove the attrval (whatever that means) and
 * deattaches the pair from the communicator.
 *
 * it's a pity that sizeof(octave_value) == 8 > 4 == sizeof(void*)
 * so we are forced to pass &octave_value instead of the real octave_value
 * we make a "static copy" of the attribute together with the wrapper entry
 * and pass the address of that copy as attribute to LAM/MPI. Thus
 * we don't want the attribute destroyed just because MPI_Attr_delete
 *
 * when communicators are duplicated, attributes should be copied as well
 * to the new communicator. MPI_Comm_dup calls the "copy-wrapper"
 * just in case the user knows what must be done to copy an attr
 * (apart from copying the void*, which is what MPI_DUP_FN does)
 * MPI_DUP_FN fits our needs, we just need the void* copied along,
 * the real attribute value is safely stored (the "static copy")
 * with the keyval
 * 
 * since users can selectively delete attributes on some communicators
 * and not in others, we do not want an octave_value to be destroyed
 * just because it must be de-attached from just 1 comm.
 * our delete wrappers don't destroy any octave_value attr (ahem!)
 * our copy wrappers don't duplicate any octave_value attr (ahem!)
 * it's when the key is freed and the pair (kv,val) is marked empty
 * when the octave_value attr can be safely destroyed (the "static copy")
 *
 * predefined attributes are tricky, they're ints and sizeof(int) == 4
 * we could also have implemented MPITB attributes that way,
 * forcing that they can be only integer values
 * and then the copy/delete wrappers (and all these MPI routines)
 * would have been much easier to code and test :-)
 *---------------------------------------------------------------------
 */

#include <parse.h>		// feval()


/*---------------------------------------------------------------------
 * User provides two feval()-able strings
 * C-callbacks are defined here in the role of 'wrappers'
 * LAM/MPI will call wapper<N> which in turn will call assocd Octave callback
 * Can't code just 1 wapper here, it might be wished 1 wapper per attribute
 * More wrappers can be added under the 'copy/delfun' definition, as desired 
 *---------------------------------------------------------------------
 */


#define COPYWRAPPER(N)							\
/*-----------------------------------*/	/*wrapper called from LAM/MPI*/	\
int copywrapper##N(MPI_Comm OLDcomm, int keyval,			\
		   void* extra_st, void* attr_in,			\
		   void* attr_out, int*    flag){			\
/*-----------------------------------*/	/*wrapper's arglist mandatory*/	\
	if (regfun[N].keyval == MPI_KEYVAL_INVALID)			\
	  error("Octave copy wrapper" STRFY(N) ": key not active ?!?");	\
									\
octave_value_list args;							\
args(0) =   MPITB_intcast	      ( OLDcomm );			\
args(1) =				keyval ;			\
args(2) = * static_cast<octave_value*> (extra_st);/*constr.1 copy more*/\
args(3) = * static_cast<octave_value*> ( attr_in);/*f/passed void ptrs*/\
									\
octave_value_list outs = feval(regfun[N].OctaveCOPY,args,4);		\
	if (outs.length() != 3){					\
	error("copy wrapper" STRFY(N) ": wrong#args(3) out from feval()");\
	}								\
									\
	if ( (outs(1).is_string())	/*internal(.oct)copy requested*/\
	  && (outs(1).string_value() == "MPITB_INTERNAL") ){		\
									\
/*	* (void**) attr_out =		/  C-style_cast !! */		\
/*	& regfun[N].st_attr ;		/  pass (void* to attr) away */	\
	* static_cast<void**>(attr_out) = & regfun[N].st_attr ;		\
									\
	}else{		/* hmpf,octave user wants to change attr :-( */	\
			/* we try our best: static copy of it now */	\
									\
	  regfun[N].scratch = outs(1);	/* pray for no more copies */	\
/*C-sty	* (void**) attr_out =		/  (it will work only once) */	\
/*cast!	& regfun[N].scratch;		/  and pass(void* to it)away */	\
	* static_cast<void**>(attr_out) = & regfun[N].scratch ;		\
	}								\
									\
	*flag     = outs(2).int_value();/* outs about to out-of-scope */\
	return	    outs(0).int_value();/* that's why staticcopy need */\
}

/*---------------------------------------------------------------------*/

#define DELWRAPPER(N)							\
/*-----------------------------------*/	/*wrapper called from LAM/MPI*/	\
int delwrapper##N(MPI_Comm comm, int keyval,				\
		  void* attr, void* extra_st){				\
/*-----------------------------------*/	/*wrapper's arglist mandatory*/	\
	if (regfun[N].keyval == MPI_KEYVAL_INVALID)			\
	  error("Octave delete wrapper" STRFY(N) ": key not active ?!?");\
									\
octave_value_list args;							\
args(0) =   MPITB_intcast	      ( comm );				\
args(1) =				keyval;				\
args(2) = * static_cast<octave_value*> (attr    );/*constr.1 copy more*/\
args(3) = * static_cast<octave_value*> (extra_st);/*f/passed void ptrs*/\
									\
	/* kind of weird, wanna del attr and yet we make copies :-) */	\
									\
octave_value_list outs = feval(regfun[N].OctaveDEL, args,1);		\
	if (outs.length() != 1){					\
	error("delete wrapper" STRFY(N) ": wrong#args(1) out from feval()");\
	}								\
									\
    /*	regfun[N].scratch = */	/* nope, not in other communicators! */	\
    /*	regfun[N].st_attr = */	/* nope, attr destroyed when key_free */\
    /*	   octave_value() ; */	/* NULOV defined later */		\
									\
/**********************************************************************/\
/* If I'm not getting it wrong, that means we have done: nothing! :-) */\
/**********************************************************************/\
									\
	return outs(0).int_value();					\
}

/*---------------------------------------------------------------------*/

	/***************************************************/
	/* forward decls to make them known under regfun[] */
	/***************************************************/
MPI_Copy_function copywrapper0x0;	MPI_Delete_function delwrapper0x0;
MPI_Copy_function copywrapper0x1;	MPI_Delete_function delwrapper0x1;
MPI_Copy_function copywrapper0x2;	MPI_Delete_function delwrapper0x2;
MPI_Copy_function copywrapper0x3;	MPI_Delete_function delwrapper0x3;
MPI_Copy_function copywrapper0x4;	MPI_Delete_function delwrapper0x4;
MPI_Copy_function copywrapper0x5;	MPI_Delete_function delwrapper0x5;
MPI_Copy_function copywrapper0x6;	MPI_Delete_function delwrapper0x6;
MPI_Copy_function copywrapper0x7;	MPI_Delete_function delwrapper0x7;
MPI_Copy_function copywrapper0x8;	MPI_Delete_function delwrapper0x8;
MPI_Copy_function copywrapper0x9;	MPI_Delete_function delwrapper0x9;
MPI_Copy_function copywrapper0xA;	MPI_Delete_function delwrapper0xA;
MPI_Copy_function copywrapper0xB;	MPI_Delete_function delwrapper0xB;
MPI_Copy_function copywrapper0xC;	MPI_Delete_function delwrapper0xC;
MPI_Copy_function copywrapper0xD;	MPI_Delete_function delwrapper0xD;
MPI_Copy_function copywrapper0xE;	MPI_Delete_function delwrapper0xE;
MPI_Copy_function copywrapper0xF;	MPI_Delete_function delwrapper0xF;
	/**************************************************************/
	/* ADD additional desired wrappers, both here and below twice */
	/**************************************************************/
/*---------------------------------------------------------------------*/

struct regfs{				/* registry for wrappers */
	int	  keyval;		/* associated attribute key */
 octave_value     stextra,		/* static extrastat     copy */
		  st_attr,		/* static original attr copy */
		  scratch;		/* static attr user-copy-out */
  std::string     OctaveCOPY;		/* desired Octave callbacks */
  std::string     OctaveDEL ;
MPI_Copy_function  *cwrapper;		/* C wrapper to call-back */
MPI_Delete_function*dwrapper;
};
#define NULOV octave_value()		/* NULL octave_value */
struct regfs regfun[]=			/* static? */
{
 {MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x0,delwrapper0x0}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x1,delwrapper0x1}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x2,delwrapper0x2}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x3,delwrapper0x3}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x4,delwrapper0x4}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x5,delwrapper0x5}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x6,delwrapper0x6}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x7,delwrapper0x7}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x8,delwrapper0x8}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0x9,delwrapper0x9}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xA,delwrapper0xA}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xB,delwrapper0xB}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xC,delwrapper0xC}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xD,delwrapper0xD}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xE,delwrapper0xE}
,{MPI_KEYVAL_INVALID, NULOV,NULOV,NULOV, "","", copywrapper0xF,delwrapper0xF}
};			/* ADD as desired, here above and here below */

COPYWRAPPER(0xF) DELWRAPPER(0xF)
COPYWRAPPER(0xE) DELWRAPPER(0xE)
COPYWRAPPER(0xD) DELWRAPPER(0xD)
COPYWRAPPER(0xC) DELWRAPPER(0xC)
COPYWRAPPER(0xB) DELWRAPPER(0xB)
COPYWRAPPER(0xA) DELWRAPPER(0xA)
COPYWRAPPER(0x9) DELWRAPPER(0x9)
COPYWRAPPER(0x8) DELWRAPPER(0x8)
COPYWRAPPER(0x7) DELWRAPPER(0x7)
COPYWRAPPER(0x6) DELWRAPPER(0x6)
COPYWRAPPER(0x5) DELWRAPPER(0x5)
COPYWRAPPER(0x4) DELWRAPPER(0x4)
COPYWRAPPER(0x3) DELWRAPPER(0x3)
COPYWRAPPER(0x2) DELWRAPPER(0x2) /* these are the real definitions */
COPYWRAPPER(0x1) DELWRAPPER(0x1) /* for the previous forward decls */
COPYWRAPPER(0x0) DELWRAPPER(0x0)

#define	MAXFUN (sizeof(regfun)/sizeof(struct regfs))


#define BLCK_LOOKFOR(PNAME,IDX,TKV,FRMT,ERRARG)				\
	unsigned IDX=0;				/* Table index   */	\
	while(  (IDX< MAXFUN)						\
	    &&  (regfun[IDX].keyval != TKV))	/* Target Keyval */	\
		 IDX++;							\
	if      (IDX==MAXFUN){						\
	  error(STRFY(PNAME) ": " STRFY(FRMT) , ERRARG);		\
	  return octave_value(MPI_ERR_ARG); /* error returns nothing */	\
	}


/*--------------------------------------------------------------------------
 * Generates a new communicator attribute key
 * [info kv] = MPI_Keyval_create (copy_fn, del_fn, extra_st)
 *
 * copy/del_fn must be "Octave-feval()-able" functions with prototypes:
 *	[attr_out flag] = copy_fn (OLDcomm, keyval, extra_st, attr_in)
 *       info           =  del_fn    (comm, keyval, attr, extra_st)
 *--------------------------------------------------------------------------
 */
#define   NAME  MPI_Keyval_create
DEFUN_DLD(NAME, args, nargout,
"MPI_Keyval_create	Generates a new communicator attribute key\n\
\n\
  [info kv] = MPI_Keyval_create (copy_fn, del_fn, extra_st)\n\
\n\
  extra_st(var)	Extra status info for callback functions (copy_/del_fn)\n\
\n\
  del_fn  (str)	Delete Octave callback function for keyval (or 'NULL')\n\
                'NULL' -> MPI_NULL_DELETE_FN\n\
	---------------------------------------------------------------\n\
	prototype:	info = del_fn (comm, keyval, attr, extra_st)\n\
	---------------------------------------------------------------\n\
\n\
  copy_fn (str) Copy   Octave callback function for keyval ('NULL'/'DUP')\n\
                'NULL' -> MPI_NULL_COPY_FN\n\
                'DUP'  -> MPI_DUP_FN\n\
	---------------------------------------------------------------\n\
	prototype:\n\
	[info attr_out flag] = copy_fn (OLDcomm, keyval, extra_st, attr_in)\n\
	---------------------------------------------------------------\n\
	where\n\
	OLDcomm  (int)	communicator in which the attribute is stored\n\
	keyval   (int)	key value for attribute access\n\
	extra_st (var)	extra information (passed when Keyval_create call)\n\
	attr_in  (var)	attribute value\n\
	info     (int)	error code (MPI_SUCCESS if no error)\n\
	attr_out (var)	attribute copy (if flag==1)\n\
	flag     (int)	0 if attr_out is meaningless, 1 if not\n\
	---------------------------------------------------------------\n\
\n\
  kv      (int)	key value for future access\n\
\n\
  info    (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     13 MPI_ERR_ARG	Invalid argument\n\
\n\
  SEE ALSO: MPI_Keyval_free, MPI_Attr_put, MPI_Attr_get, MPI_Attr_delete\n\
            MPI_NULL_DELETE_FN, MPI_NULL_COPY_FN, MPI_DUP_FN,\n\
            caching\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_create_keyval\n\
\n\
")
/*--------------------------------------------------------------------------*/
{
	/* [info kv] = MPI_Keyval_create (copy_fn, del_fn, extra_st) */

	NARGCHK    (NAME,3)
	BLCK_ONESTR(NAME,0, copy_fn)	/* 1st string arg */
	BLCK_ONESTR(NAME,1,  del_fn)	/* 2nd string arg */
	const octave_value&extra_st=	/* 3rd extra  arg */
		    args(2);		/* whichever type it is */

					/* look for free wrapper */
	BLCK_LOOKFOR(NAME,index,MPI_KEYVAL_INVALID,
		    "MPITB wrapper table exhausted: %d entries",MAXFUN)

	/*------------------------------------------------*/	// ** ARG1 **

	MPI_Copy_function *	CPF;			  int Oct_cpy_flg=0;
     if (copy_fn == "NULL")	CPF = MPI_NULL_COPY_FN;
else if (copy_fn == "DUP" )	CPF = MPI_DUP_FN      ;
else{				CPF = regfun[index].cwrapper; Oct_cpy_flg=1;
				      regfun[index].OctaveCOPY = copy_fn;
    }
	/*------------------------------------------------*/	// ** ARG2 **

	MPI_Delete_function *	DLF;			  int Oct_del_flg=0;
     if ( del_fn == "NULL")	DLF = MPI_NULL_DELETE_FN; 
else{				DLF = regfun[index].dwrapper; Oct_del_flg=1;
				      regfun[index].OctaveDEL  =  del_fn;
    }

	if (Oct_cpy_flg ^ Oct_del_flg){
	  error("MPI_Keyval_create: "
		"mixing Octave & MPI copy/delete-callbacks\n");
	  return octave_value(MPI_ERR_ARG);	// error returns nothing
	}

	/*------------------------------------------------*/	// ** ARG3 **

	regfun[index].stextra = extra_st;	/* live here from now on */

	/*------------------------------------------------*/	// ** WRAP **

	int keyval, info = NAME (CPF, DLF, &keyval, &regfun[index].stextra);

	regfun[index].keyval = keyval;		/* mark wrapper entry as used */

	RET_2_ARG  (info, keyval)
}
	/*  [info kv] = MPI_Keyval_create (copy_fn, del_fn, extra_st) */


/* ---------------------------------------------------------------------
 * no matter if extra_st was a temporary, stextra won't get out of scope
 *
 * extra_st might be a temporary (think of _create('NULL','NULL',{}) )
 * extra_st would reach here with count==2 (one in octave, other as args(2))
 * and would _disappear_ after returning (both temporary and args(2) destroyed)
 *
 * we need to copy that temporary cell {} (or whatever) in "static" scope
 * we pass that copy's address (MPI needs void*, so we cast)
 * for using that (void*) stextra from copy/delete functions,
 * cast it again to octave_value to use member functions
 *
 * Other way would have been requiring that extra_st be a variable
 * so it would be stored in symbol table (and might be cleared... d-oh)
 * ---------------------------------------------------------------------
 */



/*--------------------------------------------------------------------------
 * Frees attribute key for communicator cache attribute
 *  info     = MPI_Keyval_free   (kv)
 *--------------------------------------------------------------------------
 */
#undef    NAME
#define   NAME  MPI_Keyval_free
DEFUN_DLD(NAME, args, nargout,
"MPI_Keyval_free	Frees attribute key for communicator cache attribute\n\
\n\
  info = MPI_Keyval_free (kv)\n\
\n\
  kv   (int)	key value for attribute access (from MPI_Keyval_create)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS	No error\n\
     16 MPI_ERR_OTHER   Attempt to use an invalid keyval\n\
     13 MPI_ERR_ARG	Invalid argument\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Attr_put, MPI_Attr_get, MPI_Attr_delete\n\
            MPI_NULL_DELETE_FN, MPI_NULL_COPY_FN, MPI_DUP_FN,\n\
            caching\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_free_keyval\n\
\n\
")
/*--------------------------------------------------------------------------*/
{
	/*  info     = MPI_Keyval_free   (kv) */

	NARGCHK    (NAME,1)
	BLCK_NULSYM(NAME,0, key   )		/* keynam, keysym */
	BLCK_ONEINT(NAME,0, keyval)		/* int arg value  */

					/* look for corresponding wrapper */
	BLCK_LOOKFOR(NAME,index,keyval,
		    "keyval %d not found in MPITB wrapper table",keyval)

	regfun[index].keyval  =MPI_KEYVAL_INVALID;
//	regfun[index].stextra.~octave_value();		/* BUG ? */
	regfun[index].stextra =		// all stored values destroyed
	regfun[index].st_attr =		// attr finally destroyed (not del_fn)
	regfun[index].scratch = NULOV;	// NULOV == octave_value()
	regfun[index].OctaveCOPY = "";
	regfun[index].OctaveDEL  = "";

	int info = NAME ( &keyval );	/* KEYVAL_INVALID */
    if (keysym)
	keysym-> define (  keyval );	/* return it as well */

	RET_1_ARG (info)
}
	/*   info     = MPI_Keyval_free   (kv) */



/*--------------------------------------------------------------------------
 * Stores a communicator attribute value associated with a key (comm,kv)
 * info = MPI_Attr_put (comm, kv, attr)
 *--------------------------------------------------------------------------
 */
#undef    NAME
#define   NAME  MPI_Attr_put
DEFUN_DLD(NAME, args, nargout,
"MPI_Attr_put		Stores a comm. attribute value associated with a key\n\
\n\
  info = MPI_Attr_put (comm, kv, attr)\n\
\n\
  comm (int)	communicator to which attribute will be attached (handle)\n\
  kv   (int)	key value, as returned by MPI_Keyval_create\n\
  attr (val)	attribute value (any Octave value)\n\
\n\
  info (int)	return code\n\
      0 MPI_SUCCESS    No error\n\
      5 MPI_ERR_COMM   Invalid communicator\n\
     16 MPI_ERR_OTHER  LAM/MPI unable to acquire memory\n\
     13 MPI_ERR_ARG    Invalid argument (NULL pointer?)\n\
\n\
  SEE ALSO: MPI_Keyval_create, MPI_Keyval_free, MPI_Attr_get, MPI_Attr_delete\n\
            caching\n\
\n\
  This function is deprecated, replaced by MPI-2 MPI_Comm_set_attr\n\
\n\
")
/*--------------------------------------------------------------------------*/
{
	/* info = MPI_Attr_put (comm, kv, attr) */

	NARGCHK    (NAME,3)
	BLCK_ONECOM(NAME,0)		/* comm   */
	BLCK_ONEINT(NAME,1,keyval)	/* keyval */
//	BLCK_ONESYM(NAME,2,attr  )	/* attrnam, attrsym */
	const octave_value&attr  =	/* attr   */
		    args(2);		/* whichever type it is */

					/* look for corresponding wrapper */
	BLCK_LOOKFOR(NAME,index,keyval,
		    "keyval %d not found in MPITB wrapper table",keyval)

	regfun[index].st_attr = attr;	/* "static" copy */
					/* live here from now on */

	int info = NAME (comm, keyval, &regfun[index].st_attr);
				    // &attrsym->def());
	RET_1_ARG  (info)
}

/*--------------------------------------------------------------------------
 * MPI_Attr_put must be defined here
 * so we can take a static copy of attr in wrappers table
 *
 * Other way could be requiring that attr be a variable in symbol table
 * (and it could be cleared while keyval still in use... d-oh)
 *
 * Anyways, other ranks cannot see the octave_value but the pointer value
 * ie, attributes useless unless they are used as integers (stored as pointers)
 * like the predefined ones in LAM/MPI
 *
 * If this sounds weird to you, you'll enjoy the following remark from
 * the LAM source code: cgetattr.c:93-97
 * / *
 *   * This stuff is very confusing.  Be sure to see MPI-2 4.12.7.
 *   * /
 *               *((void **) value) = (pk->ak_flags & LAM_LANGF77)
 *	                ? (void *) &p->a_value : p->a_value;
 *
 *
 * See also the end of csetattr.c just to convince yourself that they just
 * store the (void*) values, they don't (can't) send any info to other ranks
 *--------------------------------------------------------------------------
 */
