## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function NumCmds_Init (ncmd, qflg, tagn, dbug)
%NUMCMDS_INIT		NumCmds protocol initialization
%
%	NumCmds_Init [ ( ncmd [, qflg [, tagn [, dbug] ] ] ) ]
%
%  ncmd (int)	number of commands that will be sent to each slave, or
%	[0]	0 for command-controlled loop
%  qflg (bool)	quit flag, instructing the slave octave to quit after finishing
%       [false]
%  tagn (int)	desired tag number for protocol send/recvs
%	[TAG]	if TAG is globally available, it is used as default value
%	[777]	default value if no global TAG available
%		in either case, final tagn value is exported as TAG
%  dbug (bool)	debug flag, will make slaves to display MPILoopIdx & cmd
%	[DEBUG]	if DEBUG is globally available, it is used as default value
%	[true]	default value if no global TAG available
%		in either case, final dbug value is exported as DEBUG
%
%  SEE ALSO:	startup_MPITB, startup_ncmdsSlv, NumCmds_Send
%
%  After spawning children processes that will run the startup_ncmdsSlv script,
%  this routine could be used to satisfy the protocol enforced by that script,
%  broadcasting NQTD=[NumCmds,Quit,Tag,Debug] array
%
%  Slave processes enter then either a NumCmds-controlled or a Cmd-controlled
%  loop, in which they repeatedly receive a packed buffer, possibly containing
%  - cmd (str)	the desired command to be evaluated in this iteration
%  - vars	any argument useful for eval(cmd) (w/same name used in cmd)
%  and eval the specified cmd. If no cmd is sent, the previous one is used
%  (initially cmd=''). Send cmd='' to force a data-only slave iteration.
%
%  A NumCmds-controlled loop exits after NumCmds iterations.
%  A Cmd-controlled loop exits when recvd cmdstr is either "break/exit/quit"
%  See startup_ncmdsSlv for more details
%

%%%%%%%%%%%%%%%%%
% GLOBAL EXPORT %
%%%%%%%%%%%%%%%%%
global TAG DEBUG
if isempty(TAG),	TAG  = 777; end
if isempty(DEBUG),	DEBUG=true; end

%%%%%%%%%%%%%%%%%%
% DEFAULT VALUES %
%%%%%%%%%%%%%%%%%%
if nargin<4, dbug=DEBUG; end		% if no args,
if nargin<3, tagn=  TAG; end		% assume the most-visible options:
if nargin<2, qflg=false; end		% debug behaviour (print MPILoopIdx)
if nargin<1, ncmd=    0; end		% don't quit, indeterminate ncmds

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
if nargin>3
	if ~islogical(dbug)		% dbug is a bool
	    error('NumCmds_Init: 4th arg is not a bool')
	else   DEBUG=dbug;		% export as global if valid
	end
end
if nargin>2
	if ~isscalar(tagn) || ~isreal(tagn) ||...
	   fix(tagn)~=tagn  || tagn<0
	    error('NumCmds_Init: 3rd arg is not a valid tag')
	else   TAG=tagn;		% export as global if valid
	end
end
if nargin>1
	if ~isscalar(qflg)
	    error('NumCmds_Init: 2nd arg is not a valid flag')
	end				% qflg should even be bool
	qflg=logical(qflg);
end
if nargin>0
	if ~isscalar(ncmd) || ~isreal(ncmd) ||...
	  fix(ncmd)~=ncmd  || ncmd<0
	    error('NumCmds_Init: 1st arg is not a valid #cmds')
	end
end

%%%%%%%%%%%%%%%%%
% BROADCAST NQT %
%%%%%%%%%%%%%%%%%
global NEWORLD
if isempty(NEWORLD)
  error('NumCmds_Init: global NEWORLD required (call Octave_Spawn)');
end

NQTD=[ncmd,qflg,tagn,dbug];		% promote qflg/dbug bool -> realscalar
MPI_Bcast (NQTD,          0,NEWORLD);	% expected: NUMCMDS/QUIT/TAG/DEBUG

endfunction
