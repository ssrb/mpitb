## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_oct_man(nchld,ocmd)
% SPAWN_oct_man: Spawns n octave chldrn, manually (using MPI_Comm_spawn)
%	they expect to merge with this parent, due to startup_mergeParent
%
%	Spawn_oct_man [ ( nchld [,ocmd] ) ]
%
%  nchld (int) [1]	number of octave children to spawn
%  ocmd  (str) [...]	string to be --eval by children octaves
%			default value: 'printf("hello world...")' message.
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

  DISP=getenv('DISPLAY');		% fix this if you don't get xterms back

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=1; end
if nargin<2, ocmd=[             % design your own command list at will
        '[info rank]=MPI_Comm_rank(  WORLD);'\
        '[info size]=MPI_Comm_size(  WORLD);'\
 'printf("\nmy          WORLD is size==%d, i`m rank==%d\n\n",size,rank);'\
        '[info rank]=MPI_Comm_rank(NEWORLD);'\
        '[info size]=MPI_Comm_size(NEWORLD);'\
 'printf("\nmy        NEWORLD is size==%d, i`m rank==%d\n\n",size,rank);'\
        ];			% (NE)WORLD both defined in startup_mergeParent
end
                                                                                
flag=0;
flag=flag ||     ~ischar(ocmd);
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || nchld<1;
if flag, usage('Spawn_oct_man( <int> nchld , <str> ocmd ) , with nchld>0'); end


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
printf('About to MPI_Comm_spawn(xterm -e octave...,%d)\n',nchld);
disp('script startup_mergeParent will MPI_Init children to unblock me :-)')

 [info children errs]=MPI_Comm_spawn(cmd,args,nchld,    info,0,MPI_COMM_SELF);
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% not blocked, unless you manage to avoid startup_mergeParent %
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printf('\nAbout to MPI_Intercomm_merge(children,0), i`ll be first (rank 0)\n');
disp('script startup_mergeParent will also merge children to unblock me :-)')

 [info NEWORLD]=MPI_Intercomm_merge(children,0);
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	% not blocked, unless you manage to avoid startup_mergeParent %
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	[info rank]=MPI_Comm_rank(MPI_COMM_WORLD);
	[info size]=MPI_Comm_size(MPI_COMM_WORLD);
  printf("\nmy MPI_COMM_WORLD is size==%d, i'm rank==%d\n\n",size,rank);
	[info rank]=MPI_Comm_rank(       NEWORLD);
	[info size]=MPI_Comm_size(       NEWORLD);
  printf("\nmy        NEWORLD is size==%d, i'm rank==%d\n\n",size,rank);
	page_screen_output(oldPSO);

				% comment out this line if you want to kill
  LAM_Clean;			% all xterms at once using lamclean from shell

