% TOPO                  Process topologies
%
% MPI_Dims_create        Creates a division of processors in a cartesian grid
% MPI_Cart_create        Makes new comm. with cartesian topology info attached
% MPI_Graph_create       Makes new comm. with grid topology info. attached
% MPI_Topo_test          Determines topology (if any) associated with a comm.
%
% MPI_Graphdims_get      Retrieves graph topology info. associated with a comm.
% MPI_Graph_get          Retrieves graph topology info. associated with a comm.
% MPI_Graph_neighbors_count      Returns #neighbors of node in graph topo.comm.
% MPI_Graph_neighbors    Returns neighbors of node in comm. with graph topology
%
% MPI_Cartdim_get        Retrieves cartesian topo. info. associated with comm.
% MPI_Cart_get           Retrieves cartesian topo. info. associated with comm.
% MPI_Cart_rank          Determines proc.rank in comm. given cartesian location
% MPI_Cart_coords        Determines proc. coords in cartesian topo. given rank
% MPI_Cart_shift         Returns src/dst ranks, given shift dir. and amount
% MPI_Cart_sub           Partitions cartesian comm. into lower-dim. subgroups
%
% MPI_Cart_map           Maps process to cartesian topology information
% MPI_Graph_map          Maps process to graph topology information
%
% MPI_CART               MPITB code for 'cartesian topology'
% MPI_GRAPH              MPITB code for 'graph topology'
%

