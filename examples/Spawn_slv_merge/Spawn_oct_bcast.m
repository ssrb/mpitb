## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_oct_bcast(nchld)
% SPAWN_oct_bcast: Spawns n octave chldrn, using tool Octave_Spawn
%		and issues a MPI_Bcast from master, to force user to...
%		... issue another _Bcast at slaves to unblock master
%
%	Spawn_oct_auto [ ( nchld ) ]
%
%  nchld (int) [1]	number of octave children to spawn
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

	%% Our choice here: redefine global HOSTS
	%% modify at will, this time we don't care about oversubscribing

  global HOSTS				% this list will override the
  HOSTS={'h8','h5','h4','h2','h1'};	% default global defined at LAM_Init

  % DEBUG means: I want to see slave octaves, just in case
  %	there are error messages, or just to see assignment output or printf...
  % DISPLAY_REQUIRED: useful if running "xterm" won't reach your X-server
  %	but you can work around that with "xterm --display $DISPLAY"
  %	set global DISPLAY to an appropriate value before using Octave_Spawn

  global DEBUG; DEBUG=true;		% let xterms show for this application
  global DISPLAY_REQUIRED;		% see Octave_Spawn.m code
	 DISPLAY_REQUIRED=true;		% compare to Spawn_oct_man.m

% global DISPLAY			% if commented out, $DISPLAY used
% DISP=getenv('DISPLAY');               % fix this if you don't get xterms back


%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=1; end
	      ocmd=[		% This demo has a fixed command list
	'[info rank]=MPI_Comm_rank(  WORLD);'\
	'[info size]=MPI_Comm_size(  WORLD);'\
 'printf("\nmy          WORLD is size==%d, i`m rank==%d\n\n",size,rank);'\
	'[info rank]=MPI_Comm_rank(NEWORLD);'\
	'[info size]=MPI_Comm_size(NEWORLD);'\
 'printf("\nmy        NEWORLD is size==%d, i`m rank==%d\n\n",size,rank);'\
 'printf("\nNow you are expected to type (copy-paste if you want)\n");'\
 'printf("the following command in these slave octaves\n");'\
 'printf("in order to pair(unblock?) root (rank 0) who serves the _Bcast:\n");'\
 'disp(""),'\
 'printf("\t\tcmd=blanks(100), MPI_Bcast(cmd,0,NEWORLD), cmd, eval(cmd)\n");'\
	];			% (NE)WORLD both defined in startup_mergeParent
end

flag=0;
flag=flag ||     ~ischar(ocmd);
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || nchld<1;
if flag, usage('Spawn_oct_auto( <int> nchld , <str> ocmd ), with nchld>0'); end


%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM %
%%%%%%%%%%%%%%%%%%%%%
  LAM_Init			% LAM_Init(#slaves, desired-rpi, host-list)
				% defaults to (length(HOSTS),'tcp',HOSTS)
   args={};			% command-line arguments to the Octave slave
   args={args{:},'--persist'};	% let it enter into interactive after --eval
%  args={args{:},'--echo-commands'};	% echo the --eval'ed commands
   args={args{:},'--eval',ocmd};% and into --eval we put ocmd

   info=MPI_INFO_NULL;

 oldPSO=page_screen_output(0);
printf('About to Octave_spawn(%d,ocmd...)\n',nchld);
disp('Slave Octaves will run the prescribed commands and persist')

  Octave_Spawn(nchld,args,info);
  global NEWORLD		% NEWORLD defined by Octave_Spawn

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% LAM_Clean below disconnects this master session		%
	% from MPI altogether (slaves, daemon, from everything)		%
	% so you could Spawn_oct_auto again, ...			%
	% --------------------------------------			%
	% or you could also comment it and continue playing with	%
	% the interactive Octave windows, now they're open for you	%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	[info rank]=MPI_Comm_rank(MPI_COMM_WORLD);
	[info size]=MPI_Comm_size(MPI_COMM_WORLD);
  printf("\nmy MPI_COMM_WORLD is size==%d, i'm rank==%d\n\n",size,rank);
	[info rank]=MPI_Comm_rank(       NEWORLD);
	[info size]=MPI_Comm_size(       NEWORLD);
  printf("\nmy        NEWORLD is size==%d, i'm rank==%d\n\n",size,rank);

	cmd='[stat outp]=system("hostname")';
printf('About to engage as source in collective command (might block):\n');
printf('info=MPI_Bcast(cmd,0,NEWORLD)\t(where cmd is %s)\n',cmd);
	info=MPI_Bcast(cmd,0,NEWORLD)	% blocked until slaves join the coll

	page_screen_output(oldPSO);

disp('')
disp('--> *** finish this issue first / read instructions in slaves ***')
disp('')
disp('-----------------------------------------')
disp('-- After finishing MPI_Bcast in slaves --')
disp('-----------------------------------------')
disp('You may declare "global NEWORLD" and continue playing with slaves')
disp('for inst., pair: A="Hello World!", MPI_Send(A,1,7,NEWORLD) here with')
disp('this at slave 1: B=blanks(100), MPI_Recv(B,0,MPI_ANY_TAG,NEWORLD),B')
disp('-----------------------------------------')
disp('Type LAM_Clean to disconnect from LAM/MPI')
disp('-----------------------------------------')
disp('')

% LAM_Clean;			% yep, this time we have commented it out

