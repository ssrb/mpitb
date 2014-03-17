% MISC                  Startup, Timers, Sinchronization, Miscellany
%
% MPI_Init               Initialize the MPI execution environment
% MPI_Initialized        Indicates whether MPI_Init has been called
% MPI_Finalize           Terminates MPI execution environment
% MPI_Abort              Terminates MPI execution environment
%
% MPI_Wtick              Returns the resolution of MPI_Wtime
% MPI_Wtime              Returns an elapsed time on the calling processor
%
% MPI_Address            Gets the address of a MATLAB variable's data in memory
% MPI_Get_version        Obtain the version number of MPI that LAM/MPI supports
% 
% -------------------
% Predefined Symbols:
% -------------------
% LAM_MPI / OPEN_MPI     0/1 depending on subjacent system
% MPI_VERSION            Major version number
% MPI_SUBVERSION         Minor version number
%
% MPI_SUCCESS
% MPI_ANY_SOURCE
% ...
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% #define LAM_MPI                 1
% #define MPI_VERSION             1
% #define MPI_SUBVERSION          2
% 
% #define MPI_ANY_SOURCE          -1              /* match any source rank */
% #define MPI_PROC_NULL           -2              /* rank of null process */
% #define MPI_CANCEL_SOURCE       -3              /* successful cancel */
% #define MPI_ROOT                -4
% #define MPI_ANY_TAG             -1              /* match any message tag */
% #define MPI_GER_TAG             -2              /* used for GER protocol */
% #define MPI_MERGE_TAG           -3              /* used to merge inter-comm */
% #define MPI_MAX_PROCESSOR_NAME  256             /* max proc. name length */
% #define MPI_MAX_ERROR_STRING    256             /* max error message length */
% #define MPI_MAX_OBJECT_NAME     64              /* max object name length */
% #define MPI_UNDEFINED           -32766          /* undefined stuff */
% #define MPI_CART                1               /* cartesian topology */
% #define MPI_GRAPH               2               /* graph topology */
% #define MPI_KEYVAL_INVALID      -1              /* invalid key value */
%
% #define MPI_BOTTOM              ((void *) 0) /* base reference address */
% #define MPI_BSEND_OVERHEAD      40           /* size of bsend header + ptr */
% #define MPI_MAX_INFO_KEY        36              /* max info key length */
% #define MPI_MAX_INFO_VAL        256             /* max info value length */
% #define MPI_ARGV_NULL           ((char **) 0)   /* NULL argument vector */
% #define MPI_ARGVS_NULL          ((char ***) 0)  /* NULL argument vectors */
% #define MPI_ERRCODES_IGNORE     ((int *) 0)  /* don't return error codes */
% #define MPI_MAX_PORT_NAME       36              /* max port name length */
% #define MPI_MAX_NAME_LEN        MPI_MAX_PORT_NAME /* max port name length */
% #define MPI_ORDER_C             0               /* C row major order */
% #define MPI_ORDER_FORTRAN       1            /* Fortran column major order */
% #define MPI_DISTRIBUTE_BLOCK    0               /* block distribution */
% #define MPI_DISTRIBUTE_CYCLIC   1               /* cyclic distribution */
% #define MPI_DISTRIBUTE_NONE     2               /* not distributed */
% #define MPI_DISTRIBUTE_DFLT_DARG (-1)        /* default distribution arg */
% 
% --------------
% OpenMPI-1.2.3:
% --------------
% #define OPEN_MPI 1
% #define MPI_VERSION 2
% #define MPI_SUBVERSION 0
% 
% #define MPI_ANY_SOURCE         -1      /* match any source rank */
% #define MPI_PROC_NULL          -2      /* rank of null process */
% #define MPI_ROOT               -4
% #define MPI_ANY_TAG            -1      /* match any message tag */
% #define MPI_MAX_PROCESSOR_NAME 256     /* max proc. name length */
% #define MPI_MAX_ERROR_STRING   256     /* max error message length */
% #define MPI_MAX_OBJECT_NAME    64      /* max object name length */
% #define MPI_UNDEFINED          -32766  /* undefined stuff */
% #define MPI_CART               1       /* cartesian topology */
% #define MPI_GRAPH              2       /* graph topology */
% #define MPI_KEYVAL_INVALID     -1      /* invalid key value */
% 
% #define MPI_BOTTOM               ((void *) 0)    /* base reference address */
% #define MPI_IN_PLACE             ((void *) 1)    /* in place buffer */
% #define MPI_BSEND_OVERHEAD       128     /* size of bsend header + ptr */
% #define MPI_MAX_INFO_KEY         36      /* max info key length */
% #define MPI_MAX_INFO_VAL         256     /* max info value length */
% #define MPI_ARGV_NULL            ((char **) 0)   /* NULL argument vector */
% #define MPI_ARGVS_NULL           ((char ***) 0) /* NULL argument vectors */
% #define MPI_ERRCODES_IGNORE      ((int *) 0)   /* don't return error codes */
% #define MPI_MAX_PORT_NAME        36      /* max port name length */
% #define MPI_MAX_NAME_LEN         MPI_MAX_PORT_NAME /* max port name length */
% #define MPI_ORDER_C              0       /* C row major order */
% #define MPI_ORDER_FORTRAN        1       /* Fortran column major order */
% #define MPI_DISTRIBUTE_BLOCK     0       /* block distribution */
% #define MPI_DISTRIBUTE_CYCLIC    1       /* cyclic distribution */
% #define MPI_DISTRIBUTE_NONE      2       /* not distributed */
% #define MPI_DISTRIBUTE_DFLT_DARG (-1)    /* default distribution arg */
% 
