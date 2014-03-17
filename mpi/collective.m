% COLLECTIVE            Collective communication
%
% MPI_Barrier            Blocks until all process have reached this routine
% MPI_Bcast              Broadcasts msg from "root" to all others in group
% MPI_Gather             Gathers together values from a group of processes
% MPI_Gatherv            Gathers into spcfied locations from all ranks in comm
% MPI_Scatter            Sends data from one task to all other tasks in a group
% MPI_Scatterv           Scatters a buffer in parts to all tasks in a group
%
% MPI_Allgather          Gathers data from all tasks and distribute it to all
% MPI_Allgatherv         Gathers data from all tasks and deliver it to all
% MPI_Alltoall           Sends data from all to all processes
% MPI_Alltoallv          Sends data from all to all processes with displacements
%
% MPI_Reduce             Reduces values on all processes to a single value
% MPI_Allreduce          Combines values from all/distribute result back to all
% MPI_Reduce_scatter     Combines values and scatters the results
% MPI_Scan               Computes partial reductions of data from lesser ranks
%
% -------------------
% Predefined Symbols:
% -------------------
% MPI_MAX                MPITB constant for corresponding MPI_Op object
% MPI_MIN                MPITB constant for corresponding MPI_Op object
% MPI_SUM                MPITB constant for corresponding MPI_Op object
% MPI_PROD               MPITB constant for corresponding MPI_Op object
% MPI_LAND               MPITB constant for corresponding MPI_Op object
% MPI_BAND               MPITB constant for corresponding MPI_Op object
% MPI_LOR                MPITB constant for corresponding MPI_Op object
% MPI_BOR                MPITB constant for corresponding MPI_Op object
% MPI_LXOR               MPITB constant for corresponding MPI_Op object
% MPI_BXOR               MPITB constant for corresponding MPI_Op object
% MPI_REPLACE            MPITB constant for corresponding MPI_Op object
% MPI_OP_NULL            MPITB constant for corresponding MPI_Op object
%
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% typedef struct _op              *MPI_Op;
% #define LAM_MPI_C_OP_NULL       ((MPI_Op) 0)
% #define MPI_OP_NULL             LAM_MPI_C_OP_NULL
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
% #define MPI_MAXLOC              ((MPI_Op) &lam_mpi_maxloc)	*** missing
% #define MPI_MINLOC              ((MPI_Op) &lam_mpi_minloc)	*** missing
% #define MPI_REPLACE             ((MPI_Op) &lam_mpi_replace)
% 
% --------------
% OpenMPI-1.2.3:
% --------------
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
% #define MPI_MAXLOC (&ompi_mpi_op_maxloc)	*** missing from MPITB
% #define MPI_MINLOC (&ompi_mpi_op_minloc)	*** missing from MPITB
% #define MPI_REPLACE (&ompi_mpi_op_replace)
%
