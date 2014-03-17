% HANDLES               Some MPI-predefined and assorted NULL values
%
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% #define MPI_GROUP_NULL          LAM_MPI_C_GROUP_NULL
% #define MPI_COMM_NULL           LAM_MPI_C_COMM_NULL
% #define MPI_REQUEST_NULL        LAM_MPI_C_REQUEST_NULL
% #define MPI_OP_NULL             LAM_MPI_C_OP_NULL
% #define MPI_ERRHANDLER_NULL     LAM_MPI_C_ERRHANDLER_NULL
% #define MPI_NULL_COPY_FN        LAM_MPI_C_NULL_COPY_FN
% #define MPI_NULL_DELETE_FN      LAM_MPI_C_NULL_DELETE_FN
% #define MPI_INFO_NULL           LAM_MPI_C_INFO_NULL
% #define MPI_WIN_NULL            LAM_MPI_C_WIN_NULL
% 
% #define MPI_STATUSES_IGNORE     LAM_MPI_C_STATUSES_IGNORE
% #define MPI_STATUS_IGNORE       LAM_MPI_C_STATUS_IGNORE
% 
% #define MPI_COMM_WORLD          ((MPI_Comm) &lam_mpi_comm_world)
% #define MPI_COMM_SELF           ((MPI_Comm) &lam_mpi_comm_self)
% 
% #define MPI_GROUP_EMPTY         ((MPI_Group) &lam_mpi_group_empty)
% 
% #define MPI_MAX                 ((MPI_Op) &lam_mpi_max)
% #define MPI_MIN                 ((MPI_Op) &lam_mpi_min)
% #define MPI_SUM                 ((MPI_Op) &lam_mpi_sum)
% #define MPI_PROD                ((MPI_Op) &lam_mpi_prod)
% #define MPI_LAND                ((MPI_Op) &lam_mpi_land)
% #define MPI_BAND                ((MPI_Op) &lam_mpi_band)
% #define MPI_LOR                 ((MPI_Op) &lam_mpi_lor)
% #define MPI_BOR                 ((MPI_Op) &lam_mpi_bor)
% #define MPI_LXOR                ((MPI_Op) &lam_mpi_lxor)
% #define MPI_BXOR                ((MPI_Op) &lam_mpi_bxor)
% #define MPI_REPLACE             ((MPI_Op) &lam_mpi_replace)
% 
% #define MPI_ERRORS_ARE_FATAL    ((MPI_Errhandler) &lam_mpi_errors_are_fatal)
% #define MPI_ERRORS_RETURN       ((MPI_Errhandler) &lam_mpi_errors_return)
% 
% extern int MPI_DUP_FN __ARGS((MPI_Comm, int, void *,void *, void *, int *));
% 
% --------------
% OpenMPI-1.2.3:
% --------------
% #define MPI_GROUP_NULL (&ompi_mpi_group_null)
% #define MPI_COMM_NULL (&ompi_mpi_comm_null)
% #define MPI_REQUEST_NULL (&ompi_request_null)
% #define MPI_OP_NULL (&ompi_mpi_op_null)
% #define MPI_ERRHANDLER_NULL (&ompi_mpi_errhandler_null)
% #define MPI_INFO_NULL (&ompi_mpi_info_null)
% #define MPI_WIN_NULL (&ompi_mpi_win_null)
% #define MPI_FILE_NULL (&ompi_mpi_file_null)
% 
% #define MPI_STATUS_IGNORE ((MPI_Status *) 0)
% #define MPI_STATUSES_IGNORE ((MPI_Status *) 0)
% 
% #define MPI_NULL_DELETE_FN OMPI_C_MPI_NULL_DELETE_FN
% #define MPI_NULL_COPY_FN OMPI_C_MPI_NULL_COPY_FN
% 
% #define MPI_COMM_WORLD (&ompi_mpi_comm_world)
% #define MPI_COMM_SELF (&ompi_mpi_comm_self)
%  
% #define MPI_GROUP_EMPTY (&ompi_mpi_group_empty)
%  
% #define MPI_MAX (&ompi_mpi_op_max)
% #define MPI_MIN (&ompi_mpi_op_min)
% #define MPI_SUM (&ompi_mpi_op_sum)
% #define MPI_PROD (&ompi_mpi_op_prod)
% #define MPI_LAND (&ompi_mpi_op_land)
% #define MPI_BAND (&ompi_mpi_op_band)
% #define MPI_LOR (&ompi_mpi_op_lor)
% #define MPI_BOR (&ompi_mpi_op_bor)
% #define MPI_LXOR (&ompi_mpi_op_lxor)
% #define MPI_BXOR (&ompi_mpi_op_bxor)
% #define MPI_REPLACE (&ompi_mpi_op_replace)
% 
% #define MPI_ERRORS_ARE_FATAL (&ompi_mpi_errors_are_fatal)
% #define MPI_ERRORS_RETURN (&ompi_mpi_errors_return)
% 
% #define MPI_DUP_FN OMPI_C_MPI_DUP_FN
% int OMPI_C_MPI_DUP_FN( MPI_Comm comm, int comm_keyval,
%     void* extra_state, void* attribute_val_in, void* attribute_val_out,
%     int* flag );
%
