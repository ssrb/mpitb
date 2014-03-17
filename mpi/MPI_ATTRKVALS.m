%MPI_ATTRKVALS		List of predefined attribute key values
%
% These are some remarks found in LAM source code WRT:
%	MPI_IO   MPI_HOST   MPI_UNIVERSE_SIZE
%
% * We interpret MPI_IO to include the ability to print to a terminal.
% * MPI_IO is set to my rank if I'm on the origin node.
% * Otherwise, it's set to MPI_ANY_SOURCE if all processes are on
% * the origin node, MPI_PROC_NULL if no processes are on the
% * origin node or the lowest ranked process on the origin node.
%
% * MPI_HOST is similarly related to the origin node, except if all
% * processes are on the origin, we set this attribute to rank 0.
%
% * The universe size is inherited from the parents or is the number of
% * CPUs in the LAM.
%
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% #define MPI_TAG_UB              0
% #define MPI_HOST                1
% #define MPI_IO                  2
% #define MPI_WTIME_IS_GLOBAL     3
% #define MPI_UNIVERSE_SIZE       4
% #define MPI_APPNUM              5
% #define MPI_WIN_BASE            6
% #define MPI_WIN_SIZE            7
% #define MPI_WIN_DISP_UNIT       8
% 
% #define IMPI_CLIENT_SIZE        9
% #define IMPI_CLIENT_COLOR       10
% #define IMPI_HOST_SIZE          11
% #define IMPI_HOST_COLOR         12
% 
% #define LAM_UNIVERSE_NCPUS      13			(missing in OpenMPI)
% #define LAM_UNIVERSE_NNODES     14			(...)
% 
% #define LAM_MPI_SSI_COLL                    15	(missing in OpenMPI)
% #define LAM_MPI_SSI_COLL_CROSSOVER          16	(...)
% #define LAM_MPI_SSI_COLL_ASSOCIATIVE        17
% #define LAM_MPI_SSI_COLL_REDUCE_CROSSOVER   18
% #define LAM_MPI_SSI_COLL_SHMEM_MESSAGE_POOL_SIZE 19
% #define LAM_MPI_SSI_COLL_SHMEM_NUM_SEGMENTS 20	(...)
% 
% --------------
% OpenMPI-1.2.3:
% --------------
% enum {			  OpenMPI LAM/MPI
%     /* MPI-1 */
%     MPI_TAG_UB,			 0	 0
%     MPI_HOST,				 1	 1
%     MPI_IO,				 2	 2
%     MPI_WTIME_IS_GLOBAL,		 3	 3
%     /* MPI-2 */
%     MPI_APPNUM,			 4	 5
%     MPI_LASTUSEDCODE,			 5		? (missing in LAM)
%     MPI_UNIVERSE_SIZE,		 6	 4
%     MPI_WIN_BASE,			 7	 6
%     MPI_WIN_SIZE,			 8	 7
%     MPI_WIN_DISP_UNIT,		 9	 8
%     /* Even though these four are IMPI attributes... */
%     IMPI_CLIENT_SIZE,			10	 9
%     IMPI_CLIENT_COLOR,		11	10
%     IMPI_HOST_SIZE,			12	11
%     IMPI_HOST_COLOR			13	12
% };
% 
