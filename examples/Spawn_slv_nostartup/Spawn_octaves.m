## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_octaves(nchld,ocmd)
% SPAWN_octaves:Spawns n octave chldrn, which should MPI_Init to unblock parent
%
%	Spawn_octaves [ ( nchld [,ocmd] ) ]
%
%  nchld (int) [1]	number of octave children to spawn
%  ocmd  (str) [...]	string to be --eval by children octaves
%			default value: 'MPI_Init, system("hostname")' approx.
%
%	Invoking interactively "Spawn_octaves(nchld)" in this Octave session
%	will launch nchld octaves round-robin in the hosts in hostlist
%	For instance, if nchld==length(list), the node of this Octave session
%	will be oversubscribed with one of the octave processes spawned
%	(in addition to the currently running Octave session)
%	no matter how you order hosts in hostlist
%
%  See code (M-file) for hints on howto customize the builtin host list
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

% --------------------------------------------------
%  Utility function LAM_Init is used. See its help page. Code is in utils sdir.
%  Usually, you will manually lamboot before running this demo
%  Alternatively, you can redefine the global HOSTS list here
%	or pass a local host-list and let LAM_Init make it global
%	(that way you won't need to lamboot before running the demo)
%  Alternatively (and that would fix the hostlist for all demos using utils)
%	LAM_Init.m could be edited to list your computers in the HOSTS{} cell
% --------------------------------------------------

% global HOSTS				% you can redefine global HOSTS
% HOSTS={'PC#N',...,'PC#1','PC#0'};	% or edit the LAM_Init builtin hostlist
% hosts={'PC#N',...,'PC#1','PC#0'};	% or pass local hostslist to LAM_Init
% NH=length(hosts);			% or have N+1 PCs already lambooted

	%% Our choice here: redefine global HOSTS
	%% modify at will, this time we don't care about oversubscribing

  global HOSTS				% this list will override the
  HOSTS={'h8','h5','h4','h2','h1'};	% default global defined at LAM_Init

  DISP=getenv('DISPLAY');		% fix this if you don't get xterms back

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=1; end
if nargin<2, ocmd=[		% design your own command list at will
	'mpi_init=MPI_Init,'			\
	'[sys_stat hostname]=system("hostname"),'\
	'[info rank]=MPI_Comm_rank(MPI_COMM_WORLD);'\
	'[info size]=MPI_Comm_size(MPI_COMM_WORLD);'\
 'printf("\nmy MPI_COMM_WORLD is size==%d, i`m rank==%d\n\n",size,rank);'\
	];
end

flag=0;
flag=flag ||     ~ischar(ocmd)
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || nchld<1;
if flag, usage('Spawn_octaves( <int> nchld , <str> ocmd ) , with nchld>0'); end


%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM %
%%%%%%%%%%%%%%%%%%%%%
  LAM_Init			% LAM_Init(#slaves, desired-rpi, host-list)
				% defaults to (length(HOSTS),'tcp',HOSTS)
   cmd='xterm';			% commandline to MPI_Comm_spawn

   args={};				% arguments to xterm children
   args={args{:},'-display',DISP};	% xterm -display to see xterm window
   args={args{:},'-e','octave'};	% xterm -e octave to run octave on it
   args={args{:},'-q'};			% octave -q f/ silent xterms
   args={args{:},'--persist'};		% octave -x f/interactive after --eval
%  args={args{:},'--echo-commands'};	% echo the --eval'ed commands
   args={args{:},'--eval', ocmd};	% and into --eval we put ocmd

   info=MPI_INFO_NULL;

 oldPSO=page_screen_output(0);
printf('About to get blocked on MPI_Comm_spawn(xterm -e octave...,%d)\n',nchld);
disp('MPI_Init should be somehow run on children octaves to unblock me :-)')
	page_screen_output(oldPSO);

 [info children errs]=MPI_Comm_spawn(cmd,args,nchld,    info,0,MPI_COMM_SELF);

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        % not blocked, unless you remove MPI_Init from --eval list %
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

				% comment out this line if you want to kill
  LAM_Clean;			% all xterms at once using lamclean from shell

