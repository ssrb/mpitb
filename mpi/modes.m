% MODES                 Point-to-point communication
%
% MPI_Send               Performs a basic send
% MPI_Bsend              Basic send with user-specified buffering
% MPI_Ssend              Basic synchronous send
% MPI_Rsend              Basic ready send
% MPI_Recv               Performs a basic receive
%
% MPI_Buffer_attach      Attaches a user-defined buffer for sending
% MPI_Buffer_detach      Removes an existing buffer (for use in MPI_Bsend)
%
% MPI_Sendrecv           Sends and receives a message
% MPI_Sendrecv_replace   Sends and receives using a single buffer
%
% MPI_Pack               Packs a MATLAB variable into contiguous memory
% MPI_Unpack             Unpack a MATLAB variable from contiguous memory
% MPI_Pack_size  Returns upper bound on amount of space needed to pack a msg
% MPI_Get_count          Returns the number of bytes in a message
% MPI_Get_elements       Gets the number of "top level" elements
%
% MPI_BSEND_OVERHEAD     MPI constant for 'overhead for each bsend packet'
% MPI_UNDEFINED          MPITB constant for 'undefined stuff'
% MPI_CANCEL_SOURCE      MPITB constant for 'successful cancel'
% MPI_ANY_SOURCE         MPI symbol for 'match any source rank'
% MPI_ANY_TAG            MPI symbol for 'match any message tag'
% MPI_PROC_NULL          MPITB constant for 'rank of null process'
%

