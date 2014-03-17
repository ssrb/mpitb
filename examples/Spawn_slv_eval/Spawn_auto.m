## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

function Spawn_auto(nchld)
% SPAWN_AUTO:	Spawns octave children, illustrating the ability of the
%		NumCmds protocol to send packed vars & commands to children.
%
%	Spawn_auto [ ( nchld ) ]
%
%  nchld (int) [2]	number of children to spawn
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

  global TAG  ; TAG  = 777;		% default tag used by NumCmds protocol
  global DEBUG; DEBUG=true;		% let xterms show for this application
  global DISPLAY_REQUIRED;		% set to true if your setup requires it
	 DISPLAY_REQUIRED=true;		% see Octave_Spawn.m code

%%%%%%%%%%
% ArgChk %
%%%%%%%%%%
if nargin<1,  nchld=2; end
flag=0;
flag=flag ||   ~isscalar(nchld) || ~isnumeric(nchld);
flag=flag || fix(nchld)~=nchld  || 0>=nchld || nchld>=NH;
if flag, usage('Spawn( <int> nchld ) , with 0< nchld <list_length'); end


%%%%%%%%%%%%%%%%%%%%%
%  MPITB: Start LAM % and build startup hint info
%%%%%%%%%%%%%%%%%%%%%
  LAM_Init(nchld,'t',hosts)	% LAM_Init(#slaves, desired-rpi, host-list)

   args={};			% command-line arguments to the Octave slave
   args={args{:},'--persist'};	% let it enter into interactive after --eval
   args={args{:},'--eval','startup_ncmdsSlv'};	% here: startup on --eval

  Octave_Spawn(nchld,args)	% This is the only difference,startup on --eval
% Octave_Spawn			% Octave_Spawn(NSLAVES,args={},infokey=NULL)
  global NEWORLD		% NEWORLD defined by Octave_Spawn

%%%%%%%%%%%%%%%%%%%%%%%%%%
% startup_slave protocol % NumCmds protocol, see startup_ncmdsSlv.m
%%%%%%%%%%%%%%%%%%%%%%%%%%
  NumCmds_Init(2,false)		% 2 commands, don't quit after

%%%%%%%%%%%%%%%%%%%%%%%%%% 2 cmds: 
% Num-Commands  protocol % first convolve with a diagonal stencil \ or /
%%%%%%%%%%%%%%%%%%%%%%%%%%  then convolve with a vertical stencil |

  cmd=[					% "whos" just to see local variables
	'whos cmd img stencil, stencil,'\
	'img=conv2(img,stencil,"same");'\
	'MPI_Gatherv(img, [] , [] , [] , 0,NEWORLD);'\
	];				% same command both times

  GRN=15;				% granularity / stencil size
  st1=eye(GRN);  st2=rot90(st1);	% stencils
  st3=zeros(GRN);st3(:,ceil(GRN/2))=1;
  sts=cat(3,st1,st2,st3);		% for easy indexing

  lena=loadimage('lena.mat');
  imshow(lena)				% show original image

  ncols=columns(lena);			% number of cols, whole domain
  lims=round(ncols*[1:nchld]/nchld);	% col limit for each child's domain
  lims=[0 lims(:)'];			% from lim(i)+1 to lim(i+1)

  cnts=diff(lims);			% #cols for each child (counts)
  dsps=lims(1:end-1);			% starting col C lang (displacements)
  cnts=[0 cnts(:)']; cnts*=rows(lena);	% root rank won't contribute
  dsps=[0 dsps(:)']; dsps*=rows(lena);	% => count 0 @ disp 0

%%%%%%%%%%%%%%%
% 1st command %
%%%%%%%%%%%%%%%
  data=cell(nchld,3);			% send 3 data items to each child
for i=1:nchld
  data{i,1}=cmd;			% "cmd" variable to be evaluated
  data{i,2}=lena(:,lims(i)+1:lims(i+1));% "img" to be convolved
  data{i,3}=sts(:,:,2-mod(i,2));	% zig-zag (1st \) , (2nd /), and so on
end

  NumCmds_Send({"cmd","img","stencil"},	% 1st command
		data)
	clear   data			% copies no longer required

%%%%%%%%%%%%%%%
% 1st command % visualization at master
%%%%%%%%%%%%%%%
input('press enter... ');
		     len2=zeros(size(lena));
info=MPI_Gatherv([ ],len2,cnts,dsps, 0,NEWORLD);
	      imshow(len2,[])

%%%%%%%%%%%%%%%
% 2nd command %
%%%%%%%%%%%%%%%
  NumCmds_Send({"stencil"},	{st3})	% 2nd command

  % notice we repeat cmd over already convolved img
  % because startup_numcmdsSlv checks (if ~exist('cmd','var'), cmd=oldcmd; end)
  % 				      (hence cmd is already there)
  % because cmd was img=conv2(img...) (hence img is already on child workspace)
  % because we overwrite stencil on children with new one (all the same st3)

%%%%%%%%%%%%%%%
% 2nd command % visualization at master
%%%%%%%%%%%%%%%
input('press enter... ');
info=MPI_Gatherv([ ],len2,cnts,dsps, 0,NEWORLD);
	      imshow(len2,[])

%%%%%%%%
% Done %
%%%%%%%%
				% comment out this line if you want to kill
  LAM_Clean;			% all xterms at once using lamclean from shell

