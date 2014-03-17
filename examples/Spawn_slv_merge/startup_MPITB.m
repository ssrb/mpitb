## Copyright (C) 2004-2007 Javier Fernández Baldomero, Mancia Anguita López
##
## This program is free software
## -----------------------------

% startup_MPITB		PATH/environ script for MPITB
%			(add to/replacement for) .octaverc
%			customize the <myappname>-marked sections -> Spawn

## ----------------------------------------------------
## Author: Javier Fernández Baldomero <jfernand@ugr.es>, Mancia Anguita López
## Keywords: parallel MPI
## ----------------------------------------------------


%%%%%%%%
% PATH %
%%%%%%%%
p = getenv('MPITB_HOME');
if isempty(p)
 p=[getenv('HOME') '/octave/mpitb'];
    putenv('MPITB_HOME', p)
end
q = [p '/mpi'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (help files)'), end
addpath(q);
q = [p '/DLD'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (DLD files)'), end
addpath(q);
q = [p '/startups'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (startups)'), end
addpath(q);
q = [p '/utils'];
if ~exist(q, 'dir'), clear p q, error('MPI Toolbox not found (utilities)'), end
addpath(q);

  q = [p '/Spawn_slv_merge'];
  if ~exist(q, 'dir'), clear p q, error('Spawn(slv-merge) demo not found'), end
  addpath(q);

clear p q

%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Greeting / Slave startup %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% ignore_function_time_stamp('all')
% warning("off","MPITB:internal-errors")	% verbose internal debug errmsg
						% f.instance in (Scatter/Gather)
  warning("off","MPITB:Address-verbose")	% verbose MPI_Address
  warning("off","MPITB:getBuff-mk-unique")	% lazycopied vars -> duplicated
  warning("off","MPITB:getBuff-above-cnt")	% lazycopied vars (>2 copies)
  warning("off","MPITB:getBuff-normalcnt")	% vars with exactly 2 copies
% warning("off","MPITB:getBuff-below-cnt")	% should not happen
% warning("off","MPITB:getBuff-invalid-type")	% variable invalid as buffer
% warning("off","MPITB:Pack-invalid-type")	% invalid to MPI_[Un]Pack[_size]

if isempty(getenv('LAMPARENT')) &&...		% Parent task - welcome message
   isempty(getenv('OMPI_MCA_universe'))
  disp("    Minimal test:")
  disp(["  \tMPI_Init, [a b]=MPI_Initialized, ",...
	"MPI_Finalize, [a b]=MPI_Finalized"	])
  disp("    Examples of environment variables to affect MPI behaviour")
  disp("\tLAM/MPI SSI iface: putenv('LAM_MPI_SSI_rpi',     'lamd'), MPI_Init")
  disp("\tOpenMPI MCA iface: putenv('OMPI_MCA_btl_base_debug','2'), MPI_Init")
  disp('    Help on MPI: help mpi')
  disp('Help(this demo): help Spawn_oct_man, _oct_auto, _oct_bcast')

else					% spawned Octave process
	%% - All ranks are slaves if started via mpirun, and then
	%% the preferred setup is leaving this else branch commented out since
	%% slave startup can also be controlled by using "octave -eval" switch
	%% - If started via MPI_Comm_spawn, do use at least _mergeParent
	%% there is a selection of startup protocols in the startups subdir

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%% this line commented out/in for the TUTORIAL %%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  startup_mergeParent			% this line, remove the %
% startup_mergeParent			% yup, keep it commented out

	%% these lines used to invoke the different slave startup protocols
	%% see examples in the Spawn/PingPong/Pi/Wavelets subdirs

% startup_bcastSlv	% BCast protocol: 1 command is sent using MPI_Bcast
% startup_ncmdsSlv	% NumCommands protocol: several commands are sent

end  

