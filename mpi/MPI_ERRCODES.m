%MPI_ERRCODES
%
% All symbols defined in MPITB, with value MPI_SUCESS==0 if not present in
% underlying MPI implementation (e.g. MPI_ERR_ACCESS=0 under LAM/MPI)
%
% -----------------------------
% Differences as of versions...
% -----------------------------
% LAM/MPI-7.1.3:
% --------------
% #define MPI_SUCCESS             0               /* no errors */
% #define MPI_ERR_BUFFER          1       /* invalid buffer pointer */
% #define MPI_ERR_COUNT           2       /* invalid count argument */
% #define MPI_ERR_TYPE            3       /* invalid datatype argument */
% #define MPI_ERR_TAG             4       /* invalid tag argument */
% #define MPI_ERR_COMM            5       /* invalid communicator */
% #define MPI_ERR_RANK            6       /* invalid rank */
% #define MPI_ERR_REQUEST         7       /* invalid request handle */
% #define MPI_ERR_ROOT            8       /* invalid root */
% #define MPI_ERR_GROUP           9       /* invalid group */
% #define MPI_ERR_OP              10      /* invalid operation */
% #define MPI_ERR_TOPOLOGY        11      /* invalid topology */
% #define MPI_ERR_DIMS            12      /* invalid dimension argument */
% #define MPI_ERR_ARG             13      /* invalid argument */
% #define MPI_ERR_UNKNOWN         14      /* unknown error */
% #define MPI_ERR_TRUNCATE        15      /* message truncated on receive */
% #define MPI_ERR_OTHER           16      /* LAM error */
% #define MPI_ERR_INTERN          17      /* internal MPI error */
% #define MPI_ERR_IN_STATUS       18      /* error code is in status */
% #define MPI_ERR_PENDING         19      /* pending request */
% #define MPI_ERR_SYSRESOURCE     20      /* out of system resources */
% #define MPI_ERR_LOCALDEAD       21      /* process in local group is dead */
% #define MPI_ERR_REMOTEDEAD      22      /* process in remote group is dead */
% #define MPI_ERR_VALUE           23      /* truncated info value */
% #define MPI_ERR_FLAGS           24      /* mismatched run-time flags */
% #define MPI_ERR_SERVICE         25      /* name publishing service error */
% #define MPI_ERR_NAME            26      /* name not published */
% #define MPI_ERR_SPAWN           27      /* error while spawning processes */
% #define MPI_ERR_KEYVAL          28      /* invalid key value */
% #define MPI_ERR_INFO_NOKEY      29      /* no such info key */
% #define MPI_ERR_WIN             30      /* invalid window */
% #define MPI_ERR_EPOCH           31      /* mismatched one-sided synch. */
% #define MPI_ERR_TYPENOTSUP      32      /* operation not supported on type */
% #define MPI_ERR_INFO_KEY        33      /* invalid info key */
% #define MPI_ERR_INFO_VALUE      34      /* invalid info value */
% #define MPI_ERR_NO_MEM          35      /* no memory left */
% #define MPI_ERR_BASE            36      /* invalid base pointer value */
% #define MPI_ERR_LASTCODE        37      /* last error code */
%
% --------------
% OpenMPI-1.2.3:				Comments from MPITB authors:
% --------------
% #define MPI_SUCCESS                   0
% #define MPI_ERR_BUFFER                1	= (identical to LAM-7.1.3)
% #define MPI_ERR_COUNT                 2	= (up to...)
% #define MPI_ERR_TYPE                  3	=
% #define MPI_ERR_TAG                   4	=
% #define MPI_ERR_COMM                  5	=
% #define MPI_ERR_RANK                  6	=
% #define MPI_ERR_REQUEST               7	=
% #define MPI_ERR_ROOT                  8	=
% #define MPI_ERR_GROUP                 9	=
% #define MPI_ERR_OP                    10	=
% #define MPI_ERR_TOPOLOGY              11	=
% #define MPI_ERR_DIMS                  12	=
% #define MPI_ERR_ARG                   13	=
% #define MPI_ERR_UNKNOWN               14	=
% #define MPI_ERR_TRUNCATE              15	=
% #define MPI_ERR_OTHER                 16	=
% #define MPI_ERR_INTERN                17	=
% #define MPI_ERR_IN_STATUS             18	= (..up to)
% #define MPI_ERR_PENDING               19	= (MPI_ERR_PENDING==19)
% #define MPI_ERR_ACCESS                20	? (missing in LAM)
% #define MPI_ERR_AMODE                 21	?
% #define MPI_ERR_ASSERT                22	?
% #define MPI_ERR_BAD_FILE              23	?
% #define MPI_ERR_BASE                  24	# (was 36 in LAM)
% #define MPI_ERR_CONVERSION            25	?
% #define MPI_ERR_DISP                  26	?
% #define MPI_ERR_DUP_DATAREP           27	?
% #define MPI_ERR_FILE_EXISTS           28	?
% #define MPI_ERR_FILE_IN_USE           29	?
% #define MPI_ERR_FILE                  30	?
% #define MPI_ERR_INFO_KEY              31	# (was 33 in LAM)
% #define MPI_ERR_INFO_NOKEY            32	# (was 29 in LAM)
% #define MPI_ERR_INFO_VALUE            33	# (was 34 in LAM)
% #define MPI_ERR_INFO                  34	?
% #define MPI_ERR_IO                    35	?
% #define MPI_ERR_KEYVAL                36	# (was 28 in LAM)
% #define MPI_ERR_LOCKTYPE              37	?
% #define MPI_ERR_NAME                  38	# (was 26 in LAM)
% #define MPI_ERR_NO_MEM                39	# (was 35 in LAM)
% #define MPI_ERR_NOT_SAME              40	?
% #define MPI_ERR_NO_SPACE              41	?
% #define MPI_ERR_NO_SUCH_FILE          42	?
% #define MPI_ERR_PORT                  43	?
% #define MPI_ERR_QUOTA                 44	?
% #define MPI_ERR_READ_ONLY             45	?
% #define MPI_ERR_RMA_CONFLICT          46	?
% #define MPI_ERR_RMA_SYNC              47	?
% #define MPI_ERR_SERVICE               48	# (was 25 in LAM)
% #define MPI_ERR_SIZE                  49	?
% #define MPI_ERR_SPAWN                 50	# (was 27 in LAM)
% #define MPI_ERR_UNSUPPORTED_DATAREP   51	?
% #define MPI_ERR_UNSUPPORTED_OPERATION 52	?
% #define MPI_ERR_WIN                   53	# (was 30 in LAM)
% #define MPI_ERR_LASTCODE              54	# (was 37 in LAM)
%
% #define MPI_ERR_SYSRESOURCE          -2	# (was 20 in LAM)
%
