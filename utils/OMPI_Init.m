## Copyright (C) 2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function OMPI_Init(nslaves, hosts)
% OMPI_Init	create a global MPI_Info hint (SPAWN_INFO) to MPI_Comm_spawn
%		to specify a given no. slave hosts from a given list of hosts. 
%		Oops! and MPI_Init (if not already done)!
%
%	OMPI_Init [ ( nslaves [, hosts] ) ]
%
%  Both args are optional, and override defaults if provided. Default
%  global vars NSLAVES(int) and HOSTS(cell{NSLAVES+1}-of-strings)
%  can be defined prior to invocation, and they will be used by OMPI_Init.
%  If they are not defined, hosts are taken from a builtin HOSTS list.
%  After OMPI_Init invocation, those 2 global vars will be defined
%  and SPAWN_INFO will specify the first NSLAVES hosts from HOSTS.
%
%  If you plan to use this script regularly, you'll save a lot of typing
%  if you edit HOSTS at the beginning of OMPI_Init.m to match your cluster.
%  Replace the builtin HOSTS variable by your cluster node names.
%
%  This utility should not be called directly but rather as MPITB_Init
%  which detects whether this is OMPI or other implementation (like LAM/MPI)
%  Generic names such as MPITB_Init/MPITB_Clean will (hopefully) be more
%  readable to both LAM & OMPI users.
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

oldPSO=page_screen_output(0);

%%%%%%%%%%%%%%%%%%
% DEFAULT VALUES %
%%%%%%%%%%%%%%%%%%
global NSLAVES HOSTS SPAWN_INFO		% make sure they're sensible

if isempty(HOSTS) || ~iscell(HOSTS)	% master host should be last in list
	HOSTS={	'h8','h7','h6','h5','h4','h3','h2','h1'};
end
	%---------------------------------------
	% see man page for OMPI's MPI_Comm_spawn -> MPI_Info hint
	% we can only define "host" (char*) and "wdir" (char*) keys
	% there is no other way to make MPI_Comm_spawn learn anything about
	% our cluster environment
	% this must be complemented with -host/-H/-hostfile information
	% to mpirun, so this is the whole procedure for Open-MPI:
	%---------------------------------------
	% mpirun -c 1 -H h1,<remaining cluster nodes> octave --interactive
	%	makes OMPI master_node_list to know about those other hosts
	%	also, interactive octave is launched on 1st node
	%	hopefully, you'll have no problems with $DISPLAY there
	% MPITB_Init(nslaves)
	%	will define SPAWN_INFO including first nslaves from list
	% Octave_spawn
	%	will default to those parameters (nslaves, hosts, spawn_info...)
	%---------------------------------------

if isempty(NSLAVES) ||  ~ is_scalar (NSLAVES) || ~isreal(NSLAVES) ||...
	   NSLAVES<0|| fix(NSLAVES)~=NSLAVES  ||...
	   NSLAVES>length(HOSTS)-1
	   NSLAVES=length(HOSTS)-1;
end

if isempty(SPAWN_INFO) ||~ is_scalar(SPAWN_INFO)||~isreal(SPAWN_INFO)||...
	   SPAWN_INFO<0|| fix(SPAWN_INFO)~=SPAWN_INFO
	   SPAWN_INFO = MPI_INFO_NULL;
end

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
if nargin>1
	if ~iscell(hosts)		% hostlist cell
	    error([mfilename ': 2nd arg is not a cell'])
   else	for i=1:length(hosts)		% yet more,cell-of-strings
	  if ~ischar(hosts{i})
	    error([mfilename ': 2nd arg is not cell-of-strings'])
	  end
	end
	HOSTS=hosts;			% OK, pass hosts to global
	NSLAVES=length(HOSTS)-1;	% side-effect: default nslaves changed
   end
end
if nargin>0
	if ~is_scalar(nslaves) || ~isreal(nslaves) ||...
	fix(nslaves)~=nslaves  || nslaves>=length(HOSTS)
	    error([mfilename ': 1st arg is not a valid #slaves'])
   else	NSLAVES = nslaves;		% OK, nslaves is global new default
   end
end
					% from now on use just NSLAVES & HOSTS
					%	  forget about nslaves / hosts
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% MPI_Init if not already done %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  [infI flgI] = MPI_Initialized;		% Init?
  [infF flgF] = MPI_Finalized;			% Finalize?
  if infI || infF
    page_screen_output(oldPSO);
    error([mfilename ': error calling _Initialized/_Finalized?'])
  end

  if    flgI			% already _Initialized
     if flgF			% already _Finalized!!! cycle over
	disp([mfilename ': clearing MPITB & starting over'])
	clear MPI_* MPITB_* OMPI_*
	MPI_Init;		% done
     else % flgF %
				% already _Init, nothing to do
     end  % flgF %

  else % flgI %			% not even _Init

  %	putenv('LAM_MPI_SSI_rpi',     'lamd')
  %	putenv('OMPI_MCA_btl_base_debug','2')
	MPI_Init;		% done

  end  % flgI %
 
%%%%%%%%%%%%%%%
% Create hint %
%%%%%%%%%%%%%%%
    if SPAWN_INFO == MPI_INFO_NULL	% Hmf, this is not explained in help
 [info SPAWN_INFO] = MPI_Info_create;	% user could define SPAWN_INFO prior
    end					% to OMPI_Init, and it would be reused
	HL = '';
    for hn=HOSTS(1:NSLAVES)		% for all first NSLAVES
	HL=[HL ',' hn{:}];		% add next hostname in list
    end
	HL(1)=[]			% remove leading comma

    MPI_Info_set(SPAWN_INFO, 'host', HL);	% that's the hint
%   MPI_Info_set(SPAWN_INFO, 'wdir','.');

    page_screen_output(oldPSO);

endfunction
