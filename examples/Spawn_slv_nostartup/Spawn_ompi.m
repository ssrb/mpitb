## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_ompi(nchld)
% SPAWN_xterms:	Spawns n xterm children, which should MPI_Init to unblock parent
%
%	Spawn_xterms [ ( nchld ) ]
%
%  nchld (int) [1]	number of xterm children to spawn
%
%	Invoking interactively "Spawn_xterms(nchld)" in this Octave session
%	will launch nchld xterms round-robin in the hosts in hostlist
%	For instance, if nchld==length(list), the node of this Octave session
%	will be oversubscribed with one of the xterm processes spawned
%	(in addition to the currently running Octave session)
%	no matter how you order hosts in hostlist
%
%  See code (M-file) for hints on howto customize the builtin host list
%  OpenMPI: start octave using mpirun -c 1 -H h1,h8,<...> octave --interactive
%
%  Demos here (increasing difficulty order):	Spawn_xterms Spawn_octaves
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

% --------------------------------------------------
%  Utility function MPITB_Init is used. See its help page & Code in utils sdir.
%  You can:
%  LAM		: lamboot before running this demo
%  LAM & OMPI	: redefine the global HOSTS list here (right below)
%		: pass a local host-list as MPITB_Init(arg) to make it global
%		: edit MPITB_Init.m to list your computers in the HOSTS{} cell
%        OMPI	: you _must_ pass a master host list to the interactive session
%			mpirun -c 1 -H h1,h8,<...> octave --interactive
% --------------------------------------------------

% global HOSTS				% you can redefine global HOSTS
% HOSTS={'PC#N',...,'PC#1','PC#0'};	% or edit MPITB_Init builtin HOSTS
% hosts={'PC#N',...,'PC#1','PC#0'};	% or pass hostlst as 2nd MPITB_Init(arg)
% NH=length(hosts);			% or have N+1 PCs already lambooted

	%% Our choice here: redefine global HOSTS
	%% modify at will, this time we don't care about oversubscribing

  global HOSTS				% this list will override the
  HOSTS={'h8','h5','h4','h2','h1'};	% default global defined at MPITB_Init

  DISP=getenv('DISPLAY');		% fix this if you don't get xterms back

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=1; end
flag=0;
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || nchld<1;
if flag, usage('Spawn_xterms( <int> nchld ) , with 0< nchld'); end


%%%%%%%%%%%%%%%%%%%%%		% OMPI_Init(#slaves, host-list)
%  MPITB: Start MPI %		%  LAM_Init(#slaves, host-list)
%%%%%%%%%%%%%%%%%%%%%		% defaults to (length(HOSTS)-1,HOSTS)
   MPITB_Init			% defines NSLAVES, HOSTS, SPAWN_INFO

   cmd='xterm';			% commandline to MPI_Comm_spawn

   args={};				% arguments to xterm children
   args={args{:},'-display',DISP};	% xterm -display to see xterm window

   global     SPAWN_INFO		% defined at MPITB_Init

 oldPSO=page_screen_output(0);
printf('About to get blocked on MPI_Comm_spawn(xterm,-display...,%d)\n',nchld);
disp('You will have to ./MPI_Init on children xterms to unblock me!!!')
disp('(if MPI_Init fails recompile it with mpicc -o MPI_Init MPI_Init.c)')
disp('Also, do notice hostname in your slave prompts!!! (or type hostname)')
	page_screen_output(oldPSO);

 [info children errs]=MPI_Comm_spawn(cmd,args,nchld,SPAWN_INFO,0,MPI_COMM_SELF);

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% blocked until slaves do ./MPI_Init %
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% MPITB_Clean;			% Clean MPITB from Octave so we can start over
  if strcmp(MPITB_Detect_mpi,'LAM/MPI')
	LAM_Clean		% Ouch! Open-MPI's mpirun doesn't like clear
  end

