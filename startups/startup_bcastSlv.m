## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function startup_bcastSlv
% STARTUP_bcastSLV	Script for child MPITB tasks under the BCast protocol
%
%	It will be called from the last else branch in startup_MPITB
%		if this is a spawned task (detected by LAM/OMPI envvar)
%	Programmed as function to avoid the need to clear vars (own workspace)
%		some variables globally exported: WORLD, NEWORLD, RANK, HOSTNAME
%	Take notice that MPI_Init is the first MPI call
%		that will un-block the MPI_Spawn call in the parent Octave
%	Assumes that the parent, after Spawn, will Merge (or it'll block)
%	Assumes that the parent, after Merge, will BCast (or it'll block)

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------


if isempty(getenv('LAMPARENT')) &&...
   isempty(getenv('OMPI_MCA_universe'))
	error('startup_bcastSlv: only for spawned tasks')
end

disp('Starting child Octave process ...')

info=MPI_Init;
if info~=MPI_SUCCESS
	error('startup_bcastSlv: must lamboot first')
end

disp('waiting for the parent to MPI_Intercomm_merge(children,0) ...')

global WORLD NEWORLD
WORLD=MPI_COMM_WORLD;				% tired of typing so much

MPI_Errhandler_set(  WORLD,MPI_ERRORS_RETURN);	% MPI default is aborting
[info parent ]=MPI_Comm_get_parent;
[info NEWORLD]=MPI_Intercomm_merge(parent,1);	% we are last in communicator
MPI_Errhandler_set(NEWORLD,MPI_ERRORS_RETURN);

global RANK HOSTNAME
[info RANK]   =MPI_Comm_rank(NEWORLD);		% rank in intracomm = 1..C
[sts HOSTNAME]=system('hostname');		% useful when many children
RANK,HOSTNAME

cmd=blanks(100);				% Room for cmd string
MPI_Bcast(cmd,0,NEWORLD);			% Recv command
eval(cmd);					% Evaluate command
MPI_Finalize;					% we Init -> we Finalize
quit						% done

