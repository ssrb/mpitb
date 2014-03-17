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
 * Status Functions: for the Recv, Test/Wait & Get families
 * ----------------------------------------------------
 */
#include <oct-map.h>	// avoid errmsg "Oct.map -- incomplete datatype"

/* -------------------------------------------------------------------------
 * status structs as of:
 * ----------
 * LAM-7.1.3:
 * ----------
 * struct _status {
 *         int             MPI_SOURCE;
 *         int             MPI_TAG;
 *         int             MPI_ERROR;
 *         int             st_length;              / * message length * /
 * };
 * -----------
 * OMPI-1.2.3:
 * -----------
 * struct ompi_status_public_t {
 *   int MPI_SOURCE;
 *   int MPI_TAG;
 *   int MPI_ERROR;
 *   int _count;
 *   int _cancelled;
 * };
 * -------------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 * -------- Auxiliary Functions --------------------------------------------
 * ------------------------------------------------------------------------- */
 

/* -------------------------------------------------------------------------
 * is_MPI_Stat: aux function to test MPI_Status struct
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Get_count.cc:	MPI_Get_elements.cc:
 * -------------------------------------------------------------------------
 * hTstWait.h: (1 uses) ->	MPI_Test_cancelled
 * -------------------------------------------------------------------------
 */
/*--------------------------------------*/
bool is_MPI_Stat (const octave_value &ov){
/*--------------------------------------*/
    int flag = ov.is_map();
    if (!flag) return (false);

    Octave_map map = ov.map_value();
    flag = map.contains("src")
	&& map.contains("tag")
	&& map.contains("err")
#if	LAM_MPI
	&& map.contains("len")
#elif	OPEN_MPI
	&& map.contains("cnt")
	&& map.contains("can")
#endif
	;

    return flag;
}


/* -------------------------------------------------------------------------
 * get_MPI_Stat: aux function to get MPI_Status struct
 * -------------------------------------------------------------------------
 * crossref: used from
 * MPI_Get_count.cc:	MPI_Get_elements.cc:
 * -------------------------------------------------------------------------
 * hTstWait.h: (1 uses) ->	MPI_Test_cancelled
 * -------------------------------------------------------------------------
 */
/*---------------------------------------------*/
MPI_Status get_MPI_Stat (const octave_value &ov){
/*---------------------------------------------*/
    MPI_Status stat;
    Octave_map map = ov.map_value();

    stat.MPI_SOURCE = map.contents("src")(0).int_value();
    stat.MPI_TAG    = map.contents("tag")(0).int_value();
    stat.MPI_ERROR  = map.contents("err")(0).int_value();
#if	LAM_MPI
    stat.st_length  = map.contents("len")(0).int_value();
#elif	OPEN_MPI
    stat._count     = map.contents("cnt")(0).int_value();
    stat._cancelled = map.contents("can")(0).int_value();
#endif

    return stat;
}


/* -------------------------------------------------------------------------
 * put_MPI_Stat: aux function to put MPI_Status struct
 * -------------------------------------------------------------------------
 * crossref: used from
 * hRecv.h:    (3 uses) ->	MPI_Recv / Sendrecv [_replace]
 * hTstWait.h: (7 uses) ->	MPI_Test [any] / _Wait [any]
 * 				MPI_Test_Cancelled
 * 				MPI_ [I] Probe
 * -------------------------------------------------------------------------
 */
/*---------------------------------------------*/
Octave_map put_MPI_Stat (const MPI_Status &stat){
/*---------------------------------------------*/
    Octave_map map;

    map.assign("src", stat.MPI_SOURCE);
    map.assign("tag", stat.MPI_TAG   );
    map.assign("err", stat.MPI_ERROR );
#if	LAM_MPI
    map.assign("len", stat.st_length );
#elif	OPEN_MPI
    map.assign("cnt", stat._count    );
    map.assign("can", stat._cancelled);
#endif

    return map;
}
/* -------------------------------------------------------------------------
 * put_MPI_Stats: aux function to put a vector of MPI_Status structs
 * -------------------------------------------------------------------------
 * crossref: used from
 * hTstWait.h: (4 uses) ->	MPI_Testall [some] / _Waitall [some]
 * -------------------------------------------------------------------------
 */
/*----------------------------------------------*/
Octave_map put_MPI_Stats (const int count,
			  const MPI_Status *stat){
/*----------------------------------------------*/
    Cell srcCell(1,count),
    	 tagCell(1,count),
	 errCell(1,count)
#if	LAM_MPI
	,lenCell(1,count)
#elif	OPEN_MPI
	,cntCell(1,count)
	,canCell(1,count)
#endif
	;

    for (int i=0; i<count; i++){
      srcCell(i) = stat[i].MPI_SOURCE;
      tagCell(i) = stat[i].MPI_TAG   ;
      errCell(i) = stat[i].MPI_ERROR ;
#if	LAM_MPI
      lenCell(i) = stat[i].st_length ;
#elif	OPEN_MPI
      cntCell(i) = stat[i]._count    ;
      canCell(i) = stat[i]._cancelled;
#endif
    }
    Octave_map map;
      map.assign("src", srcCell);
      map.assign("tag", tagCell);
      map.assign("err", errCell);
#if	LAM_MPI
      map.assign("len", lenCell);
#elif	OPEN_MPI
      map.assign("cnt", cntCell);
      map.assign("can", canCell);
#endif

    return map;
}
