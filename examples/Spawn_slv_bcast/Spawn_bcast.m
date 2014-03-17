## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_bcast(nchld)
% SPAWN_BCAST:	Spawns octave children, decide at runtime command to run,
%		and send command for remote evaluation to children.
%
%	Spawn_bcast [ ( nchld ) ]
%
%  nchld (int) [1]	number of children to spawn
%
%  See code (M-file) for hints on howto customize the builtin host list
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

% global HOSTS				% you can redefine global HOSTS
% HOSTS={'PC#N',...,'PC#1','PC#0'};	% or edit the LAM_Init builtin hostlist
% hosts={'PC#N',...,'PC#1','PC#0'};	% or pass local hostslist to LAM_Init
% NH=length(hosts);			% or have N+1 PCs already lambooted

	%% Our choice here: local list directly passed to LAM_Init
	%% modify at will, but make sure your master node is last in list

  hosts={'h8','h2','h4','h5','h1'};	% this list will override the
  NH=length(hosts);			% default global defined at LAM_Init

  global DEBUG; DEBUG=true;		% let xterms show for this application
  global DISPLAY_REQUIRED;		% set to true if your setup requires it
	 DISPLAY_REQUIRED=true;		% see Octave_Spawn.m code

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=1; end
flag=0;
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || 0>=nchld || nchld>=NH;
if flag, usage('Spawn( <int> nchld ) , with 0< nchld <list_length'); end


%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM % and build startup hint info
%%%%%%%%%%%%%%%%%%%%%
  LAM_Init(nchld,'t',hosts)	% LAM_Init(#slaves, desired-rpi, host-list)
  Octave_Spawn			% Octave_Spawn(NSLAVES,args={},infokey=NULL)
  global NEWORLD		% NEWORLD defined by Octave_Spawn

%%%%%%%%%%%%%%%%%%%%%%%%%%
% startup_slave protocol % Bcast protocol, see startup_bcastSlv.m
%%%%%%%%%%%%%%%%%%%%%%%%%%

  printf('deciding at runtime a command to run on slaves\n');
  printf('copy-paste [stat hnam]=system("hostname") if you are not inspired\n');
  fflush(stdout);

  cmd= input ('command? : ', "s");
  MPI_Bcast (cmd,0,NEWORLD);	% Pass runtime-decided command

				% comment out this line if you want to kill
  LAM_Clean;			% all xterms at once using lamclean from shell

