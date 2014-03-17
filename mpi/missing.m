%MISSING		MPITB - Missing bindings
%
% Function calls
%
%int MPI_Op_create ((MPI_User_function *, int, MPI_Op *);
%int MPI_Op_free ((MPI_Op *);
%int MPI_Pcontrol ((int level, ...);
%
%int MPI_Type_extent( MPI_Datatype datatype, MPI_Aint *extent )
%int MPI_Type_size  ( MPI_Datatype datatype, int *size )
%int MPI_Type_lb    ( MPI_Datatype datatype, MPI_Aint *displacement )
%int MPI_Type_ub    ( MPI_Datatype datatype, MPI_Aint *displacement )
%int MPI_Type_free  ( MPI_Datatype *datatype )
%int MPI_Type_commit ( MPI_Datatype *datatype )
%int MPI_Type_contiguous(int count,MPI_Datatype old_type,MPI_Datatype *newtype)
%int MPI_Type_vector(int count,int blocklen,int stride,MPI_Datatype old_type,
%						      MPI_Datatype *newtype )
%int MPI_Type_hvector(int count,blocklen,MPI_Aint stride,MPI_Datatype old_type,
%							MPI_Datatype *newtype )
%int MPI_Type_indexed(int count,blocklens[],int indices[],MPI_Datatype old_type,
%							 MPI_Datatype *newtype )
%int MPI_Type_hindexed(int count,int blocklens[],MPI_Aint indices[],
%				MPI_Datatype old_type, MPI_Datatype *newtype )
%int MPI_Type_struct(int count,int blocklens[],MPI_Aint indices[],
%				MPI_Datatype old_types[],MPI_Datatype *newtype )
%
%int MPI_COMM_DUP_FN (MPI_Comm,     int, void *, void *, void *, int *);
%int MPI_TYPE_DUP_FN (MPI_Datatype, int, void *, void *, void *, int *);
%int MPI_WIN_DUP_FN  (MPI_Win,      int, void *, void *, void *, int *);
%int MPI_Comm_create_errhandler (MPI_Comm_errhandler_fn *, MPI_Errhandler *);
%int MPI_Comm_get_errhandler    (MPI_Comm, MPI_Errhandler *);
%int MPI_Comm_set_errhandler    (MPI_Comm, MPI_Errhandler);
%
%MPI_Comm  MPI_Comm_f2c      (MPI_Fint);
%MPI_Fint  MPI_Comm_c2f      (MPI_Comm);
%MPI_Group MPI_Group_f2c     (MPI_Fint);
%MPI_Fint  MPI_Group_c2f     (MPI_Group);
%MPI_Datatype MPI_Type_f2c   (MPI_Fint);
%MPI_Fint  MPI_Type_c2f      (MPI_Datatype);
%MPI_Request MPI_Request_f2c (MPI_Fint);
%MPI_Fint  MPI_Request_c2f   (MPI_Request);
%MPI_Info  MPI_Info_f2c      (MPI_Fint);
%MPI_Fint  MPI_Info_c2f      (MPI_Info);
%MPI_Win   MPI_Win_f2c       (MPI_Fint);
%MPI_Fint  MPI_Win_c2f       (MPI_Win);
%int       MPI_Status_f2c (MPI_Fint *, MPI_Status *);
%int       MPI_Status_c2f (MPI_Status *, MPI_Fint *);
%
%int MPI_Type_dup           (MPI_Datatype, MPI_Datatype *);
%int MPI_Type_delete_attr   (MPI_Datatype, int);
%int MPI_Type_get_attr      (MPI_Datatype, int, void *, int *);
%int MPI_Type_set_attr      (MPI_Datatype, int, void *);
%int MPI_Type_create_resized(MPI_Datatype, MPI_Aint, MPI_Aint, MPI_Datatype *);
%int MPI_Type_create_keyval (MPI_Type_copy_attr_function *,
%                            MPI_Type_delete_attr_function *, int *, void *);
%int MPI_Comm_delete_attr   (MPI_Comm, int);
%int MPI_Comm_get_attr      (MPI_Comm, int, void *, int *);
%int MPI_Comm_set_attr      (MPI_Comm, int, void *);
%int MPI_Comm_create_keyval (MPI_Comm_copy_attr_function *,
%                            MPI_Comm_delete_attr_function *, int *, void *);
%int MPI_Win_delete_attr    (MPI_Win, int);
%int MPI_Win_get_attr       (MPI_Win, int, void *, int *);
%int MPI_Win_set_attr       (MPI_Win, int, void *);
%int MPI_Win_create_keyval  (MPI_Win_copy_attr_function *,
%                            MPI_Win_delete_attr_function *, int *, void*);
%
%int MPI_Win_create_errhandler(MPI_Win_errhandler_fn *, MPI_Errhandler *);
%int MPI_Win_get_errhandler   (MPI_Win, MPI_Errhandler *);
%int MPI_Win_set_errhandler   (MPI_Win, MPI_Errhandler);
%
%int MPI_Win_create (void *, MPI_Aint, int, MPI_Info, MPI_Comm, MPI_Win *);
%int MPI_Win_free   (MPI_Win *);
%int MPI_Win_fence  (int, MPI_Win);
%int MPI_Accumulate (void *, int, MPI_Datatype, int, MPI_Aint,
%                            int, MPI_Datatype, MPI_Op, MPI_Win);
%int MPI_Get        (void *, int, MPI_Datatype, int, MPI_Aint,
%                            int, MPI_Datatype, MPI_Win);
%int MPI_Put        (void *, int, MPI_Datatype, int, MPI_Aint,
%                            int, MPI_Datatype, MPI_Win);
%int MPI_Win_post     (MPI_Group, int, MPI_Win);
%int MPI_Win_start    (MPI_Group, int, MPI_Win);
%int MPI_Win_get_group(MPI_Win, MPI_Group *);
%int MPI_Win_wait     (MPI_Win);
%int MPI_Win_complete (MPI_Win);
%
%int MPI_Type_get_envelope   (MPI_Datatype,int*,int*,int*,int*);
%int MPI_Type_get_contents   (MPI_Datatype,int ,int ,int ,int*,
%                                                    MPI_Aint*,MPI_Datatype*);
%int MPI_Type_create_struct  (int,int*,MPI_Aint*,MPI_Datatype*,MPI_Datatype*);
%int MPI_Type_create_hindexed(int,int*,MPI_Aint*,MPI_Datatype ,MPI_Datatype*);
%int MPI_Type_create_hvector (int,int ,MPI_Aint ,MPI_Datatype ,MPI_Datatype*);
%int MPI_Type_create_subarray(int,int*,int*,int*,
%                                           int ,MPI_Datatype,MPI_Datatype*);
%int MPI_Type_create_darray  (int,int,int,int*,int*,int*,int*, 
%                                           int ,MPI_Datatype,MPI_Datatype*);
%
%int MPI_Get_address     (void *, MPI_Aint *);
%int MPI_Comm_join       (int, MPI_Comm *);
%int MPI_Comm_get_parent (MPI_Comm *);
%int MPI_Comm_free_keyval(int *);
%int MPI_Type_free_keyval(int *);
%int MPI_Win_free_keyval (int *);
%int MPI_Type_get_extent     (MPI_Datatype, MPI_Aint *, MPI_Aint *);
%int MPI_Type_get_true_extent(MPI_Datatype, MPI_Aint *, MPI_Aint *);
%                                        
%int MPI_Comm_get_name (MPI_Comm,    char*,int*);
%int MPI_Comm_set_name (MPI_Comm,    char*);
%int MPI_Type_get_name (MPI_Datatype,char*,int*);
%int MPI_Type_set_name (MPI_Datatype,char*);
%int MPI_Win_get_name  (MPI_Win,     char*,int*);
%int MPI_Win_set_name  (MPI_Win,     char*);
%
% Constants
%
%#define MPI_BYTE                ((MPI_Datatype) &lam_mpi_byte)
%#define MPI_PACKED              ((MPI_Datatype) &lam_mpi_packed)
%#define MPI_CHAR                ((MPI_Datatype) &lam_mpi_char)
%#define MPI_SHORT               ((MPI_Datatype) &lam_mpi_short)
%#define MPI_INT                 ((MPI_Datatype) &lam_mpi_int)
%#define MPI_LONG                ((MPI_Datatype) &lam_mpi_long)
%#define MPI_FLOAT               ((MPI_Datatype) &lam_mpi_float)
%#define MPI_DOUBLE              ((MPI_Datatype) &lam_mpi_double)
%#define MPI_LONG_DOUBLE         ((MPI_Datatype) &lam_mpi_long_double)
%#define MPI_UNSIGNED_CHAR       ((MPI_Datatype) &lam_mpi_unsigned_char)
%#define MPI_UNSIGNED_SHORT      ((MPI_Datatype) &lam_mpi_unsigned_short)
%#define MPI_UNSIGNED_LONG       ((MPI_Datatype) &lam_mpi_unsigned_long)
%#define MPI_UNSIGNED            ((MPI_Datatype) &lam_mpi_unsigned)
%#define MPI_FLOAT_INT           ((MPI_Datatype) &lam_mpi_float_int)
%#define MPI_DOUBLE_INT          ((MPI_Datatype) &lam_mpi_double_int)
%#define MPI_LONG_DOUBLE_INT     ((MPI_Datatype) &lam_mpi_longdbl_int)
%#define MPI_LONG_INT            ((MPI_Datatype) &lam_mpi_long_int)
%#define MPI_SHORT_INT           ((MPI_Datatype) &lam_mpi_short_int)
%#define MPI_2INT                ((MPI_Datatype) &lam_mpi_2int)
%#define MPI_UB                  ((MPI_Datatype) &lam_mpi_ub)
%#define MPI_LB                  ((MPI_Datatype) &lam_mpi_lb)
%#define MPI_WCHAR               ((MPI_Datatype) &lam_mpi_wchar)
%#define MPI_LONG_LONG_INT       ((MPI_Datatype) &lam_mpi_long_long_int)
%#define MPI_UNSIGNED_LONG_LONG  ((MPI_Datatype) &lam_mpi_unsigned_long_long)
%
%#define LAM_MPI                 1
%#define MPI_GER_TAG             -2              /* used for GER protocol */
%#define MPI_MERGE_TAG           -3              /* used to merge inter-comm */
%#define MPI_MAX_CID             0xFFE           /* max pt2pt context ID */
%#define MPI_GER_CID             0xFFF           /* used for GER protocol */
%#define MPI_MAX_PROCESSOR_NAME  256             /* max proc. name length */
%#define MPI_MAX_ERROR_STRING    256             /* max error message length */
%#define MPI_MAX_OBJECT_NAME     64              /* max object name length */
%#define MPI_BOTTOM              ((void *) 0)    /* base reference address */
%#define MPI_MAX_INFO_KEY        36              /* max info key length */
%#define MPI_MAX_INFO_VAL        256             /* max info value length */
%#define MPI_ARGV_NULL           ((char **) 0)   /* NULL argument vector */
%#define MPI_ARGVS_NULL          ((char ***) 0)  /* NULL argument vectors */
%#define MPI_ERRCODES_IGNORE     ((void *) 0)    /* don't return error codes */
%#define MPI_MAX_PORT_NAME       36              /* max port name length */
%#define MPI_MAX_NAME_LEN        MPI_MAX_PORT_NAME /* max port name length */
%#define MPI_ORDER_C             0               /* C row major order */
%#define MPI_ORDER_FORTRAN       1               /* Fortran column major ord */
%#define MPI_DISTRIBUTE_BLOCK    0               /* block distribution */
%#define MPI_DISTRIBUTE_CYCLIC   1               /* cyclic distribution */
%#define MPI_DISTRIBUTE_NONE     2               /* not distributed */
%#define MPI_DISTRIBUTE_DFLT_DARG (-1)           /* default distribution arg */
%
%#define MPI_DATATYPE_NULL       ((MPI_Datatype) 0)
%#define MPI_COMM_NULL_COPY_FN   ((MPI_Comm_copy_attr_function *) 0)
%#define MPI_COMM_NULL_DELETE_FN ((MPI_Comm_delete_attr_function *) 0)
%#define MPI_TYPE_NULL_COPY_FN   ((MPI_Type_copy_attr_function *) 0)
%#define MPI_TYPE_NULL_DELETE_FN ((MPI_Type_delete_attr_function *) 0)
%#define MPI_WIN_NULL_COPY_FN    ((MPI_Win_copy_attr_function *) 0)
%#define MPI_WIN_NULL_DELETE_FN  ((MPI_Win_delete_attr_function *) 0)
%#define MPI_WIN_NULL            ((MPI_Win) 0)
%
%#define MPI_STATUSES_IGNORE     ((MPI_Status *) 0)
%#define MPI_STATUS_IGNORE       ((MPI_Status *) 0)
%#define MPI_COMBINER_NAMED              0
%#define MPI_COMBINER_DUP                1
%#define MPI_COMBINER_CONTIGUOUS         2
%#define MPI_COMBINER_VECTOR             3
%#define MPI_COMBINER_HVECTOR_INTEGER    4
%#define MPI_COMBINER_HVECTOR            5
%#define MPI_COMBINER_INDEXED            6
%#define MPI_COMBINER_HINDEXED_INTEGER   7
%#define MPI_COMBINER_HINDEXED           8
%#define MPI_COMBINER_INDEXED_BLOCK      9
%#define MPI_COMBINER_STRUCT_INTEGER     10
%#define MPI_COMBINER_STRUCT             11
%#define MPI_COMBINER_SUBARRAY           12
%#define MPI_COMBINER_DARRAY             13
%#define MPI_COMBINER_F90_REAL           14
%#define MPI_COMBINER_F90_COMPLEX        15
%#define MPI_COMBINER_F90_INTEGER        16
%#define MPI_COMBINER_RESIZED            17
%
%#define MPI_MAXLOC              ((MPI_Op) &lam_mpi_maxloc)
%#define MPI_MINLOC              ((MPI_Op) &lam_mpi_minloc)
%

