## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------
function chldren=Octave_Spawn(nOctaves,args,infokey)
%OCTAVE_SPAWN	Spawn an (optionally) given number of slave Octaves
%		possibly with a visible xterm (if global DEBUG true)
%		- if no nOctaves given, spawn as many as slave hosts
%		- args is a cell-of-strings passed to octave ({"--eval",...})
%		- optional infokey, if not MPI_INFO_NULL will be used
%
%	[children] = Octave_Spawn [ ( nOctaves [, args [, infokey] ] ) ]
%
%  Default number of slave hosts taken from global var NSLAVES
%  On return, global NOCTAVES is defined (number of spawned Octaves)
%  If global var DEBUG is true, slave Octaves run in an xterm
%  If global DISPLAY_REQUIRED,  slave Octaves run with the -display switch
%  If global DISPLAY  defined,  it is used, else taken from $DISPLAY
%  Global var NEWORLD is defined as the merged intercomm (parents,children)
%

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------

%%%%%%%%%%%%%%%%%%
% DEFAULT VALUES %
%%%%%%%%%%%%%%%%%%
% [info CPUS flag] = MPI_Attr_get(MPI_COMM_WORLD,LAM_UNIVERSE_NCPUS);
%   if info | ~flag,	error('Octave_Spawn: missing LAM_UNIVERSE_NCPUS attr?')
%   end
global NSLAVES DEBUG DISPLAY_REQUIRED DISPLAY
if isempty(NSLAVES), error('Octave_Spawn: NSLAVES undefined, use LAM_Init'), end
if isempty(DEBUG  ), DEBUG=true; end
if isempty(DISPLAY_REQUIRED), DISPLAY_REQUIRED=false; end
if	   DISPLAY_REQUIRED
if isempty(DISPLAY), DISPLAY=getenv('DISPLAY'); end
end

%%%%%%%%%%%%
% ARGCHECK %
%%%%%%%%%%%%
if nargin>2
  if ~isscalar(infokey)   || ~isreal(infokey) ||...
	   fix(infokey)~=infokey%|| infokey<0		% not sure about that
	error('Octave_Spawn: 3rd arg is not a valid INFO object')
  end
else	infokey = MPI_INFO_NULL;
end

if nargin>1
	if ~iscell(args)		% arglist cell
	    error('Octave_Spawn: 2nd arg is not a cell')
   else	for i=1:length(args)		% yet more,cell-of-strings
	  if ~ischar(args{i})
	    error('Octave_Spawn: 2nd arg is not cell-of-strings')
	  end
	end
   end
else	args = {};
end

if nargin>0
  if ~isscalar(nOctaves)   || ~isreal(nOctaves) ||...
	   fix(nOctaves)~=nOctaves || nOctaves<=0
	error('Octave_Spawn: arg is not a valid #Octaves')
  end
  if            nOctaves > NSLAVES
	error(['Octave_Spawn: required ' int2str(nOctaves),...
		' Octaves > ' int2str(NSLAVES) ' slave computers in LAM'])
  end
else	nOctaves = NSLAVES;
end

global	NOCTAVES
	NOCTAVES=nOctaves;

%%%%%%%%%
% SPAWN %
%%%%%%%%%
  if DEBUG
	cmd='xterm';				% Octave_Spawn(n,args,info)
%	cmd='/usr/bin/xterm';			% meaningful if args are for
%	cmd='/usr/X11R6/bin/xterm';		% the octave executable
	args={'-e','octave','-q',	args{:}};% not for xterm!!!
    if DISPLAY_REQUIRED
	args={'-display',DISPLAY,	args{:}};
    end
  else
	cmd='octave';
	args={              '-q',	args{:}};
  end

    [info children errs]=MPI_Comm_spawn(cmd,args,...
					nOctaves,infokey,0,MPI_COMM_SELF);

  if info | any(errs),		error('cannot start Octaves'), end
  [info numt] = MPI_Comm_remote_size(children);
  if info | numt~=nOctaves,	error('cannot start Octaves'), end

  global NEWORLD
  [info  NEWORLD]=MPI_Intercomm_merge(children,0);
  if info,			error('cannot start Octaves'), end

  if DEBUG
    MPI_Errhandler_set(       NEWORLD,MPI_ERRORS_RETURN);
    MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
  end

