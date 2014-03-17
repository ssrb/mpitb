% COMMS                 Communicator management
%
% MPI_Comm_size          Determines size of group associated with communicator
% MPI_Comm_rank          Determines rank of calling process in communicator
% MPI_Comm_compare       Compares two communicators
% MPI_Comm_dup           Duplicates an existing comm. with all its cached info
% MPI_Comm_create        Creates a new communicator (changing group)
% MPI_Comm_split         Creates new communicators based on colors and keys
% MPI_Comm_free          Marks the communicator object for deallocation
%
% MPI_Comm_test_inter    Tests to see if a comm is an inter-communicator
% MPI_Comm_remote_size   Determines size of remote group assoc. with intercomm
% MPI_Comm_remote_group  Accesses the remote group assoc. with given intercomm
% MPI_Intercomm_create   Creates an intercommunicator from two intracomms
% MPI_Intercomm_merge    Creates intracomm (1 group) from an intercomm (2 grps)
%
% -------------------
% Predefined Symbols:
% -------------------
% MPI_COMM_WORLD         MPITB constant for corresponding communicator object
% MPI_COMM_SELF          MPITB constant for corresponding communicator object
% MPI_COMM_NULL          MPITB constant for corresponding communicator object
%
% MPI_IDENT              MPITB constant for 'identical' comparison result
% MPI_CONGRUENT          MPITB constant for 'congruent' comparison result
% MPI_SIMILAR            MPITB constant for 'similar' comparison result
% MPI_UNEQUAL            MPITB constant for 'unequal' comparison result
% 
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% typedef struct _comm            *MPI_Comm;
% #define LAM_MPI_C_COMM_NULL     ((MPI_Comm) 0)
% #define MPI_COMM_NULL           LAM_MPI_C_COMM_NULL
% #define MPI_COMM_WORLD          ((MPI_Comm) &lam_mpi_comm_world)
% #define MPI_COMM_SELF           ((MPI_Comm) &lam_mpi_comm_self)
%
% #define MPI_IDENT               1
% #define MPI_CONGRUENT           2
% #define MPI_SIMILAR             3
% #define MPI_UNEQUAL             4
%
% --------------
% OpenMPI-1.2.3:
% --------------
% #define MPI_COMM_NULL (&ompi_mpi_comm_null)
% #define MPI_COMM_WORLD (&ompi_mpi_comm_world)
% #define MPI_COMM_SELF (&ompi_mpi_comm_self)
%
% enum {
%   MPI_IDENT,
%   MPI_CONGRUENT,
%   MPI_SIMILAR,
%   MPI_UNEQUAL
% };
%
