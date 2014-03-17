## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_staggrd(nchld)
% SPAWN_STAGGRD:Spawns a child, which in turns spawns a child, and on and...
%		to make it difficult, we require each child on a different node
%
%	Spawn_staggrd [ ( nchld ) ]
%
%  nchld (int) [2]	number of children to spawn, staggered
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
if nargin<1,  nchld=2; end
flag=0;
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || 0>nchld || nchld>=NH;
if flag, usage('Spawn( <int> nchld ) , with 0<= nchld <list_length'); end


%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM % and build startup hint info
%%%%%%%%%%%%%%%%%%%%%
  LAM_Init(nchld,'t',hosts)	% LAM_Init(#slaves, desired-rpi, host-list)

	%% this is tricky: slaves will also run this section, but
	%% since there are N nodes already lambooted and
	%% this slave process has already MPI_Init'ed,
	%% LAM_Init is satisfied and does nothing :-)

  %% the node where to spawn can be controlled using the
  %% lam_spawn_sched_round_robin info key to the MPI_Comm_spawn call
  %% see "man MPI_Comm_spawn" for more info on the subject

      global NSLAVES		% NSLAVES updated on LAM_Init (fix that?)
disp([nchld  NSLAVES])		% to check while stepping thru

if nchld==0			% We got it!
  disp('I am the last requested Octave child!!! Woo-hoo!!!')
else				% for nchld==1 we have 1 to Spawn
  fprintf('%d more Octaves to go\n', nchld);
				% This is the only difference between
   ocmd= 'startup_bcastSlv' ;	% the _slv_merge and _eval versions:
				% startup is included in --eval switch
   args={};			% command-line arguments to the Octave slave
   args={args{:},'--persist'};	% let it enter into interactive after --eval
%  args={args{:},'--echo-commands'};	% echo the --eval'ed commands
   args={args{:},'--eval',ocmd};% and into --eval we put ocmd

  [info inf]=MPI_Info_create;	% create an info object
   info     =MPI_Info_set (inf,  'lam_spawn_sched_round_robin',...
				['n' int2str(nchld-1)]     );

  disp('About to spawn a slave Octave on its own xterm.')
  disp('Type ''quit''/''exit''/''dbcont'' to go on.')
  disp('You will also have to type it at the child Octave!!!')
  keyboard

  Octave_Spawn(1,args,inf);	% spawn just 1 Octave on previous slave node
  global NEWORLD		% NEWORLD defined by Octave_Spawn

%%%%%%%%%%%%%%%%%%%%%%%%%%
% startup_slave protocol % Bcast protocol, see startup_bcastSlv.m
%%%%%%%%%%%%%%%%%%%%%%%%%%

  cmd =['Spawn_staggrd(' int2str(nchld-1) ')'];	% advance to previous node
  MPI_Bcast (cmd,0,NEWORLD);			% Pass command

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% you could make octave#i to recv from #i+1
% HOMEWORK: Wait for children :-) % and octave#N-1 doesn't recv, sends to #N-2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% and #N-2 to #N-3 and so on... and then quit

end	% nchld==0

				% comment out this line if you want to kill
  LAM_Clean;			% all xterms at once using lamclean from shell

