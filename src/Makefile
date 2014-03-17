# Makefile for mpitb/src dir
# creates mpitb/DLD subdir and places there the compiled Toolbox
#

include Makefile.inc

# --------------------
# List of source files
# --------------------
#	direct listing accounting
# 308 =  15 includes (h*.h + mpitb.h)
#	  4 constants (IMPI_*.cc)
#	 10 constants (LAM_*.cc + LAM_MPI.cc + OPEN_MPI.cc)
#	  3 Makefiles
#	272 MPI_*.cc (3 of them links)
#	  1 mytest.cc
#	  1 mytests dir
#	  2 "normal" octave dlds (print_info.cc + unsetenv.cc)
# --------------------
# 287 = 14 constants IMPI* LAM* OPEN* + 272 MPI_* + 1 mytest.cc
# --------------------
#	checked by "ls MPI_* IMPI_* LAM_* OPEN_* mytest.cc | nl"
#	     or by "grep '#include "mpitb.h"' * | grep -v Makefile | nl"
#   3 of them are links:MPI_Attr_put.cc /
#			MPI_Keyval_free.cc	-> MPI_Keyval_create.cc
#			MPI_Errhandler_free.cc	-> MPI_Errhandler_create.cc
# detected due to the detail of including mpitb.h w[/out] leading space
#	for instance try "grep '^#include "mpitb.h"' * | nl"
#	and add that to	 "grep '^ #include "mpitb.h"' * | nl"
# tell them with "ls -la `grep '^ #include "mpitb.h"' * | cut -d: -f1`"
# --------------------

  FILSRCS  := $(shell grep  '^\#include "mpitb.h"' * | cut -d: -f1)
  LNKSRCS  := $(shell grep '^ \#include "mpitb.h"' * | cut -d: -f1)
  MPISRCS = $(FILSRCS) $(LNKSRCS)
  FILS    = $(patsubst %.cc, %, $(FILSRCS))
  LNKS    = $(patsubst %.cc, %, $(LNKSRCS))
  MPIS    = $(patsubst %.cc, %, $(MPISRCS))

# --------------------
# 2 NRMS, "normal" octave dlds
#	checked by "ls | grep -v MPI | grep -v LAM | grep -v OPEN | \
#			 grep -v .h | grep -v Make | grep -v mytest"
# --------------------
  NRMS = unsetenv print_info

  NAMS = $(MPIS) $(NRMS)

# --------------------
# There are also 3 Makefiles (main 2 are Makefile[.inc], and an older version)
# There are also 15 include files, and the "mytests" subdir for older mytest.cc
# --------------------

# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h

  INCS:= $(shell ls *.h )
  MAKS = Makefile Makefile.inc


# See Dependencies section below
MPIOBJS = $(addsuffix .o        ,$(MPIS))
NRMOBJS = $(addsuffix .o        ,$(NRMS))	# not really used
   OBJS = $(addsuffix .o        ,$(NAMS))
MPIDLDS = $(addsuffix .$(DLDSFX),$(MPIS))	# not really used / FILE/LINK
NRMDLDS = $(addsuffix .$(DLDSFX),$(NRMS))
   DLDS = $(addsuffix .$(DLDSFX),$(NAMS))

FILDLDS = $(addsuffix .$(DLDSFX),$(FILS))

INSTDIR = ../DLD

INSTDLDS= $(addprefix $(INSTDIR)/, $(DLDS))

#
# Target
#
install:      $(INSTDLDS)

.PHONY: clean
clean:
	-rm -f $(INSTDIR)/*			# - lets make continue
	-rmdir $(INSTDIR)			# even if cd or rmdir fail
	-rm -f *.o
	-rm -f *.$(DLDSFX)

#
# Build Rules
#
.SUFFIXES: .$(DLDSFX)

#
# DLDs go to the DLD subdir. No need to make unless sources are newer
# (DLDs in current subdir are not explicitly mentioned, just referred as %)
#
# This first rule line uses normal prerequisites
# The second uses "order only" prerequisites (make manual, sect.4.3)
# which is what we really mean: can't move there if no such subdir
#	but if it exists, we don't really care about dates
# Choose the first version if your make does not support order-only prereqs
#

#$(INSTDLDS): $(INSTDIR)/%: % $(INSTDIR)
$(INSTDLDS): $(INSTDIR)/%: % | $(INSTDIR)
	$(MV) $< $(INSTDIR)
#	$(CP) $< $(INSTDIR)

$(INSTDIR):
	mkdir -p $(INSTDIR)

#
# Normal (non-MPI) DLDs are easy to compile: use mkoctfile
#
$(NRMDLDS): %.$(DLDSFX): %.cc
	$(MKOCT) $<				# -o $@
	rm -f $*.o				# $* is the stem (%)

#
# MPITB DLDs are compiled using the flags detected in Makefile.inc
# (FILE DLDs, not LINKs)
#
$(FILDLDS): %.$(DLDSFX): %.o
	$(LD_CXX) $(LDFLAGS) $< -o $@ $(LFLAGS) $(LIBS)

$(MPIOBJS): %.o: %.cc
	$(CXX) -c $(CXXFLAGS) $(INCFLAGS) $< -o $@

#
# LINK DLDs: just link .oct file, just like .cc is linked too
#
# Having several .oct from same sources used to work for Oct-2.1.73/gcc3.2-4.1
# even though filesizes differed in 2B (as they do now) -> it's not gcc fault
# Something must have changed in Octave-2.9.10 and now it's more clever/silly
# now it detects it's not the same file and reloads it (why?)
#
# Problem circumvented if linked sources are converted into linked DLDs
# Notice that link is created in src, even if there is no target .oct there
# All that matters is that target .oct exists indeed in DLD subdir
#
# This could be improved so that no linked source file .cc was required
# In fact, I think we're there yet -> 
#
# MPI_Attr_put.cc ->	MPI_Keyval_free.cc	-> MPI_Keyval_create.cc
#			MPI_Errhandler_free.cc	-> MPI_Errhandler_create.cc
#
MPI_Errhandler_create.$(DLDSFX) \
    MPI_Keyval_create.$(DLDSFX) : %.$(DLDSFX): %.o
	$(LD_CXX) $(LDFLAGS) $< -o $@ $(LFLAGS) $(LIBS)

MPI_Errhandler_free.oct:   MPI_Errhandler_create.cc \
		$(INSTDIR)/MPI_Errhandler_create.oct
	ln -s		   MPI_Errhandler_create.oct $@

   MPI_Attr_put.oct \
MPI_Keyval_free.oct:	   MPI_Keyval_create.cc \
		$(INSTDIR)/MPI_Keyval_create.oct
	ln -s		   MPI_Keyval_create.oct $@

#
# Dependencies
#

.INTERMEDIATE:	$(OBJS) $(DLDS)

# Every MPITB command (Oct-file) source file needs mpitb.h
# MPIOBJS   Already defined at top
$(MPIOBJS): mpitb.h

# Makefile dependencies: if Makefile is changed, all must be rebuilt
# since new dependences could appear
Makefile: Makefile.inc
	touch $@

# We make the source .cc file itself depend on the Makefile
# NRMS = unsetenv print_info
# NAMS = $(MPIS) $(NRMS)
  NAMSRCS = $(addsuffix .cc, $(NAMS))

$(NAMSRCS): Makefile
	touch $@


# Include dependencies: in order to extract include dependency from .cc files
# we need to update includes whose included are updated
# For instance, if hSndRecv.h is updated, (see dependency graph below)
#	we must rebuild each file including hSndRecv / or hSend / or hRecv
# but from a .cc file including hSend we can only extract _that_ direct depend
# so we prefer touching hSend when hSndRecv changes, so the direct depend works
# 
# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h
# INCS:= $(shell ls *.h )
#
# hAttr.h hConst.h hErr.h hGrp.h hInfo.h hType.h hTopo.h mpitb.h
#					  |
#					hBuff.h
#					/  \
#				  hColl.h  hSndRecv.h  hStat.h
#						/   \  /    \
#					  hSend.h  hRecv.h  hTstWait.h
#
hTstWait.h: hStat.h
	touch $@

hRecv.h: hSndRecv.h hStat.h
	touch $@

hSend.h: hSndRecv.h
	touch $@

hColl.h hSndRecv.h: hBuff.h
	touch $@

hBuff.h: hType.h
	touch $@

# Include dependencies: extract include dependency from .cc files directly
#
# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h

# ATRS     =	MPI_Attr_delete		MPI_Attr_get
  ATRS    := $(shell grep '^.include "hAttr.h"' *.cc | cut -d: -f1)
  ATROBJS  = $(patsubst %.cc, %.o, $(ATRS))
$(ATROBJS): hAttr.h

# BUFS     =	MPI_Address					\
#		MPI_Buffer_attach	MPI_Buffer_detach	\
#		MPI_Pack		MPI_Unpack
  BUFS    := $(shell grep '^.include "hBuff.h"' *.cc | cut -d: -f1)
  BUFOBJS  = $(patsubst %.cc, %.o, $(BUFS))
$(BUFOBJS): hBuff.h

# COLS =	MPI_Allgather		MPI_Allgatherv		\
#		MPI_Alltoall		MPI_Alltoallv		\
#		MPI_Allreduce		MPI_Bcast		\
#		MPI_Gather		MPI_Gatherv		\
#		MPI_Reduce		MPI_Reduce_scatter	\
#		MPI_Scatter		MPI_Scatterv		\
#		MPI_Scan
  COLS    := $(shell grep '^.include "hColl.h"' *.cc | cut -d: -f1)
  COLOBJS  = $(patsubst %.cc, %.o, $(COLS))
$(COLOBJS): hColl.h

# CONS =	IMPI_CLIENT_COLOR.cc	IMPI_CLIENT_SIZE.cc
#		IMPI_HOST_COLOR.cc	IMPI_HOST_SIZE.cc
#
#		LAM_MPI.cc	LAM_MPI_SSI_COLL.cc
#		LAM_MPI_SSI_COLL_ASSOCIATIVE.cc
#		LAM_MPI_SSI_COLL_CROSSOVER.cc
#		LAM_MPI_SSI_COLL_REDUCE_CROSSOVER.cc
#		LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE.cc
#		LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS.cc
#
#		LAM_UNIVERSE_NCPUS.cc	LAM_UNIVERSE_NNODES.cc
#
#	MPI_ANY_SOURCE.cc	MPI_ANY_TAG.cc	MPI_APPNUM.cc
#	MPI_ARGV_NULL.cc	MPI_ARGVS_NULL.cc
#	MPI_SUBVERSION.cc	MPI_VERSION.cc
#
#	MPI_BAND.cc	MPI_BOR.cc	MPI_BXOR.cc
#	MPI_LAND.cc	MPI_LOR.cc	MPI_LXOR.cc
#	MPI_MAX.cc	MPI_MIN.cc	MPI_OP_NULL.cc
#	MPI_PROD.cc	MPI_REPLACE.cc	MPI_SUM.cc
#
#	MPI_BOTTOM.cc	MPI_BSEND_OVERHEAD.cc
#	MPI_CANCEL_SOURCE.cc
#	MPI_CART.cc	MPI_GRAPH.cc
#	MPI_MAX_ERROR_STRING.cc	MPI_MAX_INFO_KEY.cc
#	MPI_MAX_INFO_VAL.cc	MPI_MAX_NAME_LEN.cc
#	MPI_MAX_OBJECT_NAME.cc	MPI_MAX_PORT_NAME.cc
#	MPI_MAX_PROCESSOR_NAME.cc	MPI_MERGE_TAG.cc
#	MPI_ORDER_C.cc		MPI_ORDER_FORTRAN.cc
#
#	MPI_COMM_NULL.cc	MPI_COMM_SELF.cc	MPI_COMM_WORLD.cc
#	MPI_GROUP_EMPTY.cc	MPI_GROUP_NULL.cc	MPI_REQUEST_NULL.cc
#	MPI_INFO_NULL.cc	MPI_FILE_NULL.cc	MPI_PROC_NULL.cc
#
#	MPI_IDENT.cc	MPI_CONGRUENT.cc
#	MPI_SIMILAR.cc	MPI_UNEQUAL.cc
#
#	MPI_DISTRIBUTE_BLOCK.cc		MPI_DISTRIBUTE_CYCLIC.cc
#	MPI_DISTRIBUTE_DFLT_DARG.cc	MPI_DISTRIBUTE_NONE.cc
#
#	MPI_NULL_COPY_FN.cc	MPI_NULL_DELETE_FN.cc
#	MPI_DUP_FN.cc
#
#	MPI_SUCCESS.cc
#	MPI_ERR_ACCESS.cc	MPI_ERR_AMODE.cc	MPI_ERR_ARG.cc
#	MPI_ERR_ASSERT.cc	MPI_ERR_BAD_FILE.cc	MPI_ERR_BASE.cc
#	MPI_ERR_BUFFER.cc	MPI_ERR_COMM.cc		MPI_ERR_CONVERSION.cc
#	MPI_ERR_COUNT.cc	MPI_ERR_DIMS.cc		MPI_ERR_DISP.cc
#	MPI_ERR_DUP_DATAREP.cc	MPI_ERR_EPOCH.cc	MPI_ERR_FILE.cc
#	MPI_ERR_FILE_EXISTS.cc	MPI_ERR_FILE_IN_USE.cc	MPI_ERR_FLAGS.cc
#	MPI_ERR_GROUP.cc	MPI_ERR_INFO.cc		MPI_ERR_INFO_KEY.cc
#	MPI_ERR_INFO_NOKEY.cc	MPI_ERR_INFO_VALUE.cc	MPI_ERR_INTERN.cc
#	MPI_ERR_IO.cc		MPI_ERR_KEYVAL.cc	MPI_ERR_LASTCODE.cc
#	MPI_ERR_LOCALDEAD.cc	MPI_ERR_LOCKTYPE.cc	MPI_ERR_NAME.cc
#	MPI_ERR_NO_MEM.cc	MPI_ERR_NO_SPACE.cc	MPI_ERR_NO_SUCH_FILE.cc
#	MPI_ERR_NOT_SAME.cc	MPI_ERR_OP.cc		MPI_ERR_OTHER.cc
#	MPI_ERR_PENDING.cc	MPI_ERR_PORT.cc		MPI_ERR_QUOTA.cc
#	MPI_ERR_RANK.cc		MPI_ERR_READ_ONLY.cc	MPI_ERR_REMOTEDEAD.cc
#	MPI_ERR_REQUEST.cc	MPI_ERR_RMA_CONFLICT.cc	MPI_ERR_RMA_SYNC.cc
#	MPI_ERR_ROOT.cc		MPI_ERR_SERVICE.cc	MPI_ERR_SIZE.cc
#	MPI_ERR_SPAWN.cc	MPI_ERR_SYSRESOURCE.cc	MPI_ERR_TAG.cc
#	MPI_ERR_TOPOLOGY.cc	MPI_ERR_TRUNCATE.cc	MPI_ERR_TYPE.cc
#	MPI_ERR_TYPENOTSUP.cc	MPI_ERR_UNKNOWN.cc	
#	MPI_ERR_UNSUPPORTED_DATAREP.cc	MPI_ERR_UNSUPPORTED_OPERATION.cc
#	MPI_ERR_VALUE.cc	MPI_ERR_WIN.cc
#
#	MPI_KEYVAL_INVALID.cc
#
#	MPI_GER_TAG.cc		MPI_IN_PLACE.cc		MPI_IO.cc
#	MPI_HOST.cc		MPI_LASTUSEDCODE.cc	MPI_ROOT.cc
#	MPI_TAG_UB.cc		MPI_UNDEFINED.cc	MPI_UNIVERSE_SIZE.cc
#	MPI_WIN_BASE.cc		MPI_WIN_DISP_UNIT.cc	MPI_WIN_NULL.cc
#	MPI_WIN_SIZE.cc		MPI_WTIME_IS_GLOBAL.cc
#
#	MPI_ERRORS_ARE_FATAL.cc	MPI_ERRORS_RETURN.cc
#
#	MPI_ERR_IN_STATUS.cc
#
#	MPI_ERRHANDLER_NULL.cc
#
#	MPI_ERRCODES_IGNORE.cc
#	MPI_STATUSES_IGNORE.cc		MPI_STATUS_IGNORE.cc
#

  CONS    := $(shell grep '^.include "hConst.h"' *.cc | cut -d: -f1)
  CONOBJS  = $(patsubst %.cc, %.o, $(CONS))
$(CONOBJS): hConst.h

# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h

# ERRS =	MPI_Errhandler_free	MPI_Errhandler_create	\
#		MPI_Errhandler_get	MPI_Errhandler_set
  ERRS    := $(shell grep '^.include "hErr.h"' *.cc | cut -d: -f1)
  ERROBJS  = $(patsubst %.cc, %.o, $(ERRS))
$(ERROBJS): hErr.h

# GRPS =	MPI_Comm_compare	MPI_Comm_create		\
#		MPI_Comm_disconnect	MPI_Comm_dup		\
#		MPI_Comm_free		MPI_Comm_group		\
#		MPI_Comm_remote_group	MPI_Comm_split		\
#		MPI_Group_compare	MPI_Group_difference	\
#		MPI_Group_excl		MPI_Group_range_excl	\
#		MPI_Group_incl		MPI_Group_range_incl	\
#		MPI_Group_free		MPI_Group_intersection	\
#		MPI_Group_rank		MPI_Group_size		\
#		MPI_Group_translate_ranks			\
#		MPI_Group_union		MPI_Intercomm_create
  GRPS    := $(shell grep '^.include "hGrp.h"' *.cc | cut -d: -f1)
  GRPOBJS  = $(patsubst %.cc, %.o, $(GRPS))
$(GRPOBJS): hGrp.h

# INFS =	MPI_Close_port		MPI_Comm_accept		\
#		MPI_Comm_connect	MPI_Info_create		\
#		MPI_Info_delete		MPI_Info_dup		\
#		MPI_Info_free		MPI_Info_get		\
#		MPI_Info_get_nkeys	MPI_Info_get_nthkey	\
#		MPI_Info_get_valuelen	MPI_Info_set		\
#		MPI_Lookup_name		MPI_Open_port		\
#		MPI_Publish_name	MPI_Unpublish_name
  INFS    := $(shell grep '^.include "hInfo.h"' *.cc | cut -d: -f1)
  INFOBJS  = $(patsubst %.cc, %.o, $(INFS))
$(INFOBJS): hInfo.h

# RCVS =	MPI_Recv		MPI_Sendrecv		\
#					MPI_Sendrecv_replace
  RCVS    := $(shell grep '^.include "hRecv.h"' *.cc | cut -d: -f1)
  RCVOBJS  = $(patsubst %.cc, %.o, $(RCVS))
$(RCVOBJS): hRecv.h

# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h

# SNDS =	MPI_Send		MPI_Send_init		\
#		MPI_Bsend		MPI_Bsend_init		\
#		MPI_Rsend		MPI_Rsend_init		\
#		MPI_Ssend		MPI_Ssend_init		\
#		MPI_Isend		MPI_Ibsend		\
#		MPI_Irsend		MPI_Issend		\
#		MPI_Irecv		MPI_Recv_init
  SNDS    := $(shell grep '^.include "hSend.h"' *.cc | cut -d: -f1)
  SNDOBJS  = $(patsubst %.cc, %.o, $(SNDS))
$(SNDOBJS): hSend.h

# SDRS =	- (none)		hSndRecv.h just used from
#		hRecv.h			hSend.h
  SDRS    := $(shell grep '^.include "hSndRecv.h"' *.cc | cut -d: -f1)
  SDROBJS  = $(patsubst %.cc, %.o, $(SDRS))
$(SDROBJS): hSndRecv.h

# STAS =	MPI_Get_count		MPI_Get_elements
  STAS    := $(shell grep '^.include "hStat.h"' *.cc | cut -d: -f1)
  STAOBJS  = $(patsubst %.cc, %.o, $(STAS))
$(STAOBJS): hStat.h

# TOPS =	MPI_Cart_coords		MPI_Cart_create		\
#		MPI_Cart_get		MPI_Cart_map		\
#		MPI_Cart_rank		MPI_Cart_shift		\
#		MPI_Cart_sub		MPI_Graphdims_get	\
#		MPI_Dims_create		MPI_Graph_create	\
#		MPI_Graph_get		MPI_Graph_map		\
#		MPI_Graph_neighbors	MPI_Graph_neighbors_count
  TOPS    := $(shell grep '^.include "hTopo.h"' *.cc | cut -d: -f1)
  TOPOBJS  = $(patsubst %.cc, %.o, $(TOPS))
$(TOPOBJS): hTopo.h

# INCS = hAttr.h hColl.h  hErr.h hInfo.h hSend.h    hStat.h hTstWait.h \
#	 hBuff.h hConst.h hGrp.h hRecv.h hSndRecv.h hTopo.h hType.h    mpitb.h

# TSTS =	MPI_Cancel		MPI_Test_cancelled	\
#		MPI_Probe		MPI_Iprobe		\
#		MPI_Start		MPI_Startall		\
#		MPI_Test		MPI_Testall		\
#		MPI_Testany		MPI_Testsome		\
#		MPI_Wait		MPI_Waitall		\
#		MPI_Waitany		MPI_Waitsome		\
#		MPI_Request_free
  TSTS    := $(shell grep '^.include "hTstWait.h"' *.cc | cut -d: -f1)
  TSTOBJS  = $(patsubst %.cc, %.o, $(TSTS))
$(TSTOBJS): hTstWait.h

# TYPS =	MPI_Get_elements	MPI_Pack_size
  TYPS    := $(shell grep '^.include "hType.h"' *.cc | cut -d: -f1)
  TYPOBJS  = $(patsubst %.cc, %.o, $(TYPS))
$(TYPOBJS): hType.h

# Every MPITB command (Oct-file) source file needs mpitb.h -> 161 files
#   MPIOBJS   Already defined at top / dependency already expressed above
# $(MPIOBJS): mpitb.h
#	check with "grep '^#include "mpitb.h"' *.cc | nl"

# Double-check: all include dependencies are taken into account?
# INCS = hAttr 2 hColl  13 hErr  4 hInfo 16 hSend   14 hStat  2 hTstWait 15
#	 hBuff 5 hConst 25 hGrp 21 hRecv  3 hSndRecv 0 hTopo 14 hType     2 
#	 adds	7+	38+	25+	 19+	    14+	     16+	 17=136
#	check with "grep '^#include "' *.cc | grep -v mpitb | nl"

#################################################################
# List of source C files
# 160 MPIS + mytest.cc checked by "ls MPI_*.cc | nl"
#   3 of them are links: Keyval_free/Attr_put	-> Keyval_create
#			 Errhandler_free	-> Errhandler_create
#################################################################
#MPIS =	\
#	MPI_Init	MPI_Initialized	MPI_Finalize	MPI_Finalized	\
#	MPI_Pack	MPI_Pack_size	MPI_Unpack	MPI_Abort	\
#\
#	MPI_Send	MPI_Bsend	MPI_Ssend	MPI_Rsend	\
#	MPI_Isend	MPI_Ibsend	MPI_Issend	MPI_Irsend	\
#	MPI_Send_init	MPI_Bsend_init	MPI_Ssend_init	MPI_Rsend_init	\
#	MPI_Recv_init	MPI_Start	MPI_Startall	MPI_Request_free\
#	MPI_REQUEST_NULL						\
#\
#	MPI_Buffer_attach		MPI_Buffer_detach		\
#	MPI_Sendrecv			MPI_Sendrecv_replace		\
#	MPI_Cancel			MPI_Test_cancelled		\
#\
#	MPI_Recv	MPI_Irecv	MPI_Probe	MPI_Iprobe	\
#	MPI_Wait	MPI_Waitall	MPI_Waitsome	MPI_Waitany	\
#	MPI_Test	MPI_Testall	MPI_Testsome	MPI_Testany	\
#	MPI_Get_count	MPI_Get_elements		MPI_Address	\
#\
#	MPI_Barrier	MPI_Bcast	MPI_Scatter	MPI_Scatterv	\
#	MPI_Gather	MPI_Gatherv	MPI_Allgather	MPI_Allgatherv	\
#	MPI_Alltoall	MPI_Alltoallv	MPI_Reduce	MPI_Allreduce	\
#	MPI_Scan	MPI_Reduce_scatter				\
#	MPI_MAX		MPI_MIN		MPI_SUM		MPI_PROD	\
#	MPI_REPLACE	MPI_LAND	MPI_LOR		MPI_LXOR	\
#	MPI_OP_NULL	MPI_BAND	MPI_BOR		MPI_BXOR	\
#\
#	MPI_Comm_rank	MPI_Comm_size	MPI_Comm_create	MPI_Comm_free	\
#	MPI_Comm_dup	MPI_Comm_split	MPI_Comm_group	MPI_Comm_test_inter\
#\
#	MPI_Comm_remote_size	MPI_Comm_remote_group	MPI_Comm_get_parent\
#	MPI_Comm_compare	MPI_Intercomm_create	MPI_Intercomm_merge\
#	MPI_COMM_NULL		MPI_COMM_WORLD		MPI_COMM_SELF	\
#\
#	MPI_Group_size	MPI_Group_rank		MPI_Group_free		\
#	MPI_Group_union	MPI_Group_intersection	MPI_Group_difference	\
#	MPI_Group_incl	MPI_Group_range_incl	MPI_Group_compare	\
#	MPI_Group_excl	MPI_Group_range_excl	MPI_Group_translate_ranks\
#	MPI_GROUP_NULL	MPI_GROUP_EMPTY					\
#\
#	MPI_Info_create		MPI_Info_set		MPI_Info_get	\
#	MPI_Info_delete		MPI_Info_get_nkeys	MPI_Info_get_nthkey\
#	MPI_Info_get_valuelen	MPI_Info_dup		MPI_Info_free	\
#	MPI_INFO_NULL							\
#\
#	MPI_Comm_spawn		MPI_Comm_spawn_multiple			\
#	MPI_Comm_connect	MPI_Comm_disconnect			\
#	MPI_Open_port		MPI_Close_port		MPI_Comm_accept	\
#	MPI_Publish_name        MPI_Unpublish_name      MPI_Lookup_name \
#\
#	MPI_Dims_create		MPI_Cart_create		MPI_Graph_create\
#	MPI_Topo_test		MPI_Graphdims_get	MPI_Graph_get	\
#	MPI_Cart_rank		MPI_Cartdim_get		MPI_Cart_get	\
#	MPI_Cart_map		MPI_Cart_shift		MPI_Cart_sub	\
#	MPI_Graph_map		MPI_Cart_coords				\
#	MPI_Graph_neighbors	MPI_Graph_neighbors_count		\
#\
#	MPI_Errhandler_create	MPI_Errhandler_free			\
#	MPI_Errhandler_set	MPI_Errhandler_get			\
#	MPI_ERRORS_ARE_FATAL	MPI_ERRORS_RETURN	MPI_ERRHANDLER_NULL\
#	MPI_Error_class		MPI_Error_string			\
#\
#	MPI_Get_processor_name	MPI_Get_version		MPI_Wtick	\
#	MPI_Keyval_create	MPI_Keyval_free		MPI_Wtime	\
#	MPI_Attr_put		MPI_Attr_get		MPI_Attr_delete	\
#	MPI_NULL_DELETE_FN	MPI_NULL_COPY_FN	MPI_DUP_FN	\
#\
#	mytest
#################################################################
