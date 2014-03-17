## Copyright (C) 2004-2007 Javier Fernández Baldomero
##
## This program is free software
## -----------------------------

% startup_MPITB		PATH/environ script for MPITB
%			(add to/replacement for) .octaverc
%			customize the <myappname>-marked sections -> PingPong

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>
## Keywords: parallel MPI
## ----------------------------------------------------


%%%%%%%%
% PATH %
%%%%%%%%
L=path;

p = getenv('MPITB_HOME');
if isempty(p)
 p=[getenv('HOME') '/octave/mpitb'];
    putenv('MPITB_HOME', p)
end

q = [p '/mpi'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (help files)'), end
L=path(L,q);
q = [p '/DLD'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (DLD files)'), end
L=path(L,q);
q = [p '/startups'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (startups)'), end
L=path(L,q);
q = [p '/utils'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (utilities)'), end
L=path(L,q);

  q = [p '/PingPong'];
  if ~exist(q, 'dir'), clear p q, error('PingPong demo not found'), end
  L=path(L,q);

clear p q L

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Greeting / Slave startup %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ignore_function_time_stamp='all';

if isempty(getenv('LAMPARENT'))		% Parent task - welcome message
  disp('Set SSI rpi to tcp with the command:')
  disp('  putenv(''LAM_MPI_SSI_rpi'',''tcp''), MPI_Init')
  disp('Help on MPI: help mpi')
  disp('Help on this demo: help PingPong')

else					% spawned Octave process
	%% - All ranks are slaves if started via mpirun, and then
	%% the preferred setup is leaving this else branch commented out since
	%% slave startup can also be controlled by using "octave -eval" switch
	%% - If started via MPI_Comm_spawn, do use at least _mergeParent
	%% there is a selection of startup protocols in the startups subdir

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%% this line commented out/in for the TUTORIAL %%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% startup_mergeParent			% this line, remove the %
% startup_mergeParent			% yup, keep it commented out

	%% these lines used to invoke the different slave startup protocols
	%% see examples in the Spawn/PingPong/Pi/Wavelets subdirs

% startup_bcastSlv	% BCast protocol: 1 command is sent using MPI_Bcast
% startup_ncmdsSlv	% NumCommands protocol: several commands are sent

end  

