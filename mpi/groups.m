% GROUPS                Group management
%
% MPI_Group_size         Returns the size of a group
% MPI_Group_rank         Returns the rank of this process in the given group
% MPI_Group_translate_ranks      Transl.procs' ranks in one grp to another grp
% MPI_Group_compare      Compares two groups
%
% MPI_Comm_group         Accesses the group associated with given communicator
% MPI_Comm_remote_group  Accesses the remote group assoc. with given intercomm
% MPI_Group_union        Produces a group by combining two groups
% MPI_Group_difference   Makes a group from the difference of two groups
% MPI_Group_intersection Produces a group intersection of two existing groups
% MPI_Group_incl         Reorder an existing group and take only listed members
% MPI_Group_excl         Reorder existing group and take only unlisted members
% MPI_Group_range_incl   Create new group from ranges of ranks in existing group
% MPI_Group_range_excl   Produce group by excluding ranges of ranks from group
% MPI_Group_free         Frees a group
%
% MPI_GROUP_EMPTY        MPITB constant for corresponding group object
% MPI_GROUP_NULL         MPITB constant for corresponding group object
%
% MPI_IDENT              MPITB constant for 'identical' comparison result
% MPI_CONGRUENT          MPITB constant for 'congruent' comparison result
% MPI_SIMILAR            MPITB constant for 'similar' comparison result
% MPI_UNEQUAL            MPITB constant for 'unequal' comparison result
%

