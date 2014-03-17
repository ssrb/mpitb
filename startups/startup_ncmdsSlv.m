## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function startup_ncmdsSlv
% STARTUP_ncmdsSLV	Script for child MPITB tasks under the NumCmds protocol
%
%	It will be called from the last else branch in startup_MPITB
%		if this is a spawned task (detected by LAM/OMPI envvar)
%	Programmed as function to avoid the need to clear vars (own workspace)
%		some variables globally exported: WORLD, NEWORLD, RANK, HOSTNAME
%					NUMCMDS, QUIT, TAG, DEBUG, MPILoopIdx
%	It will reply to the NUMCMDS/QUIT protocol enforced by parent Octave
%		probably by using the NumCmds_Init and NumCmds_Send utilities
%	Take notice that MPI_Init is the first MPI call
%		that will un-block the MPI_Spawn call in the parent Octave
%	Assumes that the parent, after Spawn, will Merge (or it'll block)
%	Assumes that the parent, after Merge, will BCast NQTD, etc, etc

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------


if isempty(getenv('LAMPARENT')) &&...
   isempty(getenv('OMPI_MCA_universe'))
	error('startup_ncmdsSlv: only for spawned tasks')
end

disp('Starting child Octave process ...')

info=MPI_Init;
if info~=MPI_SUCCESS
	error('startup_ncmdsSlv: must lamboot first')
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

global NUMCMDS QUIT TAG DEBUG MPILoopIdx	% protocol variables
disp('NUMCMDS protocol...')			% MPILoopIdx is loop ctrl var
	  NQTD=[0 0 0 0];
MPI_Bcast(NQTD,0,NEWORLD);			% expected: NUMCMDS and QUIT
NUMCMDS	= NQTD(1);
QUIT	= NQTD(2); QUIT = logical(QUIT);
TAG	= NQTD(3);
DEBUG	= NQTD(4); DEBUG= logical(DEBUG);

   oldcmd=' ';					% dummy command

if NUMCMDS

      disp('NUMCMDS-controlled loop...')
  for MPILoopIdx=1:NUMCMDS
      [info stat]=MPI_Probe(0,TAG,NEWORLD);	% must ask for room first
      [info elem]=MPI_Get_elements(stat,'');
      buffer=blanks(elem);
      MPI_Recv  (buffer,0,TAG,NEWORLD);		% expected: cmd; maybe: args
      MPI_Unpack(buffer,0,    NEWORLD);		% unpack them all (cmd/args)

      if ~exist('cmd','var'), cmd=oldcmd; end	% repeat previous command if
      if DEBUG					% handy for DEBUGging
	fprintf('MPILoopIdx=%2d, cmd=''%s''\n',...
		 MPILoopIdx,	 cmd(1:min(50,length(cmd))));
      end
      eval(cmd); oldcmd=cmd; clear cmd		% ...no new command this turn
  end

else
      disp('cmd-controlled loop (send break/exit/quit)...')
      MPILoopIdx=0;
  while ~strcmp(oldcmd,'break')			% cmd-cntrlled
      MPILoopIdx++;
      [info stat]=MPI_Probe(0,TAG,NEWORLD);	% must ask for room first
      [info elem]=MPI_Get_elements(stat,'');
      buffer=blanks(elem);
      MPI_Recv(buffer,0,TAG,NEWORLD);		% expected: cmd; maybe: args
      MPI_Unpack(buffer,0,NEWORLD);		% unpack them all

      if ~exist('cmd','var'), cmd=oldcmd; end	% repeat previous command if
      if DEBUG					% handy for DEBUGging
	fprintf('MPILoopIdx=%2d, cmd=''%s''\n',...
		 MPILoopIdx,	 cmd(1:min(50,length(cmd))));
      end
      eval(cmd); oldcmd=cmd; clear cmd		% ...no new command this turn
  end
	% --------------------------------------------- %
	% cmd may be break/exit/quit			%
	% break: obey QUIT flag from NUMCMDS protocol	%
	% exit: keep slave Octave window (DEBUGging)	% -> sure ?!? we'll die
	% quit: quit slave Octave irrespective of QUIT	%
	% --------------------------------------------- %
end

MPI_Finalize;			% We Init, we Finalize
MPITB_Clean			% really required? we'll die anyways... ?!?

if QUIT, quit, end		% this line-of-code not-reached unless break

